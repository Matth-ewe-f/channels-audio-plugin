#include "PluginProcessor.h"
#include "PluginEditor.h"

// === Lifecycle ==============================================================

PluginEditor::PluginEditor
(PluginProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    setSize(360, 230);
    setupDelaySlider(&leftDelay);
    setupToggle(&leftDelayToggle, processorRef.leftDelayToggle);
    setupGainSlider(&leftGain);
    setupToggle(&leftGainToggle, processorRef.leftGainToggle);
    setupDelaySlider(&rightDelay);
    setupToggle(&rightDelayToggle, processorRef.rightDelayToggle);
    setupGainSlider(&rightGain);
    setupToggle(&rightGainToggle, processorRef.rightGainToggle);
    initializeGuiValues();
}

PluginEditor::~PluginEditor() {}

// === Graphics ===============================================================

void PluginEditor::paint(juce::Graphics &g)
{
    auto colorId = juce::ResizableWindow::backgroundColourId;
    g.fillAll(getLookAndFeel().findColour(colorId));
}

void PluginEditor::resized()
{
    positionSlider(&leftDelay, 0);
    positionToggle(&leftDelayToggle, 0);
    positionSlider(&leftGain, 1);
    positionToggle(&leftGainToggle, 1);
    positionSlider(&rightDelay, 2);
    positionToggle(&rightDelayToggle, 2);
    positionSlider(&rightGain, 3);
    positionToggle(&rightGainToggle, 3);
}

// === User Interaction =======================================================

void PluginEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &leftDelay)
    {
        *processorRef.leftDelay = (float) slider->getValue();
    }
    else if (slider == &leftGain)
    {
        *processorRef.leftGain = (float) slider->getValue();
    }
    else if (slider == &rightDelay)
    {
        *processorRef.rightDelay = (float) slider->getValue();
    }
    else if (slider == &rightGain) 
    {
        *processorRef.rightGain = (float) slider->getValue();
    }
}

// === Private Helper Functions ===============================================

void PluginEditor::initializeGuiValues()
{
    auto notify = juce::NotificationType::dontSendNotification;
    leftDelay.setValue(*processorRef.leftDelay);
    leftDelayToggle.setToggleState(*processorRef.leftDelayToggle, notify);
    leftGain.setValue(*processorRef.leftGain);
    leftGainToggle.setToggleState(*processorRef.leftGainToggle, notify);
    rightDelay.setValue(*processorRef.rightDelay);
    rightDelayToggle.setToggleState(*processorRef.rightDelayToggle, notify);
    rightGain.setValue(*processorRef.rightGain);
    rightGainToggle.setToggleState(*processorRef.rightGainToggle, notify);
}

void PluginEditor::setupDelaySlider(juce::Slider* slider)
{
    slider->setSliderStyle(juce::Slider::LinearVertical);
    slider->setRange(0, 50, 0.1);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    slider->setTextValueSuffix(" ms");
    slider->addListener(this);
    addAndMakeVisible(slider);
}

void PluginEditor::setupGainSlider(juce::Slider* slider)
{
    slider->setSliderStyle(juce::Slider::LinearVertical);
    slider->setRange(-60, 6, 0.1);
    slider->setSkewFactor(3);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
    slider->setTextValueSuffix(" dB");
    slider->addListener(this);
    addAndMakeVisible(slider);
}

void PluginEditor::setupToggle
(juce::ToggleButton* button, juce::AudioParameterBool* parameter)
{
    button->onClick = [button, parameter] {
        *parameter = button->getToggleState();
    };
    addAndMakeVisible(button);
}

void PluginEditor::positionSlider(juce::Slider* slider, int index)
{
    int width = 60;
    int padding = 20;
    int x = 30 + ((width + padding) * index);
    slider->setBounds(x, 20, width, getHeight() - 70);
}

void PluginEditor::positionToggle(juce::ToggleButton* button, int index)
{
    int width = 60;
    int padding = 20;
    int x = 49 + ((width + padding) * index);
    button->setBounds(x, getHeight() - 40, 24, 20);
}