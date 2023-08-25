#include "Biquad.h"

#include <algorithm>

namespace mrta
{

Biquad::Biquad(unsigned int maxNumSections, unsigned int maxNumChannels) :
    allocatedChannels { maxNumChannels },
    allocatedSections { maxNumSections },
    coeffs(allocatedSections * CoeffsPerSection, 0.f),
    states(allocatedChannels * allocatedSections * StatesPerSection, 0.f)
{
}

Biquad::Biquad()
{
}

Biquad::~Biquad()
{
}

void Biquad::clear()
{
    std::fill(states.begin(), states.end(), 0.f);
}

void Biquad::reallocateChannels(unsigned int maxNumChannels)
{
    allocatedChannels = maxNumChannels;
    states.resize(allocatedChannels * allocatedSections * StatesPerSection);
    std::fill(states.begin(), states.end(), 0.f);
}

void Biquad::reallocateSections(unsigned int numSections)
{
    allocatedSections = numSections;
    coeffs.resize(allocatedSections * CoeffsPerSection);
    states.resize(allocatedChannels * allocatedSections * StatesPerSection);
    std::fill(coeffs.begin(), coeffs.end(), 0.f);
    std::fill(states.begin(), states.end(), 0.f);
}

void Biquad::setSectionCoeffs(const std::array<float, CoeffsPerSection>& newSectionCoeffs, unsigned int section)
{
    if (section < allocatedSections)
        std::copy(newSectionCoeffs.begin(), newSectionCoeffs.end(), coeffs.begin() + (section * CoeffsPerSection));
}

void Biquad::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    numChannels = std::min(numChannels, allocatedChannels);
    for (unsigned int c = 0; c < numChannels; ++c)
    {
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            float x { input[c][n] };
            for (unsigned int s = 0; s < allocatedSections; ++s)
            {
                const unsigned int stateOffset { c * allocatedSections * StatesPerSection + s * StatesPerSection };
                const unsigned int coeffOffset { s * CoeffsPerSection };

                float acc { x * coeffs[coeffOffset + 0] }; // b0
                acc += coeffs[coeffOffset + 1] * states[stateOffset + 0]; // b1
                acc += coeffs[coeffOffset + 2] * states[stateOffset + 1]; // b2
                acc -= coeffs[coeffOffset + 3] * states[stateOffset + 2]; // a1
                acc -= coeffs[coeffOffset + 4] * states[stateOffset + 3]; // a2

                states[stateOffset + 1] = states[stateOffset + 0];
                states[stateOffset + 0] = x;
                states[stateOffset + 3] = states[stateOffset + 2];
                states[stateOffset + 2] = acc;
                x = acc;
            }
            output[c][n] = x;
        }
    }
}

}
