#include "PluginProcessor.h"
#include "PluginEditor.h"

static std::vector<mrta::ParameterInfo> parameters
{
    { Param::ID::Band0Enabled, Param::Name::Band0Enabled, "Off", "On", true },
    { Param::ID::Band0Type, Param::Name::Band0Type, Param::Ranges::Types, mrta::ParametricEqualizer::LowShelf },
    { Param::ID::Band0Freq, Param::Name::Band0Freq, Param::Unit::Freq, 100.f, Param::Ranges::FreqMin, Param::Ranges::FreqMax, Param::Ranges::FreqInc, Param::Ranges::FreqSkw },
    { Param::ID::Band0Reso, Param::Name::Band0Reso, "", 0.71f, Param::Ranges::ResoMin, Param::Ranges::ResoMax, Param::Ranges::ResoInc, Param::Ranges::ResoSkw },
    { Param::ID::Band0Gain, Param::Name::Band0Gain, Param::Unit::Gain, 0.f, Param::Ranges::GainMin, Param::Ranges::GainMax, Param::Ranges::GainInc, Param::Ranges::GainSkw },

    { Param::ID::Band1Enabled, Param::Name::Band1Enabled, "Off", "On", true },
    { Param::ID::Band1Type, Param::Name::Band1Type, Param::Ranges::Types, mrta::ParametricEqualizer::Peak },
    { Param::ID::Band1Freq, Param::Name::Band1Freq, Param::Unit::Freq, 1000.f, Param::Ranges::FreqMin, Param::Ranges::FreqMax, Param::Ranges::FreqInc, Param::Ranges::FreqSkw },
    { Param::ID::Band1Reso, Param::Name::Band1Reso, "", 0.71f, Param::Ranges::ResoMin, Param::Ranges::ResoMax, Param::Ranges::ResoInc, Param::Ranges::ResoSkw },
    { Param::ID::Band1Gain, Param::Name::Band1Gain, Param::Unit::Gain, 0.f, Param::Ranges::GainMin, Param::Ranges::GainMax, Param::Ranges::GainInc, Param::Ranges::GainSkw },

    { Param::ID::Band2Enabled, Param::Name::Band2Enabled, "Off", "On", true },
    { Param::ID::Band2Type, Param::Name::Band2Type, Param::Ranges::Types, mrta::ParametricEqualizer::HighShelf },
    { Param::ID::Band2Freq, Param::Name::Band2Freq, Param::Unit::Freq, 10000.f, Param::Ranges::FreqMin, Param::Ranges::FreqMax, Param::Ranges::FreqInc, Param::Ranges::FreqSkw },
    { Param::ID::Band2Reso, Param::Name::Band2Reso, "", 0.71f, Param::Ranges::ResoMin, Param::Ranges::ResoMax, Param::Ranges::ResoInc, Param::Ranges::ResoSkw },
    { Param::ID::Band2Gain, Param::Name::Band2Gain, Param::Unit::Gain, 0.f, Param::Ranges::GainMin, Param::Ranges::GainMax, Param::Ranges::GainInc, Param::Ranges::GainSkw },
};

ParametricEQAudioProcessor::ParametricEQAudioProcessor() :
    parameterManager(*this, ProjectInfo::projectName, parameters),
    eq(3)
{
    parameterManager.registerParameterCallback(Param::ID::Band0Type,
    [this] (float val, bool /*force*/)
    {
        eq.setBandType(0, static_cast<mrta::ParametricEqualizer::FilterType>(std::round(val)));
    });

    parameterManager.registerParameterCallback(Param::ID::Band0Freq,
    [this] (float val, bool /*force*/)
    {
        eq.setBandFrequency(0, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band0Reso,
    [this] (float val, bool /*force*/)
    {
        eq.setBandResonance(0, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band0Gain,
    [this] (float val, bool /*force*/)
    {
        eq.setBandGain(0, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band1Type,
    [this] (float val, bool /*force*/)
    {
        eq.setBandType(1, static_cast<mrta::ParametricEqualizer::FilterType>(std::round(val)));
    });

    parameterManager.registerParameterCallback(Param::ID::Band1Freq,
    [this] (float val, bool /*force*/)
    {
        eq.setBandFrequency(1, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band1Reso,
    [this] (float val, bool /*force*/)
    {
        eq.setBandResonance(1, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band1Gain,
    [this] (float val, bool /*force*/)
    {
        eq.setBandGain(1, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band2Type,
    [this] (float val, bool /*force*/)
    {
        eq.setBandType(2, static_cast<mrta::ParametricEqualizer::FilterType>(std::round(val)));
    });

    parameterManager.registerParameterCallback(Param::ID::Band2Freq,
    [this] (float val, bool /*force*/)
    {
        eq.setBandFrequency(2, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band2Reso,
    [this] (float val, bool /*force*/)
    {
        eq.setBandResonance(2, val);
    });

    parameterManager.registerParameterCallback(Param::ID::Band2Gain,
    [this] (float val, bool /*force*/)
    {
        eq.setBandGain(2, val);
    });
}

ParametricEQAudioProcessor::~ParametricEQAudioProcessor()
{
}


void ParametricEQAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    unsigned int maxNumChannels = std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels());
    eq.prepare(sampleRate, maxNumChannels);
    parameterManager.updateParameters(true);
}

void ParametricEQAudioProcessor::releaseResources()
{
    eq.clear();
}

void ParametricEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();

    eq.process(buffer.getArrayOfWritePointers(), buffer.getArrayOfReadPointers(), buffer.getNumChannels(), buffer.getNumSamples());
}

void ParametricEQAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void ParametricEQAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

//==============================================================================
const juce::String ParametricEQAudioProcessor::getName() const { return JucePlugin_Name; }
bool ParametricEQAudioProcessor::acceptsMidi() const { return false; }
bool ParametricEQAudioProcessor::producesMidi() const { return false; }
bool ParametricEQAudioProcessor::isMidiEffect() const { return false; }
double ParametricEQAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int ParametricEQAudioProcessor::getNumPrograms() { return 1; }
int ParametricEQAudioProcessor::getCurrentProgram() { return 0; }
void ParametricEQAudioProcessor::setCurrentProgram(int) { }
const juce::String ParametricEQAudioProcessor::getProgramName(int) { return {}; }
void ParametricEQAudioProcessor::changeProgramName(int, const juce::String&) { }
bool ParametricEQAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* ParametricEQAudioProcessor::createEditor() { return new ParametricEQAudioProcessorEditor(*this); }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametricEQAudioProcessor();
}
