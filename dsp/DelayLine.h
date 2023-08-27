#pragma once

#include <vector>

namespace mrta
{

class DelayLine
{
public:
    DelayLine(unsigned int maxLengthSamples, unsigned int numChannels);
    ~DelayLine();

    // No default ctor
    DelayLine() = delete;

    // No copy semantics
    DelayLine(const DelayLine&) = delete;
    const DelayLine& operator=(const DelayLine&) = delete;

    // No move semantics
    DelayLine(DelayLine&&) = delete;
    const DelayLine& operator=(DelayLine&&) = delete;

    // Clear the contents of the delay buffer
    void clear();

    // Reallocate delay buffer for the new channel count and clear its contents
    void prepare(unsigned int maxLengthSamples, unsigned int numChannels);

    // Process audio with the currently (fixed) set delay time
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

    // Single sample flavour of the fixed delay time processing
    void process(float* output, const float* input, unsigned int numChannels);

    // Process audio thru the delay line with audio rate modulation
    // The modulation input is a audio rate signal with the time modulation in samples
    // on top of the currently set delay time
    // The modulation input supports fractional values and uses linear interpolation
    void process(float* const* audioOutput, const float* const* audioInput, const float* const* modInput,
                 unsigned int numChannels, unsigned int numSamples);

    // Single sample flavour of the modulated delay time processing
    void process(float* audioOutput, const float* audioInput, const float* modInput, unsigned int numChannels);

    // Set the current delay time in samples
    void setDelaySamples(unsigned int samples);

private:
    std::vector<std::vector<float>> delayBuffer;
    unsigned int delaySamples { 0 };
    unsigned int writeIndex { 0 };
};

}
