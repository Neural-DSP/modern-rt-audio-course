#include "PluginProcessor.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> Parameters
{
    { Param::ID::Enabled,  Param::Name::Enabled,  Param::Ranges::EnabledOff, Param::Ranges::EnabledOn, true },
    { Param::ID::Offset,   Param::Name::Offset,   Param::Units::Ms,  2.f,  Param::Ranges::OffsetMin,   Param::Ranges::OffsetMax,   Param::Ranges::OffsetInc,   Param::Ranges::OffsetSkw },
    { Param::ID::Depth,    Param::Name::Depth,    Param::Units::Ms,  2.f,  Param::Ranges::DepthMin,    Param::Ranges::DepthMax,    Param::Ranges::DepthInc,    Param::Ranges::DepthSkw },
    { Param::ID::Feedback, Param::Name::Feedback, Param::Units::Pct, 50.f, Param::Ranges::FeedbackMin, Param::Ranges::FeedbackMax, Param::Ranges::FeedbackInc, Param::Ranges::FeedbackSkw },
    { Param::ID::Rate,     Param::Name::Rate,     Param::Units::Hz,  0.5f, Param::Ranges::RateMin,     Param::Ranges::RateMax,     Param::Ranges::RateInc,     Param::Ranges::RateSkw },
    { Param::ID::ModType,  Param::Name::ModType,  Param::Ranges::ModLabels, 0 }
};

FlangerAudioProcessor::FlangerAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, Parameters)
{
}

FlangerAudioProcessor::~FlangerAudioProcessor()
{
}

void FlangerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    parameterManager.updateParameters(true);
}

void FlangerAudioProcessor::releaseResources()
{
}

void FlangerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();
}

void FlangerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void FlangerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
bool FlangerAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* FlangerAudioProcessor::createEditor() { return new FlangerAudioProcessorEditor(*this); }
const juce::String FlangerAudioProcessor::getName() const { return JucePlugin_Name; }
bool FlangerAudioProcessor::acceptsMidi() const { return false; }
bool FlangerAudioProcessor::producesMidi() const { return false; }
bool FlangerAudioProcessor::isMidiEffect() const { return false; }
double FlangerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int FlangerAudioProcessor::getNumPrograms() { return 1; }
int FlangerAudioProcessor::getCurrentProgram() { return 0; }
void FlangerAudioProcessor::setCurrentProgram(int) { }
const juce::String FlangerAudioProcessor::getProgramName (int index) { return {}; }
void FlangerAudioProcessor::changeProgramName (int, const juce::String&) { }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangerAudioProcessor();
}
