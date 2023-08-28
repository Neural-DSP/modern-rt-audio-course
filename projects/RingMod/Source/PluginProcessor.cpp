#include "PluginProcessor.h"
#include "PluginEditor.h"

static const std::vector<mrta::ParameterInfo> parameters
{
    { Param::ID::ModRate,  Param::Name::ModRate,  Param::Unit::Hz,  1.f,  Param::Range::ModRateMin,  Param::Range::ModRateMax,  Param::Range::ModRateInc,  Param::Range::ModRateSkw },
    { Param::ID::ModDepth, Param::Name::ModDepth, Param::Unit::Pct, 50.f, Param::Range::ModDepthMin, Param::Range::ModDepthMax, Param::Range::ModDepthInc, Param::Range::ModDepthSkw },
    { Param::ID::ModType,  Param::Name::ModType,  Param::Range::ModTypeLabels, 0 }
};

RingModAudioProcessor::RingModAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, parameters)
{
    parameterManager.registerParameterCallback(Param::ID::ModRate,
    [this] (float value, bool /*force*/)
    {
        ringMod.setModRate(value);
    });

    parameterManager.registerParameterCallback(Param::ID::ModDepth,
    [this] (float value, bool /*force*/)
    {
        ringMod.setModDepth(value * 0.01f); // Convert from % to [0; 1]
    });

    parameterManager.registerParameterCallback(Param::ID::ModType,
    [this] (float value, bool /*force*/)
    {
        mrta::RingMod::ModType modType = static_cast<mrta::RingMod::ModType>(std::round(value));
        ringMod.setModType(modType);
    });
}

RingModAudioProcessor::~RingModAudioProcessor()
{
}

void RingModAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    ringMod.prepare(sampleRate);
    parameterManager.updateParameters(true);
}

void RingModAudioProcessor::releaseResources()
{
}

void RingModAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels{ static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples{ static_cast<unsigned int>(buffer.getNumSamples()) };

    ringMod.process(buffer.getArrayOfWritePointers(), buffer.getArrayOfReadPointers(), numChannels, numSamples);
}

void RingModAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void RingModAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
const juce::String RingModAudioProcessor::getName() const { return JucePlugin_Name; }
bool RingModAudioProcessor::acceptsMidi() const { return false; }
bool RingModAudioProcessor::producesMidi() const { return false; }
bool RingModAudioProcessor::isMidiEffect() const { return false; }
double RingModAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int RingModAudioProcessor::getNumPrograms() { return 1; }
int RingModAudioProcessor::getCurrentProgram() { return 0; }
void RingModAudioProcessor::setCurrentProgram(int) { }
const juce::String RingModAudioProcessor::getProgramName(int) { return {}; }
void RingModAudioProcessor::changeProgramName(int, const juce::String&) { }
bool RingModAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* RingModAudioProcessor::createEditor() { return new RingModAudioProcessorEditor(*this); }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingModAudioProcessor();
}
