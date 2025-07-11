/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HammondB3PluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HammondB3PluginAudioProcessorEditor (HammondB3PluginAudioProcessor&);
    ~HammondB3PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HammondB3PluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HammondB3PluginAudioProcessorEditor)
};
