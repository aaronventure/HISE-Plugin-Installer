#include "MainComponent.h"
#include "CustomLookAndFeel.h"

//==============================================================================

MainComponent::MainComponent()
{
    addAndMakeVisible(messageLabel);
    messageLabel.setJustificationType(juce::Justification::centred);
    setSize(900, 300);
}

void MainComponent::setBackgroundImage(const juce::Image& newBackground)
{
    backgroundImage = newBackground;
    repaint();
}

MainComponent::~MainComponent()
{
}

//==============================================================================

void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(0xff222222));

    g.setColour(juce::Colours::white);

    // Create a Typeface from the binary data
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(GlobalFontData::Controls, GlobalFontData::ControlsSize);

    // Create a Font from the Typeface
    juce::Font font(typeface);

    // Use the font
    g.setFont(font.withPointHeight(20.0f));

    // Now you can draw text with your custom font
    g.drawText("", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    messageLabel.setBounds(getLocalBounds());
}

void MainComponent::setMessage(const juce::String& message)
{
    messageLabel.setText(message, juce::dontSendNotification);

    // Create a Typeface from the binary data and apply it to the label
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(GlobalFontData::Controls, GlobalFontData::ControlsSize);
    juce::Font font(typeface);
    messageLabel.setFont(font.withPointHeight(30.0f));
}

