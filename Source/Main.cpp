/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
class InstallerApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    InstallerApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow.reset (new MainWindow (getApplicationName()));
        mainWindow->setContentOwned(new MainComponent(), true);
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //============================================================================================================================================

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
                g.setColour(currentColour);
                g.setFont(juce::Font("Arial", "Bold", 18.0f));
                g.drawText("X", getLocalBounds(), juce::Justification::centred, true);
            }

            void timerCallback() override
            {
                // Update the color
                currentColour = currentColour.interpolatedWith(targetColour, 0.2f);
                repaint();
            }

            void mouseEnter(const juce::MouseEvent&) override
            {
                targetColour = juce::Colour(0xffd2d2d2); // Highlighted color
            }

            void mouseExit(const juce::MouseEvent&) override
            {
                targetColour = juce::Colour(0xffbababa); // Mouse off color
            }

            void mouseDown(const juce::MouseEvent& event) override
            {
                targetColour = juce::Colour(0xffe9e9e9); // Down color
                Button::mouseDown(event); // Pass the event to the parent class
            }

            void mouseUp(const juce::MouseEvent& event) override
            {
                targetColour = juce::Colour(0xffd2d2d2); // Highlighted color
                Button::mouseUp(event); // Pass the event to the parent class
            }

        private:
            juce::Colour currentColour = juce::Colour(0xffbababa); // Initial color
            juce::Colour targetColour = juce::Colour(0xffbababa); // Initial target color
        };

        void drawDocumentWindowTitleBar(juce::DocumentWindow& window, juce::Graphics& g,
            int w, int h, int titleSpaceX, int titleSpaceW,
            const juce::Image* icon, bool drawTitleTextOnLeft) override
        {
            // Here you can customize the title bar. You can use the Graphics object to draw your custom title bar.
            //draw custom title bar

            // Call the base class implementation if you want to keep the default look and feel
            LookAndFeel_V4::drawDocumentWindowTitleBar(window, g, w, h, titleSpaceX, titleSpaceW, icon, drawTitleTextOnLeft);
        }

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




    //============================================================================================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::closeButton)
        {
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);

            setResizable (false, false);
            centreWithSize (getWidth(), getHeight());

            setLookAndFeel(&customLookAndFeel);

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.

            setLookAndFeel(nullptr); // clear the look and feel
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        CustomLookAndFeel customLookAndFeel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (InstallerApplication)
