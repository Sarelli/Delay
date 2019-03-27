
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "math.h"

Plugin_17005188AudioProcessor::Plugin_17005188AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    /* This part of the code is needed for the sliders to communicate the user input value to the plugin */
    
    addParameter (inputTime = new AudioParameterFloat("inputTime", "Input Time", NormalisableRange<float> (0.0, 2.0), 0.0));                // Time
    addParameter (inputFeedback = new AudioParameterFloat("inputFeedback", "Input Feedback", NormalisableRange<float> (0.0, 1.0), 0.0));    // Feedback
    addParameter (inputCutOff = new AudioParameterFloat("inputCutOff", "Input CutOff", NormalisableRange<float> (100, 20000), 0.0));        // Cut Off Frequency
    addParameter (inputWet = new AudioParameterFloat("inputWet", "Input Wet", NormalisableRange<float> (0.0, 1.0), 0.0));                   // Dry/Wet Signal
       
}

Plugin_17005188AudioProcessor::~Plugin_17005188AudioProcessor()
{
}

const String Plugin_17005188AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugin_17005188AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugin_17005188AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugin_17005188AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugin_17005188AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugin_17005188AudioProcessor::getNumPrograms()
{
    return 1;   
}

int Plugin_17005188AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugin_17005188AudioProcessor::setCurrentProgram (int index)
{
}

const String Plugin_17005188AudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugin_17005188AudioProcessor::changeProgramName (int index, const String& newName)
{
}

void Plugin_17005188AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
   /* This part of the code is needed to play the delayed signal out of both left and right channel at the same time */
   
   leftDelay = new DelayEffect(2, sampleRate);      // Left Channel
   rightDelay = new DelayEffect(2, sampleRate);     // Right Channel
   
   this->sampleRate = sampleRate;                   // Makes the variable sampleRate equals to the imported variable sampleRate
}

void Plugin_17005188AudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugin_17005188AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Plugin_17005188AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    /* Here is where the IIR filter used for the Cut Off Frequency parameter gets created */
    
    IIRCoefficients ic = IIRCoefficients::makeLowPass(sampleRate, inputCutOff->get());
    filterL.setCoefficients( ic );
    filterR.setCoefficients( ic );

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

    /* This part of the code is needed to playback the sample stored in a specific point of the buffer, according to the user settings */
       
       for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            if (channel == 0)
            {
               channelData[sample] = (filterL.processSingleSampleRaw(leftDelay->processedSample(channelData[sample], inputTime->get(), 
               inputFeedback->get())) * inputWet->get()) + (channelData[sample] * (1-inputWet->get()));
            } else if (channel == 1)
            {
               channelData[sample] = (filterR.processSingleSampleRaw(rightDelay->processedSample(channelData[sample], inputTime->get(), 
               inputFeedback->get())) * inputWet->get()) + (channelData[sample] * (1-inputWet->get()));
            }
        }
    }
}

bool Plugin_17005188AudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Plugin_17005188AudioProcessor::createEditor()
{
    return new Plugin_17005188AudioProcessorEditor (*this);
}

void Plugin_17005188AudioProcessor::getStateInformation (MemoryBlock& destData)
{

}

void Plugin_17005188AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugin_17005188AudioProcessor();
}
