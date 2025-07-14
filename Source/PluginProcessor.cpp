#include "PluginProcessor.h"
#include "PluginEditor.h"

HammondB3PluginAudioProcessor::HammondB3PluginAudioProcessor()
    : juce::AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    parameters = std::make_unique<juce::AudioProcessorValueTreeState>(
        *this, nullptr, juce::Identifier("HammondB3"), createParameterLayout());
    tonewheelSynth.setParameters(*parameters);
}

HammondB3PluginAudioProcessor::~HammondB3PluginAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout HammondB3PluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Drawbars (0–8, corresponding to 16', 5⅓', 8', 4', 2⅔', 2', 1⅗', 1⅓', 1')
    for (int i = 0; i < 9; ++i)
    {
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            "drawbar" + juce::String(i),
            "Drawbar " + juce::String(i + 1),
            0.0f, 8.0f, 4.0f));
    }

    // Vibrato/Chorus (V1–V3, C1–C3)
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "vibratoChorus", "Vibrato/Chorus",
        juce::StringArray{"Off", "V1", "V2", "V3", "C1", "C2", "C3"}, 0));

    // Leslie Speed
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "leslieSpeed", "Leslie Speed",
        juce::StringArray{"Stop", "Slow", "Fast"}, 1));

    // Percussion
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "percussionOn", "Percussion On", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "percussionHarmonic", "Percussion Harmonic",
        juce::StringArray{"2nd", "3rd"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "percussionDynamics", "Percussion Dynamics",
        juce::StringArray{"Normal", "Soft"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "percussionDecay", "Percussion Decay",
        juce::StringArray{"Fast", "Slow"}, 0));

    return layout;
}

void HammondB3PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    tonewheelSynth.prepare(sampleRate, samplesPerBlock);
}

void HammondB3PluginAudioProcessor::releaseResources()
{
    tonewheelSynth.releaseResources();
}

void HammondB3PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = getTotalNumInputChannels(); i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    tonewheelSynth.process(buffer, midiMessages);
}

juce::AudioProcessorEditor* HammondB3PluginAudioProcessor::createEditor()
{
    return new HammondB3PluginAudioProcessorEditor(*this);
}

void HammondB3PluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HammondB3PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        parameters->replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HammondB3PluginAudioProcessor();
}
