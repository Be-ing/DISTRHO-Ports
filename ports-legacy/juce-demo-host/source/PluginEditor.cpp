/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceDemoHostAudioProcessorEditor::JuceDemoHostAudioProcessorEditor (JuceDemoHostAudioProcessor& owner)
    : AudioProcessorEditor (owner)
{
    addAndMakeVisible (window = new MainHostWindow (owner.formatManager, owner.graph, *owner.appProperties));

    addAndMakeVisible (menu = new MenuBarComponent (window));

    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    resizeLimits.setSizeLimits (500, 400, 10000, 10000);

    {
        const ScopedLock csl(owner.midiKeyMutex);
        owner.midiKeyState = window->getMidiState();
    }

    setSize (800, 600);
}

JuceDemoHostAudioProcessorEditor::~JuceDemoHostAudioProcessorEditor()
{
    JuceDemoHostAudioProcessor& owner(getProcessor());

    {
        const ScopedLock csl(owner.midiKeyMutex);
        owner.midiKeyState = nullptr;
    }

    menu->setModel (nullptr);
}

//==============================================================================
void JuceDemoHostAudioProcessorEditor::paint (Graphics& g)
{
    g.setGradientFill (ColourGradient (Colours::white, 0, 0,
                                       Colours::grey, 0, (float) getHeight(), false));
    g.fillAll();
}

void JuceDemoHostAudioProcessorEditor::resized()
{
    menu->setBounds (0, 0, getWidth(), 24);
    window->setBounds (0, 24, getWidth(), getHeight()-24);
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
}
