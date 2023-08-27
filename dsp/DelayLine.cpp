#include "DelayLine.h"

#include <algorithm>
#include <cmath>

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
    const unsigned int delayBufferSize { static_cast<unsigned int>(delayBuffer[0].size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        unsigned int workingWriteIndex { writeIndex };
        unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

        for (unsigned int n = 0; n < numSamples; ++n)
        {
            const float x { input[ch][n] };
            output[ch][n] = delayBuffer[ch][workingReadIndex];
            delayBuffer[ch][workingWriteIndex] = x;

            ++workingWriteIndex; workingWriteIndex %= delayBufferSize;
            ++workingReadIndex; workingReadIndex %= delayBufferSize;
        }
    }

    writeIndex += numSamples; writeIndex %= delayBufferSize;
}

void DelayLine::process(float* output, const float* input, unsigned int numChannels)
{
    const unsigned int delayBufferSize{ static_cast<unsigned int>(delayBuffer[0].size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        unsigned int workingWriteIndex { writeIndex };
        unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

        const float x { input[ch] };
        output[ch] = delayBuffer[ch][workingReadIndex];
        delayBuffer[ch][workingWriteIndex] = x;

        ++workingWriteIndex; workingWriteIndex %= delayBufferSize;
        ++workingReadIndex; workingReadIndex %= delayBufferSize;
    }

    ++writeIndex; writeIndex %= delayBufferSize;
}

void DelayLine::process(float* const* audioOutput, const float* const* audioInput, const float* const* modInput, unsigned int numChannels, unsigned int numSamples)
{
    const unsigned int delayBufferSize{ static_cast<unsigned int>(delayBuffer[0].size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        // Calculate base indices based on fixed delay time
        unsigned int workingWriteIndex { writeIndex };
        unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

        for (unsigned int n = 0; n < numSamples; ++n)
        {
            // Linear interpolation coefficients
            const float m { std::fmax(modInput[ch][n], 0.f) };
            const float mFloor { std::floor(m) };
            const float mFrac0 { m - mFloor };
            const float mFrac1 { 1.f - mFrac0 };

            // Calculate read indeces
            const unsigned int readIndex0 { (workingReadIndex + delayBufferSize - static_cast<unsigned int>(mFloor)) % delayBufferSize };
            const unsigned int readIndex1 { (readIndex0 + delayBufferSize - 1u) % delayBufferSize };

            // Read from delay line
            const float read0 = delayBuffer[ch][readIndex0];
            const float read1 = delayBuffer[ch][readIndex1];

            // Read audio input
            const float x { audioInput[ch][n] };

            // Interpolate output
            audioOutput[ch][n] = read0 * mFrac1 + read1 * mFrac0;

            // Write input
            delayBuffer[ch][workingWriteIndex] = x;

            // Increament indices
            ++workingWriteIndex; workingWriteIndex %= delayBufferSize;
            ++workingReadIndex; workingReadIndex %= delayBufferSize;
        }
    }

    // Update persistent write index
    writeIndex += numSamples; writeIndex %= delayBufferSize;
}

void DelayLine::process(float* audioOutput, const float* audioInput, const float* modInput, unsigned int numChannels)
{
    const unsigned int delayBufferSize{ static_cast<unsigned int>(delayBuffer[0].size()) };

    numChannels = std::min(numChannels, static_cast<unsigned int>(delayBuffer.size()));
    for (unsigned int ch = 0; ch < numChannels; ++ch)
    {
        // Calculate base indices based on fixed delay time
        unsigned int workingWriteIndex { writeIndex };
        unsigned int workingReadIndex { (workingWriteIndex + delayBufferSize - delaySamples) % delayBufferSize };

        // Linear interpolation coefficients
        const float m { std::fmax(modInput[ch], 0.f) };
        const float mFloor { std::floor(m) };
        const float mFrac0 { m - mFloor };
        const float mFrac1 { 1.f - mFrac0 };

        // Calculate read indeces
        const unsigned int readIndex0 { (workingReadIndex + delayBufferSize - static_cast<unsigned int>(mFloor)) % delayBufferSize };
        const unsigned int readIndex1 { (readIndex0 + delayBufferSize - 1u) % delayBufferSize };

        // Read from delay line
        const float read0 = delayBuffer[ch][readIndex0];
        const float read1 = delayBuffer[ch][readIndex1];

        // Read audio input
        const float x { audioInput[ch] };

        // Interpolate output
        audioOutput[ch] = read0 * mFrac1 + read1 * mFrac0;

        // Write input
        delayBuffer[ch][workingWriteIndex] = x;

        // Increament indices
        ++workingWriteIndex; workingWriteIndex %= delayBufferSize;
        ++workingReadIndex; workingReadIndex %= delayBufferSize;
    }

    // Update persistent write index
    ++writeIndex; writeIndex %= delayBufferSize;
}

void DelayLine::setDelaySamples(unsigned int newDelaySamples)
{
    delaySamples = std::max(std::min(newDelaySamples, static_cast<unsigned int>(delayBuffer[0].size() - 1u)), 1u);
}


}
