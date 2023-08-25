#pragma once

#include <JuceHeader.h>
#include "DelayLine.h"

namespace Param
{
    namespace ID
    {
        static const juce::String Enabled { "enabled" };
        static const juce::String Offset { "offset" };
        static const juce::String Depth { "depth" };
        static const juce::String Feedback { "feedback" };
        static const juce::String Rate { "rate" };
        static const juce::String ModType { "mod_type" };
    }

    namespace Name
    {
        static const juce::String Enabled { "Enabled" };
        static const juce::String Offset { "Offset" };
        static const juce::String Depth { "Depth" };
        static const juce::String Feedback { "Feedback" };
        static const juce::String Rate { "Rate" };
        static const juce::String ModType { "Mod. Type" };
    }

    namespace Ranges
    {
        static constexpr float OffsetMin { 1.f };
        static constexpr float OffsetMax { 10.f };
        static constexpr float OffsetInc { 0.1f };
        static constexpr float OffsetSkw { 0.5f };

        static constexpr float DepthMin { 0.f };
        static constexpr float DepthMax { 5.f };
        static constexpr float DepthInc { 0.1f };
        static constexpr float DepthSkw { 0.7f };

        static constexpr float FeedbackMin { 0.f };
        static constexpr float FeedbackMax { 100.f };
        static constexpr float FeedbackInc { 0.1f };
        static constexpr float FeedbackSkw { 0.7f };

        static constexpr float RateMin { 0.1f };
        static constexpr float RateMax { 5.f };
        static constexpr float RateInc { 0.1f };
        static constexpr float RateSkw { 0.5f };

        static const juce::StringArray ModLabels { "Sine", "Triangle", "Random" };

        static const juce::String EnabledOff { "Off" };
        static const juce::String EnabledOn { "On" };
    }

    namespace Units
    {
        static const juce::String Ms { "ms" };
        static const juce::String Hz { "Hz" };
        static const juce::String Pct { "%" };
    }
}

class FlangerAudioProcessor : public juce::AudioProcessor
{
public:
    FlangerAudioProcessor();
    ~FlangerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    mrta::ParameterManager& getParameterManager() { return parameterManager; }

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================

private:
    mrta::ParameterManager parameterManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerAudioProcessor)
};
