#pragma once

#include "PluginProcessor.h"

class PluginEditor final :
    public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    // === Lifecycle ==========================================================
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    // === Graphics ===========================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // === Variables ==========================================================
    PluginProcessor& processorRef;
    juce::Slider leftDelay;
    juce::ToggleButton leftDelayToggle;
    juce::Slider leftGain;
    juce::ToggleButton leftGainToggle;
    juce::Slider rightDelay;
    juce::ToggleButton rightDelayToggle;
    juce::Slider rightGain;
    juce::ToggleButton rightGainToggle;

    // === User Interaction ===================================================
    void sliderValueChanged(juce::Slider*) override;

    // === Private Helper Functions ===========================================
    void initializeGuiValues();
    void setupDelaySlider(juce::Slider*);
    void setupGainSlider(juce::Slider*);
    void setupToggle(juce::ToggleButton*, juce::AudioParameterBool*);
    void positionSlider(juce::Slider*, int);
    void positionToggle(juce::ToggleButton*, int);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
