#include "RingMod.h"

// Windows does not have Pi constants
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace mrta
{

RingMod::RingMod()
{
}

void RingMod::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;

    depthRamp.prepare(sampleRate, true, modDepth);
    dryRamp.prepare(sampleRate, true, 1.f - modDepth);

    phaseState[0] = 0.f;
    phaseState[1] = static_cast<float>(M_PI / 2.0); // quadature osc between L and R channels
    phaseInc = static_cast<float>(2.0 * M_PI / sampleRate) * modRate;
}

void RingMod::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    for (unsigned int n = 0; n < numSamples; ++n)
    {
        // Process LFO acording to mod type
        float lfo[2] { 0.f, 0.f };
        switch (modType)
        {
        case RectSin:
            lfo[0] = std::fabs(std::sin(phaseState[0]));
            lfo[1] = std::fabs(std::sin(phaseState[1]));
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

        // Apply modulation depth gain ramp
        depthRamp.applyGain(lfo, numChannels);
        dryRamp.applySum(lfo, numChannels);

        // Do amplitude modulation
        for (unsigned int ch = 0; ch < numChannels; ++ch)
            output[ch][n] = lfo[ch] * input[ch][n];
    }
}

void RingMod::setModRate(float newModRate)
{
    modRate = std::fmax(newModRate, 0.f);
    phaseInc = static_cast<float>(2.0 * M_PI / sampleRate) * modRate;
}

void RingMod::setModDepth(float newModDepth)
{
    modDepth = std::fmin(std::fmax(newModDepth, 0.f), 1.f);
    depthRamp.setTarget(modDepth);
    dryRamp.setTarget(1.f - modDepth);
}

void RingMod::setModType(ModType type)
{
    modType = type;
}


}
