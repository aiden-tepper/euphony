/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 */
class EuphonyAudioProcessor : public juce::AudioProcessor
{
public:
  //==============================================================================
  EuphonyAudioProcessor();
  ~EuphonyAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  //==============================================================================
  const std::vector<std::string> getNextChords(const std::string &curr, const std::string &key, const std::string &major_minor);
  void generateProgression(const std::vector<std::string> &progression, const std::string &key, const std::string &major_minor);
  void resetGUI();
  void addChord(std::string chord);
  void playProgression();
  ProgressionManager prog;

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EuphonyAudioProcessor)
};

class ProgressionManager
{
public:
  // Accessors
  const std::vector<std::string> &getStrProgression() const { return strProgression; }
  const std::vector<std::vector<int>> &getIntProgression() const { return intProgression; }
  char getKey() const { return key; }
  char getSharpFlat() const { return sharpFlat; }
  const std::string &getMajorMinor() const { return majorMinor; }

  // Mutators
  void setKey(char newKey) { key = newKey; }
  void setSharpFlat(char newSharpFlat) { sharpFlat = newSharpFlat; }
  void setMajorMinor(std::string newMajorMinor) { majorMinor = newMajorMinor; }
  void setIntProgression(const std::vector<std::vector<int>> &newIntProgression) { intProgression = newIntProgression; }

  // Special function for strProgression
  void addToStrProgression(const std::string &chord) { strProgression.push_back(chord); }

private:
  std::vector<std::string> strProgression;
  std::vector<std::vector<int>> intProgression;
  char key;
  char sharpFlat;
  std::string majorMinor;
};