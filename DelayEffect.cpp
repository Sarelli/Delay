
#include "DelayEffect.h"

DelayEffect::DelayEffect(float delayTime, float sampleRate) {
    
    this->sampleRate = sampleRate;              // Makes the variable sampleRate equals to the imported variable sampleRate
    numSamples = delayTime * sampleRate;        // Expression used to calculate the number of samples
    audioBuffer.setSize(1, numSamples + 1);     // Sets the size of the Audio Buffer
    audioBuffer.clear();                        // Clear the Audio Buffer memory from previous stored data
    positionBuffer = 0;                         // Sets the initial position of the Audio Buffer to 0
};

float DelayEffect::processedSample(float sample, float inputTime, float inputFeedback)
{
    /* This function is used to process the samples received */
    
    float output = playback(inputTime);
    float feedback = output * inputFeedback;
    store(sample + feedback);
    ++positionBuffer;
    if (positionBuffer >= numSamples)
    {
        positionBuffer = 0;
    };
    return output;
    
};
        
void DelayEffect::store(float sample)
{
    /* This function is used to store the samples */
    
    float* const pointerW = audioBuffer.getWritePointer(0);
    pointerW[positionBuffer] = sample;
};

float DelayEffect::playback(float inputTime)
{
    /* This function is used to playback the processed samples */
    
    const float* pointerR = audioBuffer.getReadPointer(0);
    int delayPosition = positionBuffer - (inputTime * sampleRate) - 1;
    if (delayPosition < 0)
    {
        delayPosition = delayPosition + numSamples + 1;
    };
    return pointerR[delayPosition];
      
};

DelayEffect::~DelayEffect() {
    
};