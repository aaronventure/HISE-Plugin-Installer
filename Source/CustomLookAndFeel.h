/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 20 Jan 2024 12:02:42pm
    Author:  AV

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Custom graphics are done here
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:

    // Style the close button here, then create a new instance of it in the createDocumentWindowButton() method
    class CustomCloseButton : public juce::Button, private juce::Timer
    {
    public:
        CustomCloseButton(const juce::String& buttonName) : Button(buttonName)
        {
            startTimerHz(60); // Start the timer to update the color 60 times per second
        }

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
        {
            g.setColour(currentBackground);
            g.fillRect(0, 0, 40, 40); // Draw the background square
            g.setColour(currentColour);
            g.setFont(juce::Font("Arial", "Regular", 22.0f));
            g.drawText("X", getLocalBounds(), juce::Justification::centred, true);
        }

        void timerCallback() override
        {
            // Update the color
            currentColour = currentColour.interpolatedWith(targetColour, 0.2f);
            currentBackground = currentBackground.interpolatedWith(targetBackground, 0.2f);
            repaint();
        }

        void mouseEnter(const juce::MouseEvent&) override
        {
            targetColour = juce::Colour(0xffd2d2d2); // Highlighted color
            targetBackground = juce::Colour(0x10ffffff); // Highlighted color
        }

        void mouseExit(const juce::MouseEvent&) override
        {
            targetColour = juce::Colour(0xffbababa); // Mouse off color
            targetBackground = juce::Colour(0x0000000); // Mouse off color
        }

        void mouseDown(const juce::MouseEvent& event) override
        {
            targetColour = juce::Colour(0xffe9e9e9); // Down color
            targetBackground = juce::Colour(0x20ffffff); // Down color
            Button::mouseDown(event); // Pass the event to the parent class
        }

        void mouseUp(const juce::MouseEvent& event) override
        {
            targetColour = juce::Colour(0xffd2d2d2); // Highlighted color
            targetBackground = juce::Colour(0x10ffffff); // Highlighted color
            Button::mouseUp(event); // Pass the event to the parent class
        }

    private:
        juce::Colour currentColour = juce::Colour(0xffbababa); // Initial color
        juce::Colour targetColour = juce::Colour(0xffbababa); // Initial target color
        juce::Colour currentBackground = juce::Colour(0x00000000); // Initial background color
        juce::Colour targetBackground = juce::Colour(0x00000000); // Initial target background color
    };


    juce::Button* createDocumentWindowButton(int buttonType) override
    {
        // Here you can create your custom buttons. You can check the buttonType to determine which button to create.

        if (buttonType == juce::DocumentWindow::minimiseButton)
        {
            // Create your custom minimize button
        }
        else if (buttonType == juce::DocumentWindow::maximiseButton)
        {
            // Create your custom maximize button
        }
        else if (buttonType == juce::DocumentWindow::closeButton)
        {
            return new CustomCloseButton("close");
        }

        // Call the base class implementation if you want to keep the default look and feel
        return LookAndFeel_V4::createDocumentWindowButton(buttonType);
    }
};
