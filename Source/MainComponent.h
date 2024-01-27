#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    void paint(juce::Graphics& g) override;
    void resized() override;
    void setMessage(const juce::String& message);

    ~MainComponent() override;

private:
    //==============================================================================
    juce::Label messageLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
