
#include "PluginProcessor.h"
#include "PluginEditor.h"

Plugin_17005188AudioProcessorEditor::Plugin_17005188AudioProcessorEditor (Plugin_17005188AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    /* This part of the code is needed to create and edit the sliders in the GUI */
    
    // Time
    
    time.setRange(0, 2000, 100);
    time.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 20);
    time.setValue(*processor.inputTime * 1000);
    time.addListener(this);
    timeLabel.setColour(Label::textColourId, Colours::lightblue);
    timeLabel.setText("Time (ms)", dontSendNotification);
    timeLabel.attachToComponent(&time, false);
    
    // Feedback
    
    feedback.setRange(0, 1, 0.25);
    feedback.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 20);
    feedback.setValue(*processor.inputFeedback);
    feedback.addListener(this);
    feedbackLabel.setColour(Label::textColourId, Colours::lightblue);
    feedbackLabel.setText("Feedback (%)", dontSendNotification);
    feedbackLabel.attachToComponent(&feedback, false);
    
    // Cut Off Frequency
    
    cutOff.setRange(200, 20000, 200);
    cutOff.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 20);
    cutOff.addListener(this);
    cutOffLabel.setColour(Label::textColourId, Colours::lightblue);
    cutOffLabel.setText("Cut Off Frequency (Hz)", dontSendNotification);
    cutOffLabel.attachToComponent(&cutOff, false);
    
    // Dry/Wet Signal
    
    wet.setRange(0, 1, 0.1);
    wet.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 20);
    wet.addListener(this);
    wet.setSliderStyle(Slider::Rotary);
    wetLabel.setColour(Label::textColourId, Colours::lightblue);
    wetLabel.setText("Dry/Wet (%)", dontSendNotification);
    wetLabel.attachToComponent(&wet, false);
    
    // Pre-sets Combo Box
    
    StringArray settings = {"Small Room", "Medium Room", "Large Room"};
    presets.setTextWhenNothingSelected("Presets");
    presets.addItemList(settings, 1);
    presets.addListener(this);
    
    /* This part of the code is needed to make the sliders visible in the GUI */
    
    addAndMakeVisible(time);            // Time
    addAndMakeVisible(timeLabel);
    
    addAndMakeVisible(feedback);        // Feedback
    addAndMakeVisible(feedbackLabel);
    
    addAndMakeVisible(cutOff);          // Cut Off Frequency
    addAndMakeVisible(cutOffLabel);
    
    addAndMakeVisible(wet);             // Dry/Wet Signal
    addAndMakeVisible(wetLabel);
    
    addAndMakeVisible(presets);         // Pre-sets
    
    setSize (400, 400);     // Sets the size of the GUI
    
    startTimer(30);         // This timer is used to update the GUI following the automation in the DAW
}

Plugin_17005188AudioProcessorEditor::~Plugin_17005188AudioProcessorEditor()
{
}

void Plugin_17005188AudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));      // Sets the background colour of the plugin window

}

void Plugin_17005188AudioProcessorEditor::resized()
{
    /* This part of the code is needed to organise the sliders in the GUI, following the X and Y axis */
    
    time.setBoundsRelative(0.0, 0.3, 0.5, 0.2);         // Time
    feedback.setBoundsRelative(0.5, 0.3, 0.5, 0.2);     // Feedback
    cutOff.setBoundsRelative(0.0, 0.6, 0.5, 0.2);       // Cut Off Frequency
    wet.setBoundsRelative(0.5, 0.6, 0.5, 0.2);          // Dry/Wet Signal
    presets.setBoundsRelative(0.3, 0.1, 0.3, 0.1);      // Pre-sets
}

void Plugin_17005188AudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{   
    presets.setSelectedId(0);       // Sets the default label of the Combo Box to 'Presets'
                                    // So whenever the values do not match the ones set by the developer, the label reset to 'Presets'
    
    /* This part of the code makes the movement of the sliders communicating to the value input in the Processor */
    
    if (sliderThatWasMoved == &time)                                                // Time
    {
        *(processor.inputTime)=((float)sliderThatWasMoved->getValue() / 1000.0);
    }
    else if (sliderThatWasMoved == &feedback)                                       // Feedback
    {
        *(processor.inputFeedback)=((float)sliderThatWasMoved->getValue());
    }
    else if (sliderThatWasMoved == &cutOff)                                         // Cut Off Frequency
    {
        *(processor.inputCutOff)=((float)sliderThatWasMoved->getValue());
    }
    else if (sliderThatWasMoved == &wet)                                            // Dry/Wet Signal
    {
        *(processor.inputWet)=((float)sliderThatWasMoved->getValue());
    }
}

void Plugin_17005188AudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    /* This part of the code assign to each preset label specific input values */
    
    if (comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->getSelectedId() - 1) == "Small Room")           // Small Room Presets
    {
        *processor.inputTime = 300.0 / 1000.0;
        *processor.inputFeedback = 0.2;
        *processor.inputCutOff = 12000;
        *processor.inputWet = 0.2;
    }  
    else if (comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->getSelectedId() - 1) == "Medium Room")     // Medium Room Presets
    {
        *processor.inputTime = 500.0 / 1000.0;
        *processor.inputFeedback = 0.3;
        *processor.inputCutOff = 13000;
        *processor.inputWet = 0.4;
    }
    else if (comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->getSelectedId() - 1) == "Large Room")      // Large Room Presets
    {
        *processor.inputTime = 1000.0 / 1000.0;
        *processor.inputFeedback = 0.5;
        *processor.inputCutOff = 14000;
        *processor.inputWet = 0.6;
    }
}

void Plugin_17005188AudioProcessorEditor::timerCallback()
{
    /* This part of the code updates the GUI of the plugin following the DAW automations and settings */
    
    time.setValue(*processor.inputTime * 1000.0, dontSendNotification);     // Time
    feedback.setValue(*processor.inputFeedback, dontSendNotification);      // Feedback
    cutOff.setValue(*processor.inputCutOff, dontSendNotification);          // Cut Off Frequency
    wet.setValue(*processor.inputWet, dontSendNotification);                // Dry/Wet Signal
}
