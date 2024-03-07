/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <pybind11/embed.h>
namespace py = pybind11;

//==============================================================================
EuphonyAudioProcessor::EuphonyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
}

EuphonyAudioProcessor::~EuphonyAudioProcessor()
{
}

//==============================================================================
const juce::String EuphonyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EuphonyAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool EuphonyAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool EuphonyAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double EuphonyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EuphonyAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int EuphonyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EuphonyAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String EuphonyAudioProcessor::getProgramName(int index)
{
    return {};
}

void EuphonyAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void EuphonyAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EuphonyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EuphonyAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void EuphonyAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{

    // continually called
}

//==============================================================================
bool EuphonyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *EuphonyAudioProcessor::createEditor()
{
    return new EuphonyAudioProcessorEditor(*this);
}

//==============================================================================
void EuphonyAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EuphonyAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new EuphonyAudioProcessor();
}

//==============================================================================
const std::vector<std::string> EuphonyAudioProcessor::getNextChords(const std::string &curr, const std::string &key, const std::string &major_minor)
{
    py::scoped_interpreter guard{}; // Start the interpreter
    py::exec("import sys; sys.path.append('/home/aiden/Documents/euphony/Source/voice-leading-cmdline')");

    try
    {
        py::module_ bindings = py::module_::import("bindings");
        py::object next_chords = bindings.attr("next_chords")(curr, key, major_minor);

        py::list dropdownItems = next_chords.cast<py::list>();
        std::vector<std::string> dropdownItemsVector;

        for (const auto &item : dropdownItems)
        {
            std::string itemStr = py::str(item);
            dropdownItemsVector.push_back(itemStr);
        }
        return dropdownItemsVector;
    }
    catch (const py::error_already_set &e)
    {
        std::cerr << "Python error: " << e.what() << std::endl;
        return {};
    }
}

void EuphonyAudioProcessor::generateProgression()
{
    py::scoped_interpreter guard{}; // Start the interpreter
    py::exec("import sys; sys.path.append('/home/aiden/Documents/Euphony/Source/voice-leading-cmdline')");

    try
    {
        py::module_ bindings = py::module_::import("bindings");

        std::vector<std::string> progression = prog.getStrProgression();
        // std::vector<std::string> progression = {"ii", "V", "I"};
        py::list progression_pylist;
        for (const auto &item : progression)
        {
            progression_pylist.append(py::cast(item));
        }

        std::string key(1, prog.getKey()); // Convert character to string
        std::string major_minor = prog.getMajorMinor();

        py::object result = bindings.attr("generate_progression")(&progression_pylist, key, major_minor);
        py::list strProgression = result.cast<py::list>();

        // convert to std::vector<std::vector<int>>
        std::vector<std::vector<int>> intProgression;
        for (const auto &item : strProgression)
        {
            std::string itemStr = py::str(item).cast<std::string>();

            // Remove brackets
            itemStr.erase(std::remove(itemStr.begin(), itemStr.end(), '['), itemStr.end());
            itemStr.erase(std::remove(itemStr.begin(), itemStr.end(), ']'), itemStr.end());

            // Replace commas with spaces
            std::replace(itemStr.begin(), itemStr.end(), ',', ' ');

            std::vector<int> innerVector;
            std::istringstream ss(itemStr);

            int number;
            while (ss >> number)
            {
                innerVector.push_back(number);
            }

            if (innerVector.empty())
            {
                std::cout << "No integers parsed for item: " << itemStr << std::endl;
            }
            else
            {
                intProgression.push_back(innerVector);
            }
        }

        prog.setIntProgression(intProgression);

        // print intProgression for debugging
        for (const auto &innerVector : intProgression)
        {
            for (const auto &number : innerVector)
            {
                std::cout << number << " ";
            }
            std::cout << std::endl;
        }

        // TODO: load resources/notes.png into GUI
    }
    catch (const py::error_already_set &e)
    {
        std::cerr << "Python error: " << e.what() << std::endl;
    }
}

void EuphonyAudioProcessor::resetGUI()
{
    // redraw GUI? reset all fields somehow?
}

void EuphonyAudioProcessor::clearChords()
{
    prog.setStrProgression({});
}

void EuphonyAudioProcessor::addChord(std::string chord)
{
    prog.addToStrProgression(chord);
}

void EuphonyAudioProcessor::playProgression()
{
    py::scoped_interpreter guard{}; // Start the interpreter
    py::exec("import sys; sys.path.append('/home/aiden/Documents/euphony/Source/voice-leading-cmdline')");

    try
    {
        py::module_ bindings = py::module_::import("bindings");

        std::vector<std::vector<int>> progression = prog.getIntProgression();
        py::list progression_pylist;
        for (const auto &item : progression)
        {
            py::list inner_list;
            for (const auto &inner_item : item)
            {
                inner_list.append(inner_item);
            }
            progression_pylist.append(inner_list);
        }

        py::object play_audio = bindings.attr("play_audio")(&progression_pylist);
    }
    catch (const py::error_already_set &e)
    {
        std::cerr << "Python error: " << e.what() << std::endl;
    }
}
