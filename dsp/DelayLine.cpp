#include "DelayLine.h"

#include <algorithm>

namespace mrta
{

DelayLine::DelayLine(unsigned int maxLengthSamples, unsigned int numChannels)
{
    for (unsigned int ch = 0; ch < numChannels; ++ch)
        delayBuffer.emplace_back(maxLengthSamples, 0.f);
}

DelayLine::~DelayLine()
{
}

void DelayLine::clear()
{
    for (auto& b : delayBuffer)
        std::fill(b.begin(), b.end(), 0.f);
}

void DelayLine::prepare(unsigned int maxLengthSamples, unsigned int numChannels)
{
    delayBuffer.clear();
    for (unsigned int ch = 0; ch < numChannels; ++ch)
        delayBuffer.emplace_back(maxLengthSamples, 0.f);
}

void DelayLine::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        unsigned int workingWriteIndex { writeIndex };
        for (unsigned int n = 0; n < numSamples; ++n)
        {
            const unsigned int readIndex { workingWriteIndex + static_cast<unsigned int>(delayBuffer[0].size()) - delaySamples };
            output[ch][n] = delayBuffer[ch][readIndex];
            ++workingWriteIndex;
        }
    }
}

void DelayLine::process(float* const* audioOutput, const float* const* audioInput, float* const* modInput, unsigned int numChannels, unsigned int numSamples)
{
}

void DelayLine::setDelaySamples(unsigned int newDelaySamples)
{
    delaySamples = std::max(std::min(newDelaySamples, static_cast<unsigned int>(delayBuffer[0].size() - 1u)), 1u);
}


}
