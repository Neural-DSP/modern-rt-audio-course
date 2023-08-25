#include "ParametricEqualizer.h"

#include <cmath>

// Windows does not have Pi constants
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

namespace mrta
{

ParametricEqualizer::ParametricEqualizer(unsigned int numOfBands, unsigned int maxNumChannels) :
    biquad(numOfBands, maxNumChannels),
    bands(numOfBands)
{
    unsigned int b { 0 };
    for (const auto& band : bands)
        biquad.setSectionCoeffs(calculateCoeffs(band), b++);
}

ParametricEqualizer::~ParametricEqualizer()
{
}

void ParametricEqualizer::clear()
{
    biquad.clear();
}

void ParametricEqualizer::prepare(double newSampleRate, unsigned int maxNumChannels)
{
    biquad.reallocateChannels(maxNumChannels);

    sampleRate = newSampleRate;

    unsigned int b { 0 };
    for (const auto& band : bands)
        biquad.setSectionCoeffs(calculateCoeffs(band), b++);
}

void ParametricEqualizer::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    biquad.process(output, input, numChannels, numSamples);
}

void ParametricEqualizer::setBandType(unsigned int band, FilterType type)
{
    if (band < bands.size() && band < biquad.getAllocatedSections())
    {
        bands[band].type = type;
        biquad.setSectionCoeffs(calculateCoeffs(bands[band]), band);
    }
}

void ParametricEqualizer::setBandFrequency(unsigned int band, float frequency)
{
    if (band < bands.size() && band < biquad.getAllocatedSections())
    {
        bands[band].freq = std::fmax(frequency, 2.f);
        biquad.setSectionCoeffs(calculateCoeffs(bands[band]), band);
    }
}

void ParametricEqualizer::setBandResonance(unsigned int band, float resonance)
{
    if (band < bands.size() && band < biquad.getAllocatedSections())
    {
        bands[band].reso = std::fmax(resonance, 0.1f);
        biquad.setSectionCoeffs(calculateCoeffs(bands[band]), band);
    }
}

void ParametricEqualizer::setBandGain(unsigned int band, float gain)
{
    if (band < bands.size() && band < biquad.getAllocatedSections())
    {
        bands[band].gain = gain;
        biquad.setSectionCoeffs(calculateCoeffs(bands[band]), band);
    }
}

std::array<float, mrta::Biquad::CoeffsPerSection> ParametricEqualizer::calculateCoeffs(const Band& band)
{
    // Flat coeffs
    std::array<float, mrta::Biquad::CoeffsPerSection> coeffs { 1.f, 0.f, 0.f, 0.f, 0.f };

    switch (band.type)
    {
        case HighPass:
        {
            float n = std::tan(static_cast<float>(M_PI) * band.freq / static_cast<float>(sampleRate));
            float nSquared = n * n;
            float invQ = 1.f / band.reso;
            float c1 = 1.f / (1.f + invQ * n + nSquared);

            coeffs = { c1, c1 * -2.f, c1, c1 * 2.f * (nSquared - 1.f), c1 * (1.f - invQ * n + nSquared) };
        }
        break;

        case LowShelf:
        {
            float A = std::pow(10.f, band.gain * 0.05f);
            float aminus1 = A - 1.f;
            float aplus1 = A + 1.f;
            float omega = (2.f * static_cast<float>(M_PI) * band.freq) / static_cast<float>(sampleRate);
            float coso = std::cos(omega);
            float beta = std::sin(omega) * std::sqrt(A) / band.reso;
            float aminus1TimesCoso = aminus1 * coso;

            float a0 = 1.f / (aplus1 + aminus1TimesCoso + beta);
            coeffs = { A * (aplus1 - aminus1TimesCoso + beta) * a0,
                       A * 2.f * (aminus1 - aplus1 * coso) * a0,
                       A * (aplus1 - aminus1TimesCoso - beta) * a0,
                       -2.f * (aminus1 + aplus1 * coso) * a0,
                       (aplus1 + aminus1TimesCoso - beta) * a0 };
        }
        break;

        case Peak:
        {
            float A = std::pow(10.f, band.gain * 0.05f);
            float omega = (2.f * static_cast<float>(M_PI) * band.freq) / static_cast<float>(sampleRate);
            float alpha = std::sin(omega) / (band.reso * 2.f);
            float c2 = -2.f * std::cos(omega);
            float alphaTimesA = alpha * A;
            float alphaOverA = alpha / A;

            float a0 = 1.f / (1.f + alphaOverA);
            coeffs = { (1.f + alphaTimesA) * a0, c2 * a0, (1.f - alphaTimesA) * a0, c2 * a0, (1.f - alphaOverA) * a0 };
        }
        break;

        case LowPass:
        {
            float n = 1.f / std::tan(static_cast<float>(M_PI) * band.freq / static_cast<float>(sampleRate));
            float nSquared = n * n;
            float invQ = 1.f / band.reso;
            float c1 = 1.f / (1.f + invQ * n + nSquared);

            coeffs = { c1, c1 * 2.f, c1, c1 * 2.f * (1.f - nSquared), c1 * (1.f - invQ * n + nSquared) };
        }
        break;

        case HighShelf:
        {
            float A = std::pow(10.f, band.gain * 0.05f);
            float aminus1 = A - 1.f;
            float aplus1 = A + 1.f;
            float omega = (2.f * static_cast<float>(M_PI) * band.freq) / static_cast<float>(sampleRate);
            float coso = std::cos(omega);
            float beta = std::sin(omega) * std::sqrt(A) / band.reso;
            float aminus1TimesCoso = aminus1 * coso;

            float a0 = 1.f / (aplus1 - aminus1TimesCoso + beta);
            coeffs = { A * (aplus1 + aminus1TimesCoso + beta) * a0,
                       A * -2.f * (aminus1 + aplus1 * coso) * a0,
                       A * (aplus1 + aminus1TimesCoso - beta) * a0,
                       2.f * (aminus1 - aplus1 * coso) * a0,
                       (aplus1 - aminus1TimesCoso - beta) * a0 };
        }
        break;
    }

    return coeffs;
}

}
