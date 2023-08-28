#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class RingModAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    RingModAudioProcessorEditor(RingModAudioProcessor&);
    ~RingModAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RingModAudioProcessor& audioProcessor;
    mrta::GenericParameterEditor genericParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModAudioProcessorEditor)
};
