#include "PluginProcessor.h"
#include "PluginEditor.h"

HammondB3PluginAudioProcessor::HammondB3PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
#endif
{
}

HammondB3PluginAudioProcessor::~HammondB3PluginAudioProcessor() {}

void HammondB3PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
 
}

void HammondB3PluginAudioProcessor::releaseResources()
{
    // Clean up resources
}

void HammondB3PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


}

juce::AudioProcessorEditor* HammondB3PluginAudioProcessor::createEditor()
{
    return new HammondB3PluginAudioProcessorEditor(*this);
}

void HammondB3PluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Save parameters
}

void HammondB3PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Restore parameters
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HammondB3PluginAudioProcessor();
}
