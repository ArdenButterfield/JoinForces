#include "PluginEditor.h"
#include <iostream>

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), mappingsPanel (*p.getPluginGroup()), positionVisualizer (*processorRef.getForceFeedbackInterface())
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible(mappingViewport);
    mappingViewport.setViewedComponent(&mappingsPanel, false);

    addAndMakeVisible(addPluginButton);
    addPluginButton.setButtonText("Add plugin");

    addAndMakeVisible(createMappingButton);
    createMappingButton.setButtonText("Create mapping");

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
    createMappingButton.onClick = [&] {
        processorRef.getMappingCenter()->createMappingAtCurrentState();
    };

    addAndMakeVisible(positionVisualizer);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    setResizable (true, true);
}

PluginEditor::~PluginEditor()
{
    mappingViewport.setViewedComponent(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::black);
    g.fillRect(mappingViewport.getBounds());
}

void PluginEditor::resized()
{
    auto usableArea = getLocalBounds().withSizeKeepingCentre(getWidth() - 20, getHeight() - 20);
    positionVisualizer.setBounds(usableArea.withHeight(120).withWidth(200));

    createMappingButton.setBounds(usableArea.withX(positionVisualizer.getRight() + 10).withHeight(30).withWidth(150));
    addPluginButton.setBounds(createMappingButton.getBounds().withY(createMappingButton.getBottom() + 10));

    mappingViewport.setBounds(usableArea.withTrimmedTop(positionVisualizer.getHeight() + 10));
}
