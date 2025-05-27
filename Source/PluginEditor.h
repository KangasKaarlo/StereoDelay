/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CustomLookAndFeel : public juce::LookAndFeel_V2
{
public:
    CustomLookAndFeel() = default;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
};
class StereoDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereoDelayAudioProcessorEditor (StereoDelayAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~StereoDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
	

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoDelayAudioProcessor& audioProcessor;
	CustomLookAndFeel customLookAndFeel;

	juce::Slider delayTimeSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;

	juce::Slider feedbackSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;

	juce::Slider ratioSlider;;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;

	juce::Label delayTimeText;
	juce::Label feedbackText;
	juce::Label ratioText;

    juce::GroupComponent border;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayAudioProcessorEditor)
};