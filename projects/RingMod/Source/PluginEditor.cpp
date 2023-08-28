#include "PluginProcessor.h"
#include "PluginEditor.h"

RingModAudioProcessorEditor::RingModAudioProcessorEditor(RingModAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p),
    genericParameterEditor(audioProcessor.getParameterManager())
{
    addAndMakeVisible(genericParameterEditor);
    const int numOfParams { static_cast<int>(audioProcessor.getParameterManager().getParameters().size()) };
    setSize(300, numOfParams * genericParameterEditor.parameterWidgetHeight);
}

RingModAudioProcessorEditor::~RingModAudioProcessorEditor()
{
}

//==============================================================================
void RingModAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void RingModAudioProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}
