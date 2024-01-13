/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EuphonyAudioProcessorEditor::EuphonyAudioProcessorEditor(EuphonyAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
  // This is where our plugin’s editor size is set.
  setSize(1400, 1000);

  // Buttons
  generateButton.setButtonText("Generate");
  addAndMakeVisible(generateButton);
  generateButton.onClick = [this]
  {
    DBG("Generate button clicked.");
    audioProcessor.generateProgression();
    updateNotation();
  };
  // onClick -> generateProgression()
  // sends chordList to python binding, generates and displays lilypond image, loads audio file (or maybe it should just be played on the spot?)

  resetButton.setButtonText("Reset");
  addAndMakeVisible(resetButton);
  resetButton.onClick = [this]
  {
    audioProcessor.clearChords();
    progressionLabel.setText("", juce::NotificationType::dontSendNotification);
    keyDropdown.setSelectedId(0);
    sharpFlatDropdown.setSelectedId(1);
    majorMinorDropdown.setSelectedId(0);
    // chordDropdown.setSelectedId(0);
    chordDropdown.clear(juce::NotificationType::dontSendNotification);
    juce::File imageFile("/home/aiden/Documents/Euphony/Source/resources/placeholder.png");
    juce::Image progressionImage = juce::ImageFileFormat::loadFrom(imageFile);
    if (progressionImage.isNull())
    {
      DBG("Image failed to load.");
    }
    else
    {
      progressionImageComponent.setImage(progressionImage);
      addAndMakeVisible(progressionImageComponent);
    }
  };
  // onClick -> resetGUI()
  // resets all fields -- is it possible to just restart/redraw the GUI?

  addButton.setButtonText("Add");
  addAndMakeVisible(addButton);
  addButton.onClick = [this]
  {
    std::string chord = chordDropdown.getText().toStdString();
    audioProcessor.addChord(chord);
    DBG(chord);
    progressionLabel.setText(progressionLabel.getText() + chord + " - ", juce::NotificationType::dontSendNotification);
    chordDropdown.clear(juce::NotificationType::dontSendNotification);
    int i = 1;
    for (const auto &item : audioProcessor.getNextChords(chord, std::string(1, audioProcessor.prog.getKey()), audioProcessor.prog.getMajorMinor()))
    {
      std::string itemName = item;
      chordDropdown.addItem(itemName, i++);
      // std::cout << itemName << std::endl;
    }
  };
  // onClick -> addChord()
  // adds chord to chordList and appends to progressionLabel

  playButton.setButtonText("Play");
  addAndMakeVisible(playButton);
  playButton.onClick = [this]
  {
    audioProcessor.playProgression();
  };
  // onClick -> playProgression()
  // triggers audio playback of voice leading

  setKeyButton.setButtonText("Set");
  addAndMakeVisible(setKeyButton);
  setKeyButton.onClick = [this]
  {
    audioProcessor.prog.setKey(keyDropdown.getText()[0]);
    audioProcessor.prog.setSharpFlat(sharpFlatDropdown.getText()[0]);
    audioProcessor.prog.setMajorMinor(majorMinorDropdown.getText().toStdString());
    audioProcessor.clearChords();
    progressionLabel.setText("", juce::NotificationType::dontSendNotification);
    DBG("SET KEY");
    std::string chord;
    if (strcmp(audioProcessor.prog.getMajorMinor().c_str(), "major") == 0)
    {
      chord = "I";
    }
    else
    {
      chord = "i";
    }
    int i = 1;
    for (const auto &item : audioProcessor.getNextChords(chord, std::string(1, audioProcessor.prog.getKey()), audioProcessor.prog.getMajorMinor()))
    {
      std::string itemName = item;
      chordDropdown.addItem(itemName, i++);
      // std::cout << itemName << std::endl;
    }
  };
  // onClick -> setKey()
  // sets key

  // Dropdown Menus
  addAndMakeVisible(chordDropdown);

  keyDropdown.addItem("C", 1);
  keyDropdown.addItem("D", 2);
  keyDropdown.addItem("E", 3);
  keyDropdown.addItem("F", 4);
  keyDropdown.addItem("G", 5);
  keyDropdown.addItem("A", 6);
  keyDropdown.addItem("B", 7);
  addAndMakeVisible(keyDropdown);
  // keyDropdown.onChange = [this]
  // {
  //   audioProcessor.prog.setKey(keyDropdown.getText()[0]);
  //   audioProcessor.clearChords();
  //   progressionLabel.setText("", juce::NotificationType::dontSendNotification);
  //   DBG(keyDropdown.getText()[0]);
  // };
  keyDropdown.setSelectedId(0);

  sharpFlatDropdown.addItem(" ", 1);
  sharpFlatDropdown.addItem("#", 2);
  sharpFlatDropdown.addItem("b", 3);
  addAndMakeVisible(sharpFlatDropdown);
  // sharpFlatDropdown.onChange = [this]
  // {
  //   audioProcessor.prog.setSharpFlat(sharpFlatDropdown.getText()[0]);
  //   audioProcessor.clearChords();
  //   progressionLabel.setText("", juce::NotificationType::dontSendNotification);
  //   DBG(sharpFlatDropdown.getText()[0]);
  // };
  sharpFlatDropdown.setSelectedId(1);

  majorMinorDropdown.addItem("major", 1);
  majorMinorDropdown.addItem("minor", 2);
  addAndMakeVisible(majorMinorDropdown);
  // majorMinorDropdown.onChange = [this]
  // {
  //   audioProcessor.prog.setMajorMinor(majorMinorDropdown.getText().toStdString());
  //   audioProcessor.clearChords();
  //   progressionLabel.setText("", juce::NotificationType::dontSendNotification);
  //   DBG(majorMinorDropdown.getText().toStdString());
  // };
  majorMinorDropdown.setSelectedId(0);

  /**
   * upon any state changes in any of the above 3 dropdowns, reset the chordList and progressionLabel
   */

  // Labels
  titleLabel.setText("Euphony", juce::dontSendNotification);
  addAndMakeVisible(titleLabel);

  nextChordLabel.setText("Next Chord:", juce::dontSendNotification);
  addAndMakeVisible(nextChordLabel);

  progressionLabel.setText("", juce::dontSendNotification);
  addAndMakeVisible(progressionLabel);

  keyLabel.setText("Key: ", juce::dontSendNotification);
  addAndMakeVisible(keyLabel);

  // Images
  juce::File imageFile("/home/aiden/Documents/Euphony/Source/resources/placeholder.png");
  juce::Image progressionImage = juce::ImageFileFormat::loadFrom(imageFile);
  if (progressionImage.isNull())
  {
    DBG("Image failed to load.");
  }
  else
  {
    progressionImageComponent.setImage(progressionImage);
    addAndMakeVisible(progressionImageComponent);
  }
}

void EuphonyAudioProcessorEditor::updateNotation()
{
  juce::File imageFile("/home/aiden/Documents/Euphony/Source/resources/output.preview.png");
  juce::Image progressionImage = juce::ImageFileFormat::loadFrom(imageFile);
  if (progressionImage.isNull())
  {
    DBG("Image failed to load.");
  }
  else
  {
    progressionImageComponent.setImage(progressionImage);
    addAndMakeVisible(progressionImageComponent);
  }
}

EuphonyAudioProcessorEditor::~EuphonyAudioProcessorEditor()
{
}

//==============================================================================
void EuphonyAudioProcessorEditor::paint(juce::Graphics &g)
{
  // fill the whole window white
  g.fillAll(juce::Colours::transparentBlack);
}

void EuphonyAudioProcessorEditor::resized()
{
  // General dimensions
  const int buttonWidth = 100;
  const int buttonHeight = 30;
  const int dropdownWidth = 100;
  const int dropdownHeight = 30;
  const int labelHeight = 20;
  const int margin = 10;

  // Calculate center positions
  int centerX = getWidth() / 2;
  int centerY = getHeight() / 2;

  // titleLabel - Top left
  titleLabel.setBounds(margin, margin, 200, labelHeight); // Adjust width as needed

  // nextChordLabel - Centered vertically, left-justified horizontally
  nextChordLabel.setBounds(margin, centerY - labelHeight - dropdownHeight / 2 - margin, 200, labelHeight); // Adjust width as needed

  // chordDropdown - Right beneath nextChordLabel
  chordDropdown.setBounds(margin, centerY - dropdownHeight / 2, dropdownWidth, dropdownHeight);

  // addButton - Immediately to the right of chordDropdown
  addButton.setBounds(margin + dropdownWidth, centerY - dropdownHeight / 2, buttonWidth, buttonHeight);

  // generateButton - Bottom left
  generateButton.setBounds(margin, getHeight() - margin - buttonHeight, buttonWidth, buttonHeight);

  // progressionLabel - Immediately above generateButton, left-justified horizontally
  progressionLabel.setBounds(margin, getHeight() - 2 * margin - buttonHeight - labelHeight, 1200, labelHeight); // Adjust width as needed

  // keyLabel and associated dropdowns and button - Center-justified horizontally, top of the screen
  keyLabel.setBounds(centerX - dropdownWidth / 2 - 132, margin + labelHeight, 100, labelHeight); // Adjust width as needed
  keyDropdown.setBounds(centerX - (dropdownWidth - 25) / 2 - 40, margin + labelHeight, dropdownWidth - 25, dropdownHeight);
  sharpFlatDropdown.setBounds(centerX - (dropdownWidth - 25) / 2 + 40, margin + labelHeight, dropdownWidth - 25, dropdownHeight);
  majorMinorDropdown.setBounds(centerX - dropdownWidth / 2 + 132, margin + labelHeight, dropdownWidth, dropdownHeight);
  setKeyButton.setBounds(centerX - dropdownWidth / 2 + 220, margin + labelHeight, 100, labelHeight);

  // resetButton - Top right
  resetButton.setBounds(getWidth() - margin - buttonWidth, margin, buttonWidth, buttonHeight);

  // progressionImageComponent - Center-justified vertically, right-justified horizontally
  int imageWidth = 400;  // Adjust as needed
  int imageHeight = 300; // Adjust as needed
  progressionImageComponent.setBounds(getWidth() - margin - imageWidth, centerY - imageHeight / 2, imageWidth, imageHeight);

  // playButton - Immediately beneath progressionImageComponent
  playButton.setBounds(getWidth() - margin - buttonWidth, centerY + imageHeight / 2 + margin, buttonWidth, buttonHeight);
}
