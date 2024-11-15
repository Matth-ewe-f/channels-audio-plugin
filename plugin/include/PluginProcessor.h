#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "ChannelProcessor.h"

class PluginProcessor final : public juce::AudioProcessor
{
public:
    // === Plugin Parameters ==================================================
    juce::AudioParameterFloat* leftDelay;
    juce::AudioParameterBool* leftDelayToggle;
    juce::AudioParameterFloat* leftGain;
    juce::AudioParameterBool* leftGainToggle;
    juce::AudioParameterFloat* rightDelay;
    juce::AudioParameterBool* rightDelayToggle;
    juce::AudioParameterFloat* rightGain;
    juce::AudioParameterBool* rightGainToggle;

    // === Lifecycle ==========================================================
    PluginProcessor();
    ~PluginProcessor() override;

    // === Information about the Plugin =======================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    bool hasEditor() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    // === Processing Audio ===================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    // === State Management ===================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // === Factory Functions ==================================================
    juce::AudioProcessorEditor* createEditor() override;

private:
    ChannelProcessor left;
    ChannelProcessor right;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
