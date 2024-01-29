#include "MainComponent.h"
#include "CustomLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

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

juce::TextButton* MainComponent::createAndDisplayButton(const juce::String& buttonName, int x, int y, int width, int height, juce::LookAndFeel* lookAndFeel)
{
    // Create a Button object
    juce::TextButton* myButton = new juce::TextButton(buttonName);

    // Set the button's position and size
    myButton->setBounds(x, y, width, height);

    // Set the LookAndFeel for the button
    if (lookAndFeel != nullptr)
    {
        myButton->setLookAndFeel(lookAndFeel);
    }

    // Add the button to the interface
    addAndMakeVisible(myButton);

    // Add the button to the array
    buttons.add(myButton);

    // Return a pointer to the created button
    return myButton;
}




void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(0xff222222));

    g.setColour(juce::Colours::white);

    g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

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

