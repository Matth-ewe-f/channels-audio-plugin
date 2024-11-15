#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DelayLine.h"

// === Lifecycle ==============================================================

PluginProcessor::PluginProcessor()
	: AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	),
	left("Left"),
	right("Right")
{
	leftDelay = left.delay;
	leftDelayToggle = left.delayToggle;
	leftGain = left.gain;
	leftGainToggle = left.gainToggle;
	rightDelay = right.delay;
	rightDelayToggle = right.delayToggle;
	rightGain = right.gain;
	rightGainToggle = right.gainToggle;
}

PluginProcessor::~PluginProcessor() { }

// === Factory Functions ======================================================

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
	return new PluginProcessor();
}

juce::AudioProcessorEditor *PluginProcessor::createEditor()
{
	return new PluginEditor(*this);
}

// === Information about the Plugin ===========================================

const juce::String PluginProcessor::getName() const
{
	return "Channels";
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

bool PluginProcessor::hasEditor() const
{
	return true;
}

int PluginProcessor::getNumPrograms()
{
	// should always be at least 1 (0 can confuse hosts)
	return 1;
}

int PluginProcessor::getCurrentProgram()
{
	return 0;
}

void PluginProcessor::setCurrentProgram(int index)
{
	juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index)
{
	juce::ignoreUnused(index);
	return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String &newName)
{
	juce::ignoreUnused(index, newName);
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

#if !JucePlugin_IsSynth
	// Check if the input matches the output
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif
	return true;
#endif
}

// === Process Audio ==========================================================

void PluginProcessor::prepareToPlay
(double rate, int samplesPerBlock)
{
	left.setSampleRate(rate);
	right.setSampleRate(rate);
	juce::ignoreUnused(samplesPerBlock);
}

void PluginProcessor::releaseResources() {}

void PluginProcessor::processBlock
(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
	juce::ignoreUnused(midiMessages);
	auto numInputChannels = getTotalNumInputChannels();
	auto numOutputChannels = getTotalNumOutputChannels();
	// zeroes out any unused outputs (if there are any)
	for (auto i = numInputChannels; i < numOutputChannels; i++)
		buffer.clear(i, 0, buffer.getNumSamples());
	// process each channel of the audio independently
	for (int channel = 0; channel < numInputChannels; channel++)
	{
		float* data = buffer.getWritePointer(channel);
		int numSamples = buffer.getNumSamples();
		if (channel == 0)
			left.processAudio(data, numSamples);
		else if (channel == 1)
			right.processAudio(data, numSamples);
	}
}

// === State Management =======================================================

void PluginProcessor::getStateInformation(juce::MemoryBlock &dest)
{
	auto stream = juce::MemoryOutputStream(dest, true);
	stream.writeFloat(*leftDelay);
	stream.writeBool(*leftDelayToggle);
	stream.writeFloat(*leftGain);
	stream.writeBool(*leftGainToggle);
	stream.writeFloat(*rightDelay);
	stream.writeBool(*rightDelayToggle);
	stream.writeFloat(*rightGain);
	stream.writeBool(*rightGainToggle);
}

void PluginProcessor::setStateInformation
(const void *data, int sizeInBytes)
{
	auto stream = juce::MemoryInputStream(data, (size_t) sizeInBytes, false);
	*leftDelay = stream.readFloat();
	*leftDelayToggle = stream.readBool();
	*leftGain = stream.readFloat();
	*leftGainToggle = stream.readBool();
	*rightDelay = stream.readFloat();
	*rightDelayToggle = stream.readBool();
	*rightGain = stream.readFloat();
	*rightGainToggle = stream.readBool();
}