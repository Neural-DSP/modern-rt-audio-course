#pragma once

namespace mrta
{

class ParameterSlider : public juce::Slider
{
public:
    ParameterSlider(const juce::String& paramID, juce::AudioProcessorValueTreeState& apvts) :
        juce::Slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxRight),
        att(apvts, paramID, *this)
    {
        juce::AudioParameterFloat* param { dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(paramID)) };
        if (!param)
        {
            // Parameter type is not Float
            jassertfalse;
        }
    }

    ParameterSlider() = delete;

private:
    juce::AudioProcessorValueTreeState::SliderAttachment att;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSlider)
};

class ParameterComboBox : public juce::ComboBox
{
public:
    ParameterComboBox(const juce::String& paramID, juce::AudioProcessorValueTreeState& apvts) :
        att(apvts, paramID, *this)
    {
        juce::AudioParameterChoice* param { dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(paramID)) };
        if (!param)
        {
            // Parameter type is not Choice
            jassertfalse;
        }

        addItemList(param->choices, 1);
        setSelectedItemIndex(param->getIndex());
    }

    ParameterComboBox() = delete;

private:
    juce::AudioProcessorValueTreeState::ComboBoxAttachment att;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterComboBox)
};

class ParameterButton : public juce::TextButton
{
public:
    ParameterButton(const juce::String& paramID, juce::AudioProcessorValueTreeState& apvts) :
        att(apvts, paramID, *this)
    {
        juce::AudioParameterBool* param { dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(paramID)) };
        if (!param)
        {
            // Parameter type is not Bool
            jassertfalse;
        }

        juce::StringArray labels { param->getAllValueStrings() };

        setClickingTogglesState(true);
        setButtonText(labels[param->get() ? 1 : 0]);
        onStateChange = [labels, this]
        {
            setButtonText(labels[getToggleState() ? 1 : 0]);
        };
    }

    ParameterButton() = delete;

private:
    juce::AudioProcessorValueTreeState::ButtonAttachment att;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterButton)
};

}

