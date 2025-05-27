/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class AudioDelay
{

public:
    void initDelay(double _sampleRate, int _samplesPerBlock, int numOfChannels);
    void process(juce::AudioBuffer<float>& buffer);
    float feedback = 0.5f;
    void setDelayTime(float time);
    float ratio = 50.0f;
    
private:
    float delayTime = 800;
    int delayBufferSize;
    int sampleRate;
    int samplesPerBlock;
    int writeIndex;
    int wrapMask;
    int delayInSamples;
    int writePos = 0;
	 // in ms
    juce::AudioBuffer<float> delayBuffer;
};

class StereoDelayAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
    #if JucePlugin_Enable_ARA
        , public juce::AudioProcessorARAExtension
    #endif
{
public:
    //==============================================================================
    StereoDelayAudioProcessor();
    ~StereoDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState treeState;
private:
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	AudioDelay delay;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayAudioProcessor)
};
