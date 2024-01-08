/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 */
class EuphonyAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
  EuphonyAudioProcessorEditor(EuphonyAudioProcessor &);
  ~EuphonyAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  EuphonyAudioProcessor &audioProcessor;

  // Buttons
  juce::TextButton generateButton;
  juce::TextButton resetButton;
  juce::TextButton addButton;
  juce::TextButton playButton;

  // Dropdown Menus
  juce::ComboBox chordDropdown;
  juce::ComboBox keyDropdown;

  // Labels
  juce::Label titleLabel;
  juce::Label nextChordLabel;
  juce::Label progressionLabel;
  juce::Label keyLabel;

  // Images
  juce::ImageComponent progressionImageComponent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EuphonyAudioProcessorEditor)
};
