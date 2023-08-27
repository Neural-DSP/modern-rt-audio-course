#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <algorithm>

static const std::vector<mrta::ParameterInfo> Parameters
{
    { Param::ID::Enabled,  Param::Name::Enabled,  Param::Ranges::EnabledOff, Param::Ranges::EnabledOn, true },
    { Param::ID::Offset,   Param::Name::Offset,   Param::Units::Ms,  2.f,  Param::Ranges::OffsetMin,   Param::Ranges::OffsetMax,   Param::Ranges::OffsetInc,   Param::Ranges::OffsetSkw },
    { Param::ID::Depth,    Param::Name::Depth,    Param::Units::Ms,  2.f,  Param::Ranges::DepthMin,    Param::Ranges::DepthMax,    Param::Ranges::DepthInc,    Param::Ranges::DepthSkw },
    { Param::ID::Feedback, Param::Name::Feedback, Param::Units::Pct, 0.f,  Param::Ranges::FeedbackMin, Param::Ranges::FeedbackMax, Param::Ranges::FeedbackInc, Param::Ranges::FeedbackSkw },
    { Param::ID::Rate,     Param::Name::Rate,     Param::Units::Hz,  0.5f, Param::Ranges::RateMin,     Param::Ranges::RateMax,     Param::Ranges::RateInc,     Param::Ranges::RateSkw },
    { Param::ID::ModType,  Param::Name::ModType,  Param::Ranges::ModLabels, 0 }
};

FlangerAudioProcessor::FlangerAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, Parameters),
    flanger(20.f, 2),
    enableRamp(0.05f)
{
    parameterManager.registerParameterCallback(Param::ID::Enabled,
    [this](float newValue, bool force)
    {
        enableRamp.setTarget(std::fmin(std::fmax(newValue, 0.f), 1.f), force);
    });

    parameterManager.registerParameterCallback(Param::ID::Offset,
    [this] (float newValue, bool /*force*/)
    {
        flanger.setOffset(newValue);
    });

    parameterManager.registerParameterCallback(Param::ID::Depth,
    [this](float newValue, bool /*force*/)
    {
        flanger.setDepth(newValue);
    });

    parameterManager.registerParameterCallback(Param::ID::Feedback,
    [this](float newValue, bool /*force*/)
    {
        flanger.setFeedback(newValue * 0.01f);
    });

    parameterManager.registerParameterCallback(Param::ID::Rate,
    [this] (float newValue, bool /*force*/)
    {
        flanger.setModulationRate(newValue);
    });

    parameterManager.registerParameterCallback(Param::ID::ModType,
    [this](float newValue, bool /*force*/)
    {
        mrta::Flanger::ModulationType modType = static_cast<mrta::Flanger::ModulationType>(std::round(newValue));
        flanger.setModulationType(std::min(std::max(modType, mrta::Flanger::Sin), mrta::Flanger::Saw));
    });
}

FlangerAudioProcessor::~FlangerAudioProcessor()
{
}

void FlangerAudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    const unsigned int numChannels { static_cast<unsigned int>(std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels())) };

    flanger.prepare(newSampleRate, 20.f, numChannels);
    enableRamp.prepare(newSampleRate);

    parameterManager.updateParameters(true);

    fxBuffer.setSize(static_cast<int>(numChannels), samplesPerBlock);
    fxBuffer.clear();
}

void FlangerAudioProcessor::releaseResources()
{
    flanger.clear();
}

void FlangerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    const unsigned int numChannels { static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples { static_cast<unsigned int>(buffer.getNumSamples()) };

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        fxBuffer.copyFrom(ch, 0, buffer, ch, 0, static_cast<int>(numSamples));

    flanger.process(fxBuffer.getArrayOfWritePointers(), fxBuffer.getArrayOfReadPointers(), numChannels, numSamples);
    enableRamp.applyGain(fxBuffer.getArrayOfWritePointers(), numChannels, numSamples);

    for (int ch = 0; ch < static_cast<int>(numChannels); ++ch)
        buffer.addFrom(ch, 0, fxBuffer, ch, 0, static_cast<int>(numSamples));
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
const juce::String FlangerAudioProcessor::getProgramName (int) { return {}; }
void FlangerAudioProcessor::changeProgramName (int, const juce::String&) { }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangerAudioProcessor();
}
