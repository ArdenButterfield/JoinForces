#include "PluginEditor.h"
#include <iostream>

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), mappingsPanel (*p.getPluginGroup())
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (mappingsPanel);
    addAndMakeVisible(addPluginButton);

    addPluginButton.setButtonText("Add plugin");

    // this chunk of code instantiates and opens the melatonin inspector
    addPluginButton.onClick = [&] {
        fileChooser = std::make_unique<juce::FileChooser> ("Select an audio plugin...",
                                       juce::File::getSpecialLocation (juce::File::globalApplicationsDirectory),
                                       "*");

        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories | juce::FileBrowserComponent::canSelectFiles;

        fileChooser->launchAsync (folderChooserFlags, [this] (const juce::FileChooser& chooser)
        {
            juce::File file (chooser.getResult());

            juce::String errorMessage;
            processorRef.getPluginGroup()->addPlugin(file, errorMessage);
            if (errorMessage != "") {
                std::cout << "error in adding plugin:" << errorMessage << "\n";
            }
        });

    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    mappingsPanel.setBounds (getLocalBounds().withTrimmedTop(10).withTrimmedLeft(10).withTrimmedRight(10).withTrimmedBottom(40));
    addPluginButton.setBounds (getLocalBounds().withHeight(30).withBottomY(getHeight()).withWidth(150));
}
