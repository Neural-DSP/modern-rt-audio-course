#pragma once

namespace mrta
{

struct ParameterInfo
{
    enum Type : uint32_t
    {
        Float = 0,
        Choice,
        Bool
    };

    // Float ctor
    ParameterInfo(const juce::String& _ID, const juce::String& _name, const juce::String& _unit,
                  float _def, float _min, float _max,
                  float _inc, float _skw) :
        ID { _ID }, name { _name }, unit { _unit }, steps { },
        type { Float },
        def { _def }, min { _min }, max { _max },
        inc { _inc }, skw { _skw }
    {
        jassert(ID.isNotEmpty());
        jassert(min < max);
        jassert(def <= max && def >= min);
        jassert(inc > 0.f);
        jassert(skw > 0.f);
    }

    // Choice ctor
    ParameterInfo( const juce::String& _ID, const juce::String& _name, const juce::StringArray& _steps, unsigned int _def) :
        ID { _ID }, name { _name }, unit { }, steps { _steps },
        type { Choice },
        def { static_cast<float>(_def) }, min { 0.f }, max { static_cast<float>(_steps.size() - 1) },
        inc { 1.f }, skw { 1.f }
    {
        jassert(ID.isNotEmpty());
        jassert(min < max);
        jassert(def <= max && def >= min);
        jassert(steps.size() == 0 || steps.size() == static_cast<unsigned int>(max + 1.f));
    }

    // Bool ctor
    ParameterInfo(const juce::String& _ID, const juce::String& _name,
                  const juce::String& offStepName, const juce::String& onStepName,
                  bool _def) :
        ID { _ID }, name { _name }, unit { }, steps { offStepName, onStepName },
        type { Bool },
        def { static_cast<float>(_def) }, min { 0.f }, max { 1.f },
        inc { 1.f }, skw { 1.f }
    {
        jassert(ID.isNotEmpty());
    }

    // Generic ctor
    ParameterInfo(const juce::String& _ID, const juce::String& _name, const juce::String& _unit, const juce::String& _steps,
                  Type _type,
                  float _def, float _min, float _max,
                  float _inc, float _skw) :
        ID { _ID }, name { _name }, unit { _unit }, steps { _steps },
        type { _type },
        def { _def }, min { _min }, max { _max },
        inc { _inc }, skw { _skw }
    {
        jassert(ID.isNotEmpty());
        jassert(min < max);
        jassert(def <= max && def >= min);
        jassert(inc > 0.f);
        jassert(skw > 0.f);
    }

    // Copy ctor
    ParameterInfo(const ParameterInfo& other) :
        ID { other.ID }, name { other.name }, unit { other.unit }, steps { other.steps },
        type { other.type },
        def { other.def }, min { other.min }, max { other.max },
        inc { other.inc }, skw { other.skw }
    { }

    // Move ctor
    ParameterInfo(ParameterInfo&& other) :
        ID { std::move(other.ID) }, name { std::move(other.name) },
        unit { std::move(other.unit) }, steps { std::move(other.steps) },
        type { other.type },
        def { other.def }, min { other.min }, max { other.max },
        inc { other.inc }, skw { other.skw }
    { }

    // No default ctor
    ParameterInfo() = delete;

    // No copy assign
    const ParameterInfo& operator=(const ParameterInfo&) = delete;

    // No move asign
    const ParameterInfo& operator=(ParameterInfo&) = delete;

    ~ParameterInfo() noexcept { }

    const juce::String ID;
    const juce::String name;
    const juce::String unit;
    const juce::StringArray steps;

    const Type type;

    const float def;
    const float min;
    const float max;
    const float inc;
    const float skw;
};

}
