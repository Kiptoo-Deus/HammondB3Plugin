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

    std::array<juce::Slider, 9> drawbarSliders;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 9> drawbarAttachments;

    juce::ComboBox vibratoChorusSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoChorusAttachment;

    juce::ComboBox leslieSpeedSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> leslieSpeedAttachment;

    juce::ToggleButton percussionToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> percussionAttachment;

    juce::ComboBox percussionHarmonicSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> percussionHarmonicAttachment;

    juce::ComboBox percussionDynamicsSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> percussionDynamicsAttachment;

    juce::ComboBox percussionDecaySelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> percussionDecayAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HammondB3PluginAudioProcessorEditor)
};
