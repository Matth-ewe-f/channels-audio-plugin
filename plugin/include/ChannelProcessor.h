#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "DelayLine.h"

class ChannelProcessor
{
public:
    // === Parameters =========================================================
    juce::AudioParameterFloat* delay;
    juce::AudioParameterBool* delayToggle;
    juce::AudioParameterFloat* gain;
    juce::AudioParameterBool* gainToggle;

    // === Lifecycle ==========================================================
    ChannelProcessor(std::string name);
    ~ChannelProcessor();

    // === Process Audio ======================================================
    void processAudio(float* buffer, int numSamples);
    inline void setSampleRate(double rate) { sampleRate = rate; }

private:
    std::string channelName;
    DelayLine delayLine;
    double sampleRate;
    float lastGain;
};