
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DelayEffect {
    
    public:
    
        DelayEffect(float delayTime, float sampleRate);                             // Constructor
        ~DelayEffect();                                                             // Deconstructor
        float processedSample(float sample, float inputTime, float inputFeedback);  // Prototype of the function Processed Sample

    private:
    
        void store(float sample);                   // Prototype of the function Store
        float playback(float inputTime);            // Prototype of the function Playback
        AudioBuffer<float> audioBuffer;             // Declaring the Audio Buffer
        int positionBuffer, numSamples;             // Declaring the variables of type int
        float sampleRate, inputCutOff;              // Declaring the variables of type float
};