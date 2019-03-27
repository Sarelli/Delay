
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Plugin_17005188AudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public ComboBox::Listener, private Timer
{
public:
    Plugin_17005188AudioProcessorEditor (Plugin_17005188AudioProcessor&);   // Constructor
    ~Plugin_17005188AudioProcessorEditor();                                 // Deconstructor

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;      // Prototype of Member Function of the class Slider needed to get the value input from the slider when it gets changed
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;   // Prototype of Member Function of the class ComboBox needed to get the value input from the combo box when it gets changed

private:
    
    void timerCallback() override;                                  // Prototype of the function Timer Callback, needed to process automation
    Slider time, feedback, cutOff, wet;                             // Declaring the variables for the class Slider
    Label timeLabel, feedbackLabel, cutOffLabel, wetLabel;          // Declaring the variables for the class Label
    ComboBox presets;                                               // Declaring the variables for the class ComboBox
    Plugin_17005188AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin_17005188AudioProcessorEditor)
};
