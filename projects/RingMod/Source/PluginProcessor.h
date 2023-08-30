#pragma once

#include <JuceHeader.h>

#include "RingMod.h"

namespace Param
{
    namespace ID
    {
        static const juce::String ModRate { "mod_rate" };
        static const juce::String ModDepth { "mod_depth" };
        static const juce::String ModType { "mod_type" };
    }

    namespace Name
    {
        static const juce::String ModRate { "Mod. Rate" };
        static const juce::String ModDepth { "Mod. Depth" };
        static const juce::String ModType { "Mod. Type" };
    }

    namespace Unit
    {
        static const juce::String Hz { "Hz" };
        static const juce::String Pct { "%" };
    }

    namespace Range
    {
        static constexpr float ModRateMin { 1.f };
        static constexpr float ModRateMax { 1000.f };
        static constexpr float ModRateInc { 0.1f };
        static constexpr float ModRateSkw { 0.35f };

        static constexpr float ModDepthMin { 0.f };
        static constexpr float ModDepthMax { 100.f };
        static constexpr float ModDepthInc { 0.1f };
        static constexpr float ModDepthSkw { 1.f };

        static const juce::StringArray ModTypeLabels { "Sin", "Bal. Sin", "Rect. Sin", "Tri" };
    }
}

class RingModAudioProcessor : public juce::AudioProcessor
{
public:
    RingModAudioProcessor();
    ~RingModAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

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
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override;
    //==============================================================================

private:
    mrta::ParameterManager parameterManager;
    mrta::RingMod ringMod;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingModAudioProcessor)
};
