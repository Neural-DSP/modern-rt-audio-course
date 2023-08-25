#pragma once

#include "Biquad.h"

namespace mrta
{

class ParametricEqualizer
{
public:
    enum FilterType : unsigned int
    {
        Flat = 0,
        HighPass,
        LowShelf,
        Peak,
        LowPass,
        HighShelf
    };

    // Main ctor
    // Requires number of bands and channels to be allocated
    // The number of bands cannot be modified later but channels can be reallocated
    // All bands filters will be initialised to Flat
    ParametricEqualizer(unsigned int numOfBands, unsigned int maxNumChannels = 2);

    // Dtor
    ~ParametricEqualizer();

    // No default ctor
    ParametricEqualizer() = delete;

    // No copy sematics
    ParametricEqualizer(const ParametricEqualizer&) = delete;
    const ParametricEqualizer& operator=(const ParametricEqualizer&) = delete;

    // No move semantics
    ParametricEqualizer(ParametricEqualizer&&) = delete;
    const ParametricEqualizer& operator=(ParametricEqualizer&&) = delete;

    // Clear states
    void clear();

    // Clear states, recalculate coeffs to new sample rate and reallocate channels
    void prepare(double sampleRate, unsigned int maxNumChannels);

    // Process audio buffers
    // This method can be called with a lower number of channels than allocated
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

    // Set filter type of a band
    void setBandType(unsigned int band, FilterType type);

    // Set filter frequency of a band in Hz
    void setBandFrequency(unsigned int band, float frequency);

    // Set filter resonance of a band in Q factor
    void setBandResonance(unsigned int band, float resonance);

    // Set filter gain of a band in dB
    void setBandGain(unsigned int band, float gain);

private:
    // Biquad structure for filter realization
    mrta::Biquad biquad;

    // Current sample rate of coefficients
    double sampleRate { 48000.0 };

    // Structure to hold band filter information
    struct Band
    {
        FilterType type { Flat };
        float freq { 1000.f };
        float reso { 0.7071f };
        float gain { 0.f };
    };

    // All bands information
    std::vector<Band> bands;

    // Helper function to calculate coefficients
    std::array<float, mrta::Biquad::CoeffsPerSection> calculateCoeffs(const Band & band);
};

}
