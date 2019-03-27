
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayEffect.h"


class Plugin_17005188AudioProcessor  : public AudioProcessor
{
public:

    Plugin_17005188AudioProcessor();                                                    // Constructor
    ~Plugin_17005188AudioProcessor();                                                   // Deconstructor

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    /* Here different variables get assigned to the subclass AudioParameterFloat to be able to communicate the values to the Editor */
    
    AudioParameterFloat* inputTime;
    AudioParameterFloat* inputFeedback;
    AudioParameterFloat* inputCutOff;
    AudioParameterFloat* inputWet;
    
private:
    
    ScopedPointer<DelayEffect> rightDelay, leftDelay;       // Declaring the Scoped Pointers
    IIRFilter filterL, filterR;                             // Declaring the variables for the class IIRFilter
    double sampleRate;                                      // Declaring the variables of type double
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin_17005188AudioProcessor)
};
