#include "PluginProcessor.h"
#include "PluginEditor.h"

HammondB3PluginAudioProcessorEditor::HammondB3PluginAudioProcessorEditor(HammondB3PluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

HammondB3PluginAudioProcessorEditor::~HammondB3PluginAudioProcessorEditor() {}

void HammondB3PluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hammond B3 Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void HammondB3PluginAudioProcessorEditor::resized()
{
    
}
