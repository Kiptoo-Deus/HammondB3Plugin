#include "PluginEditor.h"

HammondB3PluginAudioProcessorEditor::HammondB3PluginAudioProcessorEditor(HammondB3PluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Drawbar Sliders
    for (int i = 0; i < 9; ++i)
    {
        drawbarSliders[i].setSliderStyle(juce::Slider::LinearVertical);
        drawbarSliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
        addAndMakeVisible(drawbarSliders[i]);
        drawbarAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.getParameters(), "drawbar" + juce::String(i), drawbarSliders[i]);
    }

    // Vibrato/Chorus Selector
    vibratoChorusSelector.addItemList({"Off", "V1", "V2", "V3", "C1", "C2", "C3"}, 1);
    addAndMakeVisible(vibratoChorusSelector);
    vibratoChorusAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getParameters(), "vibratoChorus", vibratoChorusSelector);

    // Leslie Speed Selector
    leslieSpeedSelector.addItemList({"Stop", "Slow", "Fast"}, 1);
    addAndMakeVisible(leslieSpeedSelector);
    leslieSpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getParameters(), "leslieSpeed", leslieSpeedSelector);

    // Percussion Controls
    percussionToggle.setButtonText("Percussion");
    addAndMakeVisible(percussionToggle);
    percussionAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getParameters(), "percussionOn", percussionToggle);

    percussionHarmonicSelector.addItemList({"2nd", "3rd"}, 1);
    addAndMakeVisible(percussionHarmonicSelector);
    percussionHarmonicAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getParameters(), "percussionHarmonic", percussionHarmonicSelector);

    percussionDynamicsSelector.addItemList({"Normal", "Soft"}, 1);
    addAndMakeVisible(percussionDynamicsSelector);
    percussionDynamicsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getParameters(), "percussionDynamics", percussionDynamicsSelector);

    percussionDecaySelector.addItemList({"Fast", "Slow"}, 1);
    addAndMakeVisible(percussionDecaySelector);
    percussionDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getParameters(), "percussionDecay", percussionDecaySelector);

    setSize(800, 400);
}

HammondB3PluginAudioProcessorEditor::~HammondB3PluginAudioProcessorEditor() {}

void HammondB3PluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Hammond B3 Plugin", getLocalBounds().withHeight(40), juce::Justification::centred, 1);
}

void HammondB3PluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto topBar = area.removeFromTop(40); // For title

    // Drawbars
    auto drawbarArea = area.removeFromLeft(400);
    for (int i = 0; i < 9; ++i)
    {
        drawbarSliders[i].setBounds(drawbarArea.removeFromLeft(40).reduced(2));
    }

    // Controls
    auto controlArea = area.removeFromTop(100);
    vibratoChorusSelector.setBounds(controlArea.removeFromLeft(100).reduced(5));
    leslieSpeedSelector.setBounds(controlArea.removeFromLeft(100).reduced(5));
    percussionToggle.setBounds(controlArea.removeFromLeft(100).reduced(5));
    percussionHarmonicSelector.setBounds(controlArea.removeFromLeft(100).reduced(5));
    percussionDynamicsSelector.setBounds(controlArea.removeFromLeft(100).reduced(5));
    percussionDecaySelector.setBounds(controlArea.removeFromLeft(100).reduced(5));
}
