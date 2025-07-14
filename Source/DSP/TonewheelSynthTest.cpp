/*
  ==============================================================================

    TonewheelSynthTest.cpp
    Created: 11 Jul 2025 11:30:38am
    Author:  Joel

  ==============================================================================
*/

#include <catch.hpp>
#include <JuceHeader.h>
#include "TonewheelSynth.h"

TEST_CASE("TonewheelSynth Tests", "[TonewheelSynth]")
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for (int i = 0; i < 9; ++i)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            "drawbar" + juce::String(i), "Drawbar " + juce::String(i + 1), 0.0f, 8.0f, 8.0f));
    juce::AudioProcessorValueTreeState parameters(nullptr, nullptr, juce::Identifier("Test"), std::move(layout));
    TonewheelSynth synth(parameters);

    SECTION("Frequency Accuracy")
    {
        synth.prepare(44100.0, 512);
        REQUIRE(synth.getBaseTonewheelIndex(36) == 0); // C2 maps to tonewheel 0
        REQUIRE(synth.getBaseTonewheelIndex(48) == 12); // C3 maps to tonewheel 12
    }

    SECTION("Drawbar Scaling")
    {
        juce::AudioBuffer<float> buffer(2, 512);
        juce::MidiBuffer midi;
        synth.prepare(44100.0, 512);
        parameters.getParameter("drawbar0")->setValueNotifyingHost(0.0f); // Mute 16'
        synth.process(buffer, midi);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            REQUIRE(buffer.getSample(0, sample) == 0.0f); // No output when drawbars off
    }
}
