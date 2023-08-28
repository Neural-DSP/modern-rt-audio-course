#pragma once

namespace mrta
{

class ParameterManager : public juce::AudioProcessorValueTreeState::Listener
{
public:
    // Callback function type alias
    using Callback = std::function<void(float value, bool forced)>;

    // Main ctor
    ParameterManager(juce::AudioProcessor& audioProcessor,
                     const juce::String& identifier,
                     const std::vector<mrta::ParameterInfo>& parameters);

    // No default ctor
    ParameterManager() = delete;

    // Dtor
    ~ParameterManager();

    // Register a callback lambda function for a parameter ID
    // This method should prefereably be used on the processors ctor body
    // to avoid missing parameter events
    bool registerParameterCallback(const juce::String& ID, Callback cb);

    // Checks if there are parameter change events on the queue
    // and call the respective callbacks for them
    // This method is supposed to be calle on every process buffer
    // before the audio processing
    // The optional 'force' argutment will call flush
    // all the current parameter values to the callback
    // regardless if there are events on the queue, this is a
    // good way to guarantee the DSP has updated parameters
    void updateParameters(bool force = false);

    // Empty the paramter event queue
    void clearParameterQueue();

    // Get a vector with all the parameters information structs
    const std::vector<mrta::ParameterInfo>& getParameters() const;

    // Get underlying JUCE APVTS, can be useful for
    // using widgets to track parameters
    juce::AudioProcessorValueTreeState& getAPVTS();

    // Helper functions for serialising the current parameter
    // state, has the same signature as juce::AudioProcessor
    // uses for parameter state load and save
    void setStateInformation(const void* data, int sizeInBytes);

    // Helper functions for serialising the current parameter
    // state, has the same signature as juce::AudioProcessor
    // uses for parameter state load and save
    void getStateInformation(juce::MemoryBlock& destData);

    // Parameter change listener, this function is not meant to
    // be called by user, it is used by the APVTS listerners only
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

