/*
  ==============================================================================

    TonewheelSynth.cpp
    Created: 11 Jul 2025 11:19:14am
    Author:  Joel

  ==============================================================================
*/

#include "TonewheelSynth.h"

TonewheelSynth::TonewheelSynth()
{
    voices.resize(maxVoices);
}

void TonewheelSynth::setParameters(juce::AudioProcessorValueTreeState& params)
{
    parameters = &params;
}

void TonewheelSynth::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
    for (int i = 0; i < numTonewheels; ++i)
    {
        phaseIncrements[i] = tonewheelFrequencies[i] / sampleRate * juce::MathConstants<double>::twoPi;
    }

    vibratoDelay.prepare({sampleRate, (juce::uint32)samplesPerBlock, 2});
    vibratoDelay.setMaximumDelayInSamples(0.02 * sampleRate); // 20ms max delay

    leslieDelay.prepare({sampleRate, (juce::uint32)samplesPerBlock, 2});
    leslieDelay.setMaximumDelayInSamples(0.05 * sampleRate); // 50ms max delay for Leslie
}

void TonewheelSynth::releaseResources()
{
}

void TonewheelSynth::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            Voice* targetVoice = nullptr;
            double oldestTime = juce::Time::getMillisecondCounterHiRes();
            for (auto& voice : voices)
            {
                if (!voice.isActive)
                {
                    targetVoice = &voice;
                    break;
                }
                if (voice.startTime < oldestTime)
                {
                    oldestTime = voice.startTime;
                    targetVoice = &voice;
                }
            }
            if (targetVoice)
            {
                targetVoice->midiNote = msg.getNoteNumber();
                targetVoice->isActive = true;
                targetVoice->startTime = juce::Time::getMillisecondCounterHiRes();
                targetVoice->percussionGain = parameters->getRawParameterValue("percussionOn")->load() ? 1.0f : 0.0f;
                targetVoice->percussionPhase = 0.0f;
                targetVoice->keyClickPhase = 0.0f;
            }
        }
        else if (msg.isNoteOff())
        {
            for (auto& voice : voices)
            {
                if (voice.isActive && voice.midiNote == msg.getNoteNumber())
                {
                    voice.isActive = false;
                    voice.midiNote = -1;
                    break;
                }
            }
        }
        else if (msg.isController() && msg.getControllerNumber() >= 12 && msg.getControllerNumber() <= 20)
        {
            int drawbarIndex = msg.getControllerNumber() - 12;
            float value = msg.getControllerValue() / 127.0f * 8.0f;
            parameters->getParameter("drawbar" + juce::String(drawbarIndex))->setValueNotifyingHost(value / 8.0f);
        }
    }

    auto vibratoMode = parameters->getRawParameterValue("vibratoChorus")->load();
    auto leslieSpeed = parameters->getRawParameterValue("leslieSpeed")->load();
    auto percussionHarmonic = parameters->getRawParameterValue("percussionHarmonic")->load();
    auto percussionDynamics = parameters->getRawParameterValue("percussionDynamics")->load();
    auto percussionDecay = parameters->getRawParameterValue("percussionDecay")->load();

    juce::AudioBuffer<float> monoBuffer(1, buffer.getNumSamples());
    auto* monoWrite = monoBuffer.getWritePointer(0);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float output = 0.0f;

        for (auto& voice : voices)
        {
            if (!voice.isActive) continue;

            int baseTonewheel = getBaseTonewheelIndex(voice.midiNote);
            if (baseTonewheel < 0) continue;

            for (int i = 0; i < numDrawbars; ++i)
            {
                int tonewheelIndex = baseTonewheel + drawbarOffsets[i];
                if (tonewheelIndex >= 0 && tonewheelIndex < numTonewheels)
                {
                    float drawbarValue = parameters->getRawParameterValue("drawbar" + juce::String(i))->load();
                    output += std::sin(phases[tonewheelIndex]) * drawbarValue / 8.0f;

                    if (tonewheelIndex > 0)
                        output += std::sin(phases[tonewheelIndex - 1]) * drawbarValue * leakageAmount / 8.0f;
                    if (tonewheelIndex < numTonewheels - 1)
                        output += std::sin(phases[tonewheelIndex + 1]) * drawbarValue * leakageAmount / 8.0f;

                    phases[tonewheelIndex] += phaseIncrements[tonewheelIndex];
                    if (phases[tonewheelIndex] >= juce::MathConstants<double>::twoPi)
                        phases[tonewheelIndex] -= juce::MathConstants<double>::twoPi;
                }
            }

            // Key click synthesis
            if (voice.keyClickPhase < 0.05 * sampleRate) // 50ms duration
            {
                float envelope = std::exp(-voice.keyClickPhase / (0.01 * sampleRate)); // Fast decay
                output += noiseGenerator.nextFloat() * envelope * 0.1f;
                voice.keyClickPhase += 1.0f;
            }

            // Percussion synthesis
            if (voice.percussionGain > 0.0f)
            {
                float decayRate = percussionDecay == 0 ? 0.02f : 0.01f;
                float gain = percussionDynamics == 0 ? 0.2f : 0.1f;
                float freq = percussionHarmonic == 0 ? tonewheelFrequencies[baseTonewheel + 12] : tonewheelFrequencies[baseTonewheel + 19];
                output += std::sin(voice.percussionPhase) * voice.percussionGain * gain;
                voice.percussionPhase += freq * juce::MathConstants<float>::twoPi / sampleRate;
                voice.percussionGain *= (1.0f - decayRate);
            }
        }

        if (vibratoMode > 0)
        {
            float depth = vibratoMode <= 3 ? 0.005f * vibratoMode : 0.01f * (vibratoMode - 3);
            vibratoLfo = std::sin(vibratoPhase) * depth * sampleRate;
            vibratoPhase += 6.0f * juce::MathConstants<float>::twoPi / sampleRate;
            if (vibratoPhase >= juce::MathConstants<float>::twoPi)
                vibratoPhase -= juce::MathConstants<float>::twoPi;

            float delayedSample = vibratoDelay.popSample(0, vibratoLfo);
            output = vibratoMode <= 3 ? delayedSample : output * 0.5f + delayedSample * 0.5f;
            vibratoDelay.pushSample(0, output);
        }

        // Leslie effect
        if (leslieSpeed > 0)
        {
            float hornSpeed = leslieSpeed == 1 ? 0.7f : 7.0f; // Slow: 0.7Hz, Fast: 7Hz
            float rotorSpeed = leslieSpeed == 1 ? 0.6f : 6.0f;
            float hornDelay = 0.01f * sampleRate * (1.0f + std::sin(lesliePhaseHorn)) * 0.5f;
            float rotorDelay = 0.02f * sampleRate * (1.0f + std::sin(lesliePhaseRotor)) * 0.5f;
            lesliePhaseHorn += hornSpeed * juce::MathConstants<float>::twoPi / sampleRate;
            lesliePhaseRotor += rotorSpeed * juce::MathConstants<float>::twoPi / sampleRate;
            if (lesliePhaseHorn >= juce::MathConstants<float>::twoPi) lesliePhaseHorn -= juce::MathConstants<float>::twoPi;
            if (lesliePhaseRotor >= juce::MathConstants<float>::twoPi) lesliePhaseRotor -= juce::MathConstants<float>::twoPi;

            float hornSample = leslieDelay.popSample(0, hornDelay);
            float rotorSample = leslieDelay.popSample(1, rotorDelay);
            output = output * 0.5f + (hornSample * 0.3f + rotorSample * 0.2f);
            leslieDelay.pushSample(0, output);
            leslieDelay.pushSample(1, output);
        }

        monoWrite[sample] = output;
    }

    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float val = monoBuffer.getSample(0, sample) * 0.1f;
        leftChannel[sample] = val;
        rightChannel[sample] = val;
    }
}

int TonewheelSynth::getBaseTonewheelIndex(int midiNote) const
{
    int tonewheelIndex = midiNote - 36;
    if (tonewheelIndex < 0)
        tonewheelIndex += 12;
    else if (tonewheelIndex >= numTonewheels)
        tonewheelIndex -= 12;
    return tonewheelIndex;
}
