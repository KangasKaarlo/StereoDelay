/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoDelayAudioProcessor::StereoDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "parameters", createParameterLayout())
#endif
{
    // Attach a listener to the treeState for parameter changes
    treeState.addParameterListener("delayTime", this);
    treeState.addParameterListener("feedback", this);
    treeState.addParameterListener("ratio", this);
	//treeState.addParameterListener("pingpong", this);
}

StereoDelayAudioProcessor::~StereoDelayAudioProcessor()
{
    treeState.removeParameterListener("delayTime", this);
    treeState.removeParameterListener("feedback", this);
    treeState.removeParameterListener("ratio", this);
	//treeState.removeParameterListener("pingpong", this);
}


juce::AudioProcessorValueTreeState::ParameterLayout StereoDelayAudioProcessor::createParameterLayout()
{

    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    auto delayTimeParam = std::make_unique<juce::AudioParameterFloat>(
        "delayTime",
        "DelayTime",
        juce::NormalisableRange<float>(0.0f, 2000.0f),
        800.0f
    );
    auto feedbackParam = std::make_unique<juce::AudioParameterFloat>(
        "feedback",
        "Feedback",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f
    );
    auto ratioParam = std::make_unique<juce::AudioParameterFloat>(
        "ratio",
        "Ratio",
        juce::NormalisableRange<float>(0.0f, 100.0f),
        50.0f
    );

	/*auto pingpongParam = std::make_unique<juce::AudioParameterBool>(
		"delayType",
		"DelayType",
		false
	);*/
    

    params.push_back(std::move(delayTimeParam));
    params.push_back(std::move(feedbackParam));
    params.push_back(std::move(ratioParam));
    //params.push_back(std::move(pingpongParam));
    return { params.begin(), params.end() };
}

void StereoDelayAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{// This function will be called when a parameter changes.
    // You can use this to update the UI or perform other actions based on the parameter change.
    
	if (parameterID == "delayTime")
	{
        delay.setDelayTime(newValue);

	}
	else if (parameterID == "feedback")
	{
		delay.feedback = newValue;
	}
	else if (parameterID == "ratio")
	{
		delay.ratio = newValue;
	}

}

//==============================================================================
const juce::String StereoDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereoDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	delay.initDelay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());



}

void StereoDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StereoDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
	delay.process(buffer);
}

//==============================================================================
bool StereoDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereoDelayAudioProcessor::createEditor()
{
    return new StereoDelayAudioProcessorEditor (*this, treeState);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void StereoDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoDelayAudioProcessor();
}

void AudioDelay::initDelay(double _sampleRate, int _samplesPerBlock, int numOfChannels)
{
	sampleRate = _sampleRate;
	samplesPerBlock = _samplesPerBlock;
	int maxDelayTime = 2; // in seconds
	// Calculate the buffer length in samples
    int bufferLength = (unsigned int)(samplesPerBlock * maxDelayTime * (sampleRate)) + 1;
	// Find the nearest power of two
    delayBufferSize = (unsigned int)(pow(2, ceil(log(bufferLength) / log(2))));
	wrapMask = delayBufferSize - 1;
    delayBuffer.setSize(numOfChannels, delayBufferSize);
    delayBuffer.clear();
	delayInSamples = (int)(delayTime * sampleRate / 1000.0);
}

void AudioDelay::process(juce::AudioBuffer<float>& buffer)
{
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            // Calculate the read position with wrapping
            int readPos = (writePos - delayInSamples + delayBufferSize) % delayBufferSize;
			// Read the delay buffer
			auto delay = delayBuffer.getReadPointer(channel)[readPos];
			auto input =  buffer.getReadPointer(channel)[i];
			// Write the delay buffer
			delayBuffer.setSample(channel, writePos, input + feedback * delay);
			// Write the output buffer
			buffer.setSample(channel, i, input * (1 - ratio / 100) +  delay * (ratio / 100));
        }
        writePos++;
        writePos &= wrapMask;
    }
}

void AudioDelay::setDelayTime(float time)
{
	delayTime = time;
	delayInSamples = (int)(delayTime * sampleRate / 1000.0);
}

