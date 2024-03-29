/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 20 Jan 2024 12:02:42pm
    Author:  AV

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GlobalFontData.h"

// Custom graphics are done here
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:

    void drawDocumentWindowTitleBar(juce::DocumentWindow& window,
        juce::Graphics& g,
        int w, int h,
        int titleSpaceX, int titleSpaceW,
        const juce::Image* icon,
        bool drawTitleTextOnLeft) override
    {
        // Fill the title bar background
        g.fillAll(juce::Colour(0xff222222));

        // Create a Typeface from the binary data
        juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(GlobalFontData::Controls, GlobalFontData::ControlsSize);

        // Create a Font from the Typeface
        juce::Font font(typeface);

        // Use the font
        g.setFont(font.withPointHeight(18.0f));

        g.setColour(juce::Colours::white); // Choose an appropriate text color

        // Retrieve the title of the window
        juce::String title = window.getName();

        // Set the area where the title will be drawn
        juce::Rectangle<int> textArea(0, 0, window.getWidth(), h);

        // Draw the title text
        g.drawText(title, textArea, juce::Justification::centred, true);

        // Temporarily draw the text area for debugging
        //g.setColour(juce::Colours::red);
        //g.drawRect(textArea); 
    }

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


    // Style the minimize button here, then create a new instance of it in the createDocumentWindowButton() method
    class CustomMinimizeButton : public juce::Button, private juce::Timer
    {
    public:
        CustomMinimizeButton(const juce::String& buttonName) : Button(buttonName)
        {
            startTimerHz(60); // Start the timer to update the color 60 times per second
        }

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
        {
            g.setColour(currentBackground);
            g.fillRect(0, 0, 40, 40); // Draw the background square
            g.setColour(currentColour);
            g.setFont(juce::Font("Arial", "Regular", 22.0f));
            g.drawText("/", getLocalBounds(), juce::Justification::centred, true);
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
            return new CustomMinimizeButton("minimize");
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

class DirectorySelectButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    DirectorySelectButtonLookAndFeel(juce::String& path) : currentPath(path) {}

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        g.setColour(shouldDrawButtonAsDown ? juce::Colour(0xffe9e9e9)
            : shouldDrawButtonAsHighlighted ? juce::Colour(0xffd2d2d2)
            : juce::Colour(0xffbababa));

        juce::Rectangle<int> rect(0, 0, button.getWidth(), button.getHeight());
        g.drawRect(rect, 1);

    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        // Draw the button text in the center of the button
        g.setColour(shouldDrawButtonAsDown ? juce::Colour(0xffe9e9e9)
            : shouldDrawButtonAsHighlighted ? juce::Colour(0xffd2d2d2)
            : juce::Colour(0xffbababa));

        // Create a Typeface from the binary data
        juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(GlobalFontData::Controls, GlobalFontData::ControlsSize);

        // Create a Font from the Typeface
        juce::Font font(typeface);

        // Use the font
        g.setFont(font.withPointHeight(16.0f));

        // Use the current path as the button text
        g.drawText(currentPath, button.getLocalBounds(), juce::Justification::centred, true);
    }

private:
    juce::String& currentPath;
};

class InstallButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        g.setColour(shouldDrawButtonAsDown ? juce::Colour(0xffe9e9e9)
            : shouldDrawButtonAsHighlighted ? juce::Colour(0xffd2d2d2)
            : juce::Colour(0xffbababa));

        juce::Rectangle<int> rect(0, 0, button.getWidth(), button.getHeight());
        g.drawRect(rect, 2);

    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        // Draw the button text in the center of the button
        g.setColour(shouldDrawButtonAsDown ? juce::Colour(0xffe9e9e9)
            : shouldDrawButtonAsHighlighted ? juce::Colour(0xffd2d2d2)
            : juce::Colour(0xffbababa));

        // Create a Typeface from the binary data
        juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(GlobalFontData::Controls, GlobalFontData::ControlsSize);

        // Create a Font from the Typeface
        juce::Font font(typeface);

        // Use the font
        g.setFont(font.withPointHeight(26.0f));

        // Use the current path as the button text
        g.drawText("Install", button.getLocalBounds(), juce::Justification::centred, true);
    }
};