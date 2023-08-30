#include "PluginProcessor.h"
#include "PluginEditor.h"

ParametricEQAudioProcessorEditor::ParametricEQAudioProcessorEditor(ParametricEQAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p),
    band0ParameterEditor(audioProcessor.getParamterManager(), ParamHeight,
                         { Param::ID::Band0Enabled, Param::ID::Band0Type, Param::ID::Band0Freq, Param::ID::Band0Reso, Param::ID::Band0Gain }),
    band1ParameterEditor(audioProcessor.getParamterManager(), ParamHeight,
                         { Param::ID::Band1Enabled, Param::ID::Band1Type, Param::ID::Band1Freq, Param::ID::Band1Reso, Param::ID::Band1Gain }),
    band2ParameterEditor(audioProcessor.getParamterManager(), ParamHeight,
                         { Param::ID::Band2Enabled, Param::ID::Band2Type, Param::ID::Band2Freq, Param::ID::Band2Reso, Param::ID::Band2Gain })
{
    addAndMakeVisible(band0ParameterEditor);
    addAndMakeVisible(band1ParameterEditor);
    addAndMakeVisible(band2ParameterEditor);

    setSize(NumOfBands * BandWidth, ParamsPerBand * ParamHeight);
}

ParametricEQAudioProcessorEditor::~ParametricEQAudioProcessorEditor()
{
}

void ParametricEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ParametricEQAudioProcessorEditor::resized()
{
    auto localBounds { getLocalBounds() };
    band0ParameterEditor.setBounds(localBounds.removeFromLeft(BandWidth));
    band1ParameterEditor.setBounds(localBounds.removeFromLeft(BandWidth));
    band2ParameterEditor.setBounds(localBounds);
}
