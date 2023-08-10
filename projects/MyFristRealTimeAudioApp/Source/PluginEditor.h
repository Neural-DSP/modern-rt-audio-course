#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MainProcessorEditor : public juce::AudioProcessorEditor
{
public:
    MainProcessorEditor(MainProcessor&);
    ~MainProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MainProcessor& audioProcessor;
    mrta::GenericParameterEditor genericParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainProcessorEditor)
};
