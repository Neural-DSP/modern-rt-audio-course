#pragma once

#include <array>
#include <vector>

namespace mrta
{

class Biquad
{
public:
    Biquad(unsigned int numSections, unsigned int maxNumChannels);
    Biquad();
    ~Biquad();

    Biquad(const Biquad&);
    const Biquad& operator=(const Biquad&);

    Biquad(Biquad&&) = delete;
    const Biquad& operator=(Biquad&&) = delete;

    static const unsigned int CoeffsPerSection = 5;
    static const unsigned int StatesPerSection = 4;

    // Clear all states
    void clear();

    // Reallocate state storage
    // Calling this method will clear the states
    void reallocateChannels(unsigned int maxNumChannels);

    // Reallocate coefficient and state storage
    // Calling this method will clear the coefficients and states
    void reallocateSections(unsigned int numSections);

    // Set new coeffs to a section
    void setSectionCoeffs(const std::array<float, CoeffsPerSection>& newSectionCoeffs, unsigned int section);

    // Process audio
    // This method can be called with a lower number of channels than allocated
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

    // return the number of currently allocated channels
    unsigned int getAllocatedChannels() const noexcept { return allocatedChannels; }

    // return the number of currently allocated sections
    unsigned int getAllocatedSections() const noexcept { return allocatedSections; }

private:
    unsigned int allocatedChannels { 0 };
    unsigned int allocatedSections { 0 };

    // vector of coeffs of all sections
    // [sos0_b0, sos0_b1, sos0_b2, sos0_a1, sos0_a2, sos1_b0, sos1_b1, ...]
    std::vector<float> coeffs;

    // vector of states of all channels and sections
    // [ch0_sos0_bz1, ... , ch0_sos0_az2, ch0_sos1_bz1, ... , ch0_sos1_az2, ... ,
    //  ch1_sos0_bz1, ... , ch1_sos0_az2, ch1_sos1_bz1, ... , ch1_sos1_az2, ...]
    std::vector<float> states;
};

}
