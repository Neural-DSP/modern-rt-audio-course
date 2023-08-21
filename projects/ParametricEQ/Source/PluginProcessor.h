#pragma once

#include <JuceHeader.h>

#include "ParametricEqualizer.h"

namespace Param
{
    namespace ID
    {
        static const juce::String Band0Enabled { "band0_enabled" };
        static const juce::String Band0Type { "band0_type" };
        static const juce::String Band0Freq { "band0_freq" };
        static const juce::String Band0Reso { "band0_reso" };
        static const juce::String Band0Gain { "band0_gain" };

        static const juce::String Band1Enabled { "band1_enabled" };
        static const juce::String Band1Type { "band1_type" };
        static const juce::String Band1Freq { "band1_freq" };
        static const juce::String Band1Reso { "band1_reso" };
        static const juce::String Band1Gain { "band1_gain" };

        static const juce::String Band2Enabled { "band2_enabled" };
        static const juce::String Band2Type { "band2_type" };
        static const juce::String Band2Freq { "band2_freq" };
        static const juce::String Band2Reso { "band2_reso" };
        static const juce::String Band2Gain { "band2_gain" };
    }

    namespace Name
    {
        static const juce::String Band0Enabled { "B0 Enabled" };
        static const juce::String Band0Type { "B0 Type" };
        static const juce::String Band0Freq { "B0 Frequency" };
        static const juce::String Band0Reso { "B0 Resonance" };
        static const juce::String Band0Gain { "B0 Gain" };

        static const juce::String Band1Enabled { "B1 Enabled" };
        static const juce::String Band1Type { "B1 Type" };
        static const juce::String Band1Freq { "B1 Frequency" };
        static const juce::String Band1Reso { "B1 Resonance" };
        static const juce::String Band1Gain { "B1 Gain" };

        static const juce::String Band2Enabled { "B2 Enabled" };
        static const juce::String Band2Type { "B2 Type" };
        static const juce::String Band2Freq { "B2 Frequency" };
        static const juce::String Band2Reso { "B2 Resonance" };
        static const juce::String Band2Gain { "B2 Gain" };
    }

    namespace Ranges
    {
        static const float FreqMin { 20.f };
        static const float FreqMax { 20000.f };
        static const float FreqInc { 1.f };
        static const float FreqSkw { 0.3f };

        static const float ResoMin { 0.1f };
        static const float ResoMax { 10.f };
        static const float ResoInc { 0.01f };
        static const float ResoSkw { 0.5f };

        static const float GainMin { -24.f };
        static const float GainMax { 24.f };
        static const float GainInc { 0.1f };
        static const float GainSkw { 1.f };

        static const juce::StringArray Types { "Flat", "High Pass", "Low Shelf", "Peak", "Low Pass", "High Shelf" };

        static const juce::String EnabledOn { "On" };
        static const juce::String EnabledOff { "Off" };
    }

    namespace Unit
    {
        static const juce::String Freq { "Hz" };
        static const juce::String Gain { "dB" };
    }
}

class ParametricEQAudioProcessor : public juce::AudioProcessor
{
public:
    ParametricEQAudioProcessor();
    ~ParametricEQAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    mrta::ParameterManager& getParamterManager() { return parameterManager; }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override;
    //==============================================================================

private:
    mrta::ParameterManager parameterManager;
    mrta::ParametricEqualizer eq;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametricEQAudioProcessor)
};
