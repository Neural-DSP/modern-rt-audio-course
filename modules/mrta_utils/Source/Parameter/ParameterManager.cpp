namespace mrta
{

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(const std::vector<mrta::ParameterInfo>& infos)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for (const auto& info : infos)
    {
        switch (info.type)
        {
            case mrta::ParameterInfo::Float:
            {
                layout.add(std::make_unique<juce::AudioParameterFloat>(info.ID, info.name,
                                                                       juce::NormalisableRange<float>(info.min, info.max, info.inc, info.skw),
                                                                       info.def,
                                                                       juce::AudioParameterFloatAttributes().withLabel(info.unit)));
            }
            break;

            case mrta::ParameterInfo::Choice:
            {
                layout.add(std::make_unique<juce::AudioParameterChoice>(info.ID, info.name, info.steps, static_cast<int>(info.def)));
            }
            break;

            case mrta::ParameterInfo::Bool:
            {
                juce::String falseLabel { info.steps[0] };
                juce::String trueLabel { info.steps[1] };
                auto attr = juce::AudioParameterBoolAttributes().withStringFromValueFunction([falseLabel, trueLabel] (bool val, int)
                                                                                             {
                                                                                                 if (val) return trueLabel;
                                                                                                 else return falseLabel;
                                                                                             })
                                                                .withValueFromStringFunction([falseLabel, trueLabel] (const juce::String& text)
                                                                                             {
                                                                                                 if (falseLabel.compare(text) == 0) return false;
                                                                                                 if (trueLabel.compare(text) == 0) return true;
                                                                                                 return false;
                                                                                             });
                layout.add(std::make_unique<juce::AudioParameterBool>(info.ID, info.name, static_cast<bool>(info.def), attr));
            }
            break;

            default:
                break;
        }
    }
    return layout;
}

ParameterManager::ParameterManager(juce::AudioProcessor& audioProcessor, const juce::String& identifier, const std::vector<mrta::ParameterInfo>& _parameters) :
    apvts(audioProcessor, nullptr, identifier, createParameterLayout(_parameters)),
    parameters { _parameters }
{
}

ParameterManager::~ParameterManager()
{
    for (const auto& c : callbacks)
        apvts.removeParameterListener(c.first, this);
}

bool ParameterManager::registerParameterCallback(const juce::String& ID, Callback cb)
{
    if (ID.isNotEmpty() && cb)
    {
        if (callbacks.find(ID) == callbacks.end())
        {
            apvts.addParameterListener(ID, this);
            callbacks[ID] = cb;
            return true;
        }
    }
    return false;
}

void ParameterManager::updateParameters(bool force)
{
    if (force)
    {
        std::for_each(callbacks.begin(), callbacks.end(), [this] (auto& p)
        {
            if (auto* raw { apvts.getRawParameterValue(p.first) })
                p.second(raw->load(), true);
        });
        fifo.clear();
    }

    auto newParam = fifo.popParameter();
    while (newParam.first)
    {
        auto it = callbacks.find(newParam.second.first);
        if (it != callbacks.end())
            it->second(newParam.second.second, false);
        newParam = fifo.popParameter();
    }
}

void ParameterManager::clearParameterQueue()
{
    fifo.clear();
}

const std::vector<mrta::ParameterInfo>& ParameterManager::getParameters() const
{
    return parameters;
}

juce::AudioProcessorValueTreeState& ParameterManager::getAPVTS()
{
    return apvts;
}

void ParameterManager::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ValueTree newState { juce::ValueTree::readFromData(data, sizeInBytes) };
    apvts.replaceState(newState);
}

void ParameterManager::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, false);
    apvts.state.writeToStream(mos);
}

void ParameterManager::parameterChanged(const juce::String& parameterID, float newValue)
{
    fifo.pushParameter(parameterID, newValue);
}

}
