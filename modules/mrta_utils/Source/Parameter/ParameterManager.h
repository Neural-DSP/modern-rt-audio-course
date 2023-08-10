#pragma once

namespace mrta
{

class ParameterManager : public juce::AudioProcessorValueTreeState::Listener
{
public:
    using Callback = std::function<void(float value, bool forced)>;

    ParameterManager(juce::AudioProcessor& audioProcessor,
                     const juce::String& identifier,
                     const std::vector<mrta::ParameterInfo>& parameters);
    ParameterManager() = delete;

    ~ParameterManager();

    bool registerParameterCallback(const juce::String& ID, Callback cb);

    void updateParameters(bool force = false);

    void clearParameterQueue();

    const std::vector<mrta::ParameterInfo>& getParameters() const;

    juce::AudioProcessorValueTreeState& getAPVTS();

    void setStateInformation(const void* data, int sizeInBytes);

    void getStateInformation(juce::MemoryBlock& destData);

    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    juce::AudioProcessorValueTreeState apvts;
    std::vector<mrta::ParameterInfo> parameters;
    mrta::ParameterFIFO<64> fifo;
    std::unordered_map<juce::String, Callback> callbacks;

    JUCE_DECLARE_NON_COPYABLE(ParameterManager)
    JUCE_DECLARE_NON_MOVEABLE(ParameterManager)
    JUCE_LEAK_DETECTOR(ParameterManager)
};

}

