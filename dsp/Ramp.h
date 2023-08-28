#pragma once

#include <cmath>

namespace mrta
{

template<typename F>
class Ramp
{
public:
    // Default ramp time of 50ms
    static constexpr F DefaultRampTime { static_cast<F>(0.05) };

    Ramp(F rampTimeSec) :
        rampTime { std::fmax(rampTimeSec, minRampTime) }
    { }

    ~Ramp() { }

    // No default ctor
    Ramp() :
        rampTime { DefaultRampTime }
    { }

    // No copy semantics
    Ramp(const Ramp&) = delete;
    const Ramp& operator=(const Ramp&) = delete;

    // No move semantics
    Ramp(Ramp&&) = delete;
    const Ramp& operator=(Ramp&&) = delete;

    // Update sample rate of the ramp time, optionally allowing to
    // skip to ramp value
    void prepare(double newSampleRate, bool skipRamp = false, F skipRampToValue = static_cast<F>(0))
    {
        sampleRate = newSampleRate;
        if (skipRamp)
            setTarget(skipRampToValue, true);
        else
            setTarget(targetValue);
    }

    // Set the target value to the ramp
    // optionally allowing to skip the ramp
    void setTarget(F newTargetValue, bool skipRamp = false)
    {
        if (std::abs(newTargetValue - currentValue) > minDelta)
        {
            targetValue = newTargetValue;
            rampStep = (targetValue - currentValue) / static_cast<F>(sampleRate * rampTime);
        }

        if (skipRamp)
            currentValue = targetValue = newTargetValue;
    }

    // Apply summing ramp to a single sample in-place
    void applySum(F* buffers, unsigned int numChannels)
    {
        const F targetDelta { std::fabs(targetValue - currentValue) };
        if ((targetDelta > std::fabs(static_cast<F>(2) * rampStep)) && (std::fabs(rampStep) > minDelta))
            currentValue += rampStep;
        else
            currentValue = targetValue;

        for (unsigned int ch = 0; ch < numChannels; ++ch)
            buffers[ch] += currentValue;
    }

    // Apply summing ramp to an audio buffer in-place
    void applySum(F* const* buffers, unsigned int numChannels, unsigned int numSamples)
    {
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            const F targetDelta { std::fabs(targetValue - currentValue) };
            if ((targetDelta > std::fabs(static_cast<F>(2) * rampStep)) && (std::fabs(rampStep) > minDelta))
                currentValue += rampStep;
            else
                currentValue = targetValue;

            for (unsigned int ch = 0; ch < numChannels; ++ch)
                buffers[ch][n] += currentValue;
        }
    }

    // Apply summing ramp to an audio buffer out-of-place
    void applySum(F* const* output, const F* const* input, unsigned int numChannels, unsigned int numSamples)
    {
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            const F targetDelta { std::fabs(targetValue - currentValue) };
            if ((targetDelta > std::fabs(static_cast<F>(2) * rampStep)) && (std::fabs(rampStep) > minDelta))
                currentValue += rampStep;
            else
                currentValue = targetValue;

            for (unsigned int ch = 0; ch < numChannels; ++ch)
                output[ch][n] = currentValue + input[ch][n];
        }
    }

    // Apply gain ramp to an audio buffer in-place for single sample
    void applyGain(F* buffers, unsigned int numChannels)
    {
        const F targetDelta { std::fabs(targetValue - currentValue) };
        if ((targetDelta > std::fabs(static_cast<F>(2) * rampStep)) && (std::fabs(rampStep) > minDelta))
            currentValue += rampStep;
        else
            currentValue = targetValue;

        for (unsigned int ch = 0; ch < numChannels; ++ch)
            buffers[ch] *= currentValue;
    }

    // Apply gain ramp to an audio buffer in-place
    void applyGain(F* const* buffers, unsigned int numChannels, unsigned int numSamples)
    {
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            const F targetDelta { std::fabs(targetValue - currentValue) };
            if ((targetDelta > std::fabs(static_cast<F>(2) * rampStep)) && (std::fabs(rampStep) > minDelta))
                currentValue += rampStep;
            else
                currentValue = targetValue;

            for (unsigned int ch = 0; ch < numChannels; ++ch)
                buffers[ch][n] *= currentValue;
        }
    }

    // Apply gain ramp to an audio buffer out-of-place
    void applyGain(F* const* output, const F* const* input, unsigned int numChannels, unsigned int numSamples)
    {
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            const F targetDelta{ std::fabs(targetValue - currentValue) };
            if ((targetDelta > std::fabs(static_cast<F>(2) * rampStep)) && (std::fabs(rampStep) > minDelta))
                currentValue += rampStep;
            else
                currentValue = targetValue;

            for (unsigned int ch = 0; ch < numChannels; ++ch)
                output[ch][n] = currentValue * input[ch][n];
        }
    }

    // Minimum ramp time in secondes
    static constexpr F minRampTime { static_cast<F>(1e-3) }; // 1ms

    // Minimun absolute differente between target and current value
    static constexpr F minDelta { static_cast<F>(1e-9) };

private:
    double sampleRate { 48000.0 };
    F rampTime;
    F rampStep { static_cast<F>(0) };
    F targetValue { static_cast<F>(0) };
    F currentValue { static_cast<F>(0) };
};

}
