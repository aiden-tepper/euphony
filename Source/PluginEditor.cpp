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
  chordDropdown.addItem("Chord 1", 1);
  chordDropdown.addItem("Chord 2", 2);
  addAndMakeVisible(chordDropdown);

  keyDropdown.addItem("Key 1", 1);
  keyDropdown.addItem("Key 2", 2);
  addAndMakeVisible(keyDropdown);

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

  // TESTING
  for (const auto &item : audioProcessor.getArrayOfStrings())
  {
    std::string itemName = item;
    std::cout << itemName << std::endl;
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
  const int dropdownWidth = 150;
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

  // keyLabel and keyDropdown - Center-justified horizontally, top of the screen
  keyLabel.setBounds(centerX - dropdownWidth / 2 - 100, margin, 100, labelHeight); // Adjust width as needed
  keyDropdown.setBounds(centerX - dropdownWidth / 2, margin + labelHeight, dropdownWidth, dropdownHeight);

  // resetButton - Top right
  resetButton.setBounds(getWidth() - margin - buttonWidth, margin, buttonWidth, buttonHeight);

  // progressionImageComponent - Center-justified vertically, right-justified horizontally
  int imageWidth = 400;  // Adjust as needed
  int imageHeight = 300; // Adjust as needed
  progressionImageComponent.setBounds(getWidth() - margin - imageWidth, centerY - imageHeight / 2, imageWidth, imageHeight);

  // playButton - Immediately beneath progressionImageComponent
  playButton.setBounds(getWidth() - margin - buttonWidth, centerY + imageHeight / 2 + margin, buttonWidth, buttonHeight);
}