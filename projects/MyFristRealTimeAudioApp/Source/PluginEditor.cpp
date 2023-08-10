#include "PluginProcessor.h"
#include "PluginEditor.h"

MainProcessorEditor::MainProcessorEditor(MainProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p),
    genericParameterEditor(audioProcessor.getParameterManager())
{
    int height = static_cast<int>(audioProcessor.getParameterManager().getParameters().size())
               * genericParameterEditor.parameterWidgetHeight;
    setSize(300, height);
    addAndMakeVisible(genericParameterEditor);
}

MainProcessorEditor::~MainProcessorEditor()
{
}

void MainProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainProcessorEditor::resized()
{
    genericParameterEditor.setBounds(getLocalBounds());
}
