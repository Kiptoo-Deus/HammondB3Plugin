/*
  ==============================================================================

    TonewheelSynth.h
    Created: 11 Jul 2025 11:19:14am
    Author:  Joel

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TonewheelSynth
{
public:
    TonewheelSynth();
    void setParameters(juce::AudioProcessorValueTreeState& params);
    void prepare(double sampleRate, int samplesPerBlock);
    void releaseResources();
    void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
    static constexpr int numDrawbars = 9;
    static constexpr int maxVoices = 32;
    static constexpr int numTonewheels = 91;

    struct Voice
    {
        int midiNote = -1;
        bool isActive = false;
        double startTime = 0.0;
        float percussionGain = 0.0f;
        float percussionPhase = 0.0f;
        float keyClickPhase = 0.0f;
    };

    static constexpr auto tonewheelFrequencies = std::array<double, numTonewheels>{
        32.40, 34.32, 36.35, 38.50, 40.80, 43.23, 45.80, 48.50, 51.39, 54.45, 57.69, 61.13,
        64.81, 68.64, 72.70, 77.00, 81.61, 86.46, 91.60, 97.01, 102.78, 108.91, 115.37, 122.25,
        129.61, 137.29, 145.41, 154.00, 163.22, 172.93, 183.21, 194.02, 205.57, 217.82, 230.74, 244.50,
        259.23, 274.57, 290.81, 308.01, 326.43, 345.85, 366.41, 388.03, 411.13, 435.63, 461.48, 489.00,
        518.46, 549.14, 581.62, 616.02, 652.86, 691.70, 732.82, 776.06, 822.26, 871.27, 922.96, 978.00,
        1036.91, 1098.29, 1163.24, 1232.04, 1305.73, 1383.41, 1465.64, 1552.12, 1644.52, 1742.53, 1845.93, 1956.01,
        2073.83, 2196.58, 2326.49, 2464.08, 2611.46, 2766.81, 2931.28, 3104.24, 3289.04, 3485.06, 3691.85, 3912.02,
        4147.65, 4393.15, 4652.97, 4928.16, 5222.92, 5533.62, 5862.56
    };

    std::array<double, numTonewheels> phases{0.0};
    std::array<double, numTonewheels> phaseIncrements{0.0};
    std::array<float, numTonewheels> detuneFactors{1.0f}; // For subtle detuning
    std::vector<Voice> voices;
    double sampleRate = 44100.0;

    juce::AudioProcessorValueTreeState* parameters = nullptr;
    static constexpr std::array<int, numDrawbars> drawbarOffsets = {0, 12, 7, 19, 24, 28, 31, 33, 36};
    static constexpr float leakageAmount = 0.02f;
    static constexpr float crosstalkAmount = 0.01f; // For crosstalk between tonewheels

    // Vibrato/Chorus
    juce::dsp::DelayLine<float> vibratoDelay{44100};
    float vibratoPhase = 0.0f;
    float vibratoLfo = 0.0f;

    // Leslie Effect
    juce::dsp::DelayLine<float> leslieDelay{44100};
    float lesliePhaseHorn = 0.0f;
    float lesliePhaseRotor = 0.0f;

    // Noise Generator for Key Click
    juce::Random noiseGenerator;

    int getBaseTonewheelIndex(int midiNote) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TonewheelSynth)
};
