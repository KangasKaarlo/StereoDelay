/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoDelayAudioProcessorEditor::StereoDelayAudioProcessorEditor (StereoDelayAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(600, 300);

	addAndMakeVisible(delayTimeSlider);
	delayTimeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	delayTimeSlider.setLookAndFeel(&customLookAndFeel);
	delayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 30);
	delayTimeAttachment.reset( new juce::AudioProcessorValueTreeState::SliderAttachment(
		vts,
		"delayTime",
		delayTimeSlider
	));

	addAndMakeVisible(feedbackSlider);
	feedbackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	feedbackSlider.setLookAndFeel(&customLookAndFeel);
	feedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	feedbackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
		vts,
		"feedback",
		feedbackSlider
	));

	addAndMakeVisible(ratioSlider);
	ratioSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	ratioSlider.setLookAndFeel(&customLookAndFeel);
	ratioSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	ratioAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
		vts,
		"ratio",
		ratioSlider
	));

	addAndMakeVisible(delayTimeText);
	delayTimeText.setText("Time", juce::dontSendNotification);
	delayTimeText.setJustificationType(juce::Justification::centred);
	delayTimeText.setBorderSize(juce::BorderSize(10));
	delayTimeText.setFont(juce::Font("Arial", 20, 0));
	delayTimeText.attachToComponent(&delayTimeSlider, false);

	addAndMakeVisible(feedbackText);
	feedbackText.setText("Feedback", juce::dontSendNotification);
	feedbackText.setJustificationType(juce::Justification::centred);
	feedbackText.setBorderSize(juce::BorderSize(10));
	feedbackText.setFont(juce::Font("Arial", 20, 0));
	feedbackText.attachToComponent(&feedbackSlider, false);

	addAndMakeVisible(ratioText);
	ratioText.setText("Mix", juce::dontSendNotification);
	ratioText.setJustificationType(juce::Justification::centred);
	ratioText.setBorderSize(juce::BorderSize(10));
	ratioText.setFont(juce::Font("Arial", 20, 0));
	ratioText.attachToComponent(&ratioSlider, false);

	addAndMakeVisible(border);
}

StereoDelayAudioProcessorEditor::~StereoDelayAudioProcessorEditor()
{
}

//==============================================================================
void StereoDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
	auto area = getLocalBounds();
	auto myImage = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);
	g.setColour(juce::Colours::white);
	g.drawImage(myImage, area.toFloat());
	auto sliderBackground = juce::ImageCache::getFromMemory(BinaryData::silverbrushedmetalbutton_png, BinaryData::silverbrushedmetalbutton_pngSize);
	g.drawImage(sliderBackground, delayTimeSlider.getBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
	g.drawImage(sliderBackground, feedbackSlider.getBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
	g.drawImage(sliderBackground, ratioSlider.getBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
}

void StereoDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	
	auto area = getLocalBounds();
	int sliderSize = area.getHeight() / 3;

	delayTimeSlider.setBounds(area.getWidth() / 3 - sliderSize, area.getHeight() / 2 - sliderSize / 2, sliderSize, sliderSize);
	feedbackSlider.setBounds(area.getWidth()/ 2 - sliderSize / 2, area.getHeight() / 2 - sliderSize / 2, sliderSize, sliderSize);
	ratioSlider.setBounds(area.getWidth() / 3 * 2, area.getHeight() / 2 - sliderSize / 2, sliderSize, sliderSize);

	int borderWidth = 10;
	border.setBounds(borderWidth, borderWidth, getWidth() - borderWidth * 2, getHeight() - borderWidth * 2);
}
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
	//Helper varibales
	auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
	auto centreX = (float)x + (float)width * 0.5f;
	auto centreY = (float)y + (float)height * 0.5f;
	auto rx = centreX - radius;
	auto ry = centreY - radius;
	auto rw = radius * 2.0f;
	auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	// outline
	g.setColour(juce::Colours::black);
	g.drawEllipse(rx, ry, rw, rw, 1.0f);

	juce::Path p;
	auto pointerLength = radius * 0.5f;
	auto pointerThickness = 4.0f;
	p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
	// pointer
	g.setColour(juce::Colours::black);
	g.fillPath(p);
}