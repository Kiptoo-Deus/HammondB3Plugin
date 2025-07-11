#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class HammondB3PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    HammondB3PluginAudioProcessorEditor(HammondB3PluginAudioProcessor&);
    ~HammondB3PluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    HammondB3PluginAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HammondB3PluginAudioProcessorEditor)
};
