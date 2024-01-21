#include "MainComponent.h"
#include "CustomLookAndFeel.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (900, 300);
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
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::FuturaStdCondensed_otf, BinaryData::FuturaStdCondensed_otfSize);

    // Create a Font from the Typeface
    juce::Font font(typeface);

    // Use the font
    g.setFont(font.withPointHeight(20.0f));

    // Now you can draw text with your custom font
    g.drawText("Hello, World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
