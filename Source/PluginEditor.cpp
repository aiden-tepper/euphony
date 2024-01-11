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

  resetButton.setButtonText("Reset");
  addAndMakeVisible(resetButton);

  addButton.setButtonText("Add");
  addAndMakeVisible(addButton);

  playButton.setButtonText("Play");
  addAndMakeVisible(playButton);

  // Dropdown Menus
  int i = 1;
  for (const auto &item : audioProcessor.getNextChords("I", "C", "major"))
  {
    std::string itemName = item;
    chordDropdown.addItem(itemName, i++);
    // std::cout << itemName << std::endl;
  }
  addAndMakeVisible(chordDropdown);

  keyDropdown.addItem("C", 1);
  keyDropdown.addItem("D", 2);
  keyDropdown.addItem("E", 3);
  keyDropdown.addItem("F", 4);
  keyDropdown.addItem("G", 5);
  keyDropdown.addItem("A", 6);
  keyDropdown.addItem("B", 7);
  addAndMakeVisible(keyDropdown);

  sharpFlatDropdown.addItem(" ", 1);
  sharpFlatDropdown.addItem("#", 2);
  sharpFlatDropdown.addItem("b", 3);
  addAndMakeVisible(sharpFlatDropdown);

  majorMinorDropdown.addItem("Major", 1);
  majorMinorDropdown.addItem("Minor", 2);
  addAndMakeVisible(majorMinorDropdown);

  // Labels
  titleLabel.setText("Euphony", juce::dontSendNotification);
  addAndMakeVisible(titleLabel);

  nextChordLabel.setText("Next Chord:", juce::dontSendNotification);
  addAndMakeVisible(nextChordLabel);

  progressionLabel.setText("Dm G7 Cmaj7 ...", juce::dontSendNotification);
  addAndMakeVisible(progressionLabel);

  keyLabel.setText("Key: ", juce::dontSendNotification);
  addAndMakeVisible(keyLabel);

  // Images
  juce::File imageFile("Source/test_image.png");
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
  g.fillAll(juce::Colours::black);
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
  progressionLabel.setBounds(margin, getHeight() - 2 * margin - buttonHeight - labelHeight, 200, labelHeight); // Adjust width as needed

  // keyLabel and associated dropdowns - Center-justified horizontally, top of the screen
  keyLabel.setBounds(centerX - dropdownWidth / 2 - 132, margin + labelHeight, 100, labelHeight); // Adjust width as needed
  keyDropdown.setBounds(centerX - (dropdownWidth-25) / 2 - 40, margin + labelHeight, dropdownWidth - 25, dropdownHeight);
  sharpFlatDropdown.setBounds(centerX - (dropdownWidth-25) / 2 + 40, margin + labelHeight, dropdownWidth - 25, dropdownHeight);
  majorMinorDropdown.setBounds(centerX - dropdownWidth / 2 + 132, margin + labelHeight, dropdownWidth, dropdownHeight);

  // resetButton - Top right
  resetButton.setBounds(getWidth() - margin - buttonWidth, margin, buttonWidth, buttonHeight);

  // progressionImageComponent - Center-justified vertically, right-justified horizontally
  int imageWidth = 400;  // Adjust as needed
  int imageHeight = 300; // Adjust as needed
  progressionImageComponent.setBounds(getWidth() - margin - imageWidth, centerY - imageHeight / 2, imageWidth, imageHeight);

  // playButton - Immediately beneath progressionImageComponent
  playButton.setBounds(getWidth() - margin - buttonWidth, centerY + imageHeight / 2 + margin, buttonWidth, buttonHeight);
}