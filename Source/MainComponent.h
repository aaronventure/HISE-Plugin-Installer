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

    void MainComponent::setTitle(const juce::String& newTitle);
    void setBackgroundImage(const juce::Image& newBackground);
    juce::TextButton* MainComponent::createAndDisplayButton(const juce::String& buttonName, int x, int y, int width, int height, juce::LookAndFeel* lookAndFeel);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void setMessage(const juce::String& message);

    ~MainComponent() override;

private:
    //==============================================================================
    juce::Label messageLabel;
    juce::Label currentTaskLabel;


    juce::Image backgroundImage;

    juce::String title = "Installer"; // Default title

    juce::OwnedArray<juce::TextButton> buttons;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
