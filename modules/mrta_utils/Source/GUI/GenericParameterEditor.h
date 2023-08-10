#pragma once

namespace mrta
{

class GenericParameterEditor : public juce::Component
{
public:
    GenericParameterEditor(mrta::ParameterManager& parameterManager,
                           int parameterWidgetHeight = 80,
                           const juce::StringArray& parameterIDs = {});
    GenericParameterEditor() = delete;

    void paint(juce::Graphics&) override;
    void resized() override;

    const int parameterWidgetHeight;

private:
    juce::StringArray parameterIDs;
    std::vector<std::unique_ptr<juce::Component>> parameterComponents;
    std::vector<std::unique_ptr<juce::Label>> parameterLabels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GenericParameterEditor)
};

}
