#include "Flanger.h"

#include <cmath>

// Windows does not have Pi constants
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace mrta
{

Flanger::Flanger(float maxTimeMs, unsigned int numChannels) :
    delayLine(static_cast<unsigned int>(std::ceil(std::fmin(maxTimeMs, 1.f) * sampleRate)), numChannels),
    offsetRamp(0.05f),
    modDepthRamp(0.05f),
    feedbackRamp(0.05f)
{
}

Flanger::~Flanger()
{
}

void Flanger::prepare(double newSampleRate, float maxTimeMs, unsigned int numChannels)
{
    sampleRate = newSampleRate;

    delayLine.prepare(static_cast<unsigned int>(std::round(maxTimeMs * static_cast<float>(0.001 * sampleRate))), numChannels);
    delayLine.setDelaySamples(static_cast<unsigned int>(std::ceil(0.001 * sampleRate))); // Set fixed delay to 1ms

    offsetRamp.prepare(sampleRate, true, offsetMs * static_cast<float>(0.001 * sampleRate));
    modDepthRamp.prepare(sampleRate, true, modDepthMs * static_cast<float>(0.001 * sampleRate));
    feedbackRamp.prepare(sampleRate, true, feedback);

    phaseState[0] = 0.f;
    phaseState[1] = static_cast<float>(M_PI / 2.0);
    phaseInc = static_cast<float>(2.0 * M_PI / sampleRate) * modRate;

    feedbackState[0] = 0.f;
    feedbackState[1] = 0.f;
}

void Flanger::clear()
{
    delayLine.clear();
}

void Flanger::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    for (unsigned int n = 0; n < numSamples; ++n)
    {
        // Process LFO acording to mod type
        float lfo[2] { 0.f, 0.f };
        switch (modType)
        {
        case Saw:
            lfo[0] = phaseState[0] * static_cast<float>(0.5 / M_PI);
            lfo[1] = phaseState[1] * static_cast<float>(0.5 / M_PI);
            break;

        case Tri:
            lfo[0] = std::fabs((phaseState[0] - static_cast<float>(M_PI)) / static_cast<float>(M_PI));
            lfo[1] = std::fabs((phaseState[1] - static_cast<float>(M_PI)) / static_cast<float>(M_PI));
            break;

        case Sin:
            lfo[0] = 0.5f + 0.5f * std::sin(phaseState[0]);
            lfo[1] = 0.5f + 0.5f * std::sin(phaseState[1]);
            break;
        }

        // Increment and wrap phase states
        phaseState[0] = std::fmod(phaseState[0] + phaseInc, static_cast<float>(2 * M_PI));
        phaseState[1] = std::fmod(phaseState[1] + phaseInc, static_cast<float>(2 * M_PI));

        // Apply mod depth and offset ramps
        modDepthRamp.applyGain(lfo, numChannels);
        offsetRamp.applySum(lfo, numChannels);

        // Processo feedback gain ramp
        feedbackRamp.applyGain(feedbackState, numChannels);

        // Read inputs
        float x[2] { 0.f, 0.f };
        for (unsigned int ch = 0; ch < numChannels; ++ch)
            x[ch] = input[ch][n] + feedbackState[ch];

        // Process delay
        delayLine.process(feedbackState, x, lfo, numChannels);

        // Write to output buffers
        for (unsigned int ch = 0; ch < numChannels; ++ch)
            output[ch][n] = feedbackState[ch];
    }
}

void Flanger::setOffset(float newOffsetMs)
{
    // Since the fixed delay is set to 1ms
    // We can deduct that from the offset ramp
    offsetMs = std::fmax(newOffsetMs - 1.f, 0.f);
    offsetRamp.setTarget(offsetMs * static_cast<float>(0.001 * sampleRate));
}

void Flanger::setDepth(float newDepthMs)
{
    modDepthMs = std::fmax(newDepthMs, 0.f);
    modDepthRamp.setTarget(modDepthMs * static_cast<float>(0.001 * sampleRate));
}

void Flanger::setFeedback(float newFeedback)
{
    feedback = std::fmin(std::fmax(newFeedback, -1.f), 1.f);
    feedbackRamp.setTarget(feedback);
}

void Flanger::setModulationRate(float newModRateHz)
{
    modRate = std::fmax(newModRateHz, 0.f);
    phaseInc = static_cast<float>(2.0 * M_PI / sampleRate) * modRate;
}

void Flanger::setModulationType(ModulationType newModType)
{
    modType = newModType;
}

}
