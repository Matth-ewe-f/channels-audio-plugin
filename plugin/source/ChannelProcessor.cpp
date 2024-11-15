#include "ChannelProcessor.h"
#include <format>

// === Lifecycle ==============================================================

ChannelProcessor::ChannelProcessor(std::string name)
    : channelName(name), delayLine(2410)
{
    sampleRate = 44100; // default value
    delay = new juce::AudioParameterFloat(
		name + "-delay", name + " Delay", 0, 50, 0
	);
	delayToggle = new juce::AudioParameterBool(
		name + "-delay-toggle", name + " Delay Toggle", true
	);
	gain = new juce::AudioParameterFloat(
		name + "-gain", name + " Gain", -60, 6, 0
	);
    lastGain = *gain;
	gainToggle = new juce::AudioParameterBool(
		name + "-gain-toggle", name + " Gain Toggle", true
	);
}

ChannelProcessor::~ChannelProcessor() {}

// === Processing Audio =======================================================

void ChannelProcessor::processAudio(float* buffer, int numSamples)
{
    float curGain = *gainToggle ? (float) pow(10, *gain / 20) : 1;
	bool gainRamp = !juce::approximatelyEqual(curGain, lastGain);
    int curDelay = *delayToggle ? (int) (*delay * sampleRate / 1000) : 0;
	// process each sample
	for (int i = 0;i < numSamples;i++)
	{
		// apply delay
		delayLine.addSample(buffer[i]);
        buffer[i] = delayLine.getDelayed(curDelay);
		// apply gain
		if (gainRamp)
        {
            float proportion = ((float) i) / numSamples;
			buffer[i] *= juce::jmap(proportion, lastGain, curGain);
        }
		else
        {
            buffer[i] *= curGain;
        }
		if (buffer[i] > 1)
			buffer[i] = 1;
		else if (buffer[i] < -1)
			buffer[i] = -1;
	}
    lastGain = curGain;
}