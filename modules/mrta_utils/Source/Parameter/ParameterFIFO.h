#pragma once

namespace mrta
{

template<size_t Capacity>
class ParameterFIFO
{
public:
    ParameterFIFO() :
        abstractFIFO { Capacity }
    {
        static_assert(Capacity > 0, "Capacity should be at least 1.");
    }

    void clear()
    {
        abstractFIFO.reset();
    }

    bool pushParameter(const juce::String& parameterID, float newValue)
    {
        if (abstractFIFO.getFreeSpace() == 0)
            return false;

        auto scope = abstractFIFO.write(1);

        if (scope.blockSize1 > 0)
            buffer[scope.startIndex1] = std::make_pair(parameterID, newValue);

        if (scope.blockSize2 > 0)
            buffer[scope.startIndex2] = std::make_pair(parameterID, newValue);

        return true;
    }

    std::optional<std::pair<juce::String, float>> popParameter()
    {
        if (abstractFIFO.getNumReady() == 0)
            return {};

        auto scope = abstractFIFO.read(1);

        if (scope.blockSize1 > 0)
            return buffer[scope.startIndex1];

        if (scope.blockSize2 > 0)
            return buffer[scope.startIndex2];

        return {};
    }

private:
    juce::AbstractFifo abstractFIFO;
    std::array<std::pair<juce::String, float>, Capacity> buffer;

    JUCE_DECLARE_NON_COPYABLE(ParameterFIFO)
    JUCE_DECLARE_NON_MOVEABLE(ParameterFIFO)
    JUCE_LEAK_DETECTOR(ParameterFIFO)
};

}
