#include "PluginEditor.h"

#include "gui/JoinForcesLookFeel.h"

#include <iostream>

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), mappingsPanel (*p.getMappingCenter()), forceAmountSliders (*p.getMappingCenter())
{
    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    for (auto button : {&addPluginButton, &inspectButton, &exportToClipboardButton, &importFromClipboardButton}) {
        button->setColour (juce::TextButton::buttonColourId,    JoinForcesLookFeel::getBackgroundColour());
        button->setColour(juce::TextButton::buttonOnColourId, JoinForcesLookFeel::getShadowColour());
        button->setColour (juce::TextButton::textColourOffId, JoinForcesLookFeel::getTextColour());
        button->setColour (juce::TextButton::textColourOnId, JoinForcesLookFeel::getTextColour());
    }

    addAndMakeVisible (forceAmountSliders);
    addAndMakeVisible (mappingViewport);
    mappingViewport.setViewedComponent(&mappingsPanel, false);

    addAndMakeVisible(addPluginButton);
    addPluginButton.setButtonText("Add plugin");

    addAndMakeVisible(createMappingButton);
    createMappingButton.setButtonText("Create mapping");

    addAndMakeVisible(importFromClipboardButton);
    addAndMakeVisible(exportToClipboardButton);

    // this chunk of code instantiates and opens the melatonin inspector
    addPluginButton.onClick = [&] {
        fileChooser = std::make_unique<juce::FileChooser> ("Select an audio plugin",
#if JUCE_WINDOWS
                                       juce::File::getSpecialLocation (juce::File::globalApplicationsDirectory).getChildFile("Common Files").getChildFile("VST3"),
#else
                                       juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory),
#endif
                                       "*");

        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories | juce::FileBrowserComponent::canSelectFiles;

        fileChooser->launchAsync (folderChooserFlags, [this] (const juce::FileChooser& chooser)
        {
            juce::File file (chooser.getResult());

            juce::String errorMessage;
            processorRef.getMappingCenter()->addPlugin(file, errorMessage);
            if (errorMessage != "") {
                std::cout << "error in adding plugin:" << errorMessage << "\n";
            }
        });
    };
    createMappingButton.onClick = [&] {
        processorRef.getMappingCenter()->createMappingAtCurrentState();
    };

    importFromClipboardButton.onClick = [&] {
        auto element = juce::XmlDocument::parse(juce::SystemClipboard::getTextFromClipboard());
        if (element.get() != nullptr) {
            processorRef.getMappingCenter()->importFromXml(*element);
        }
    };

    exportToClipboardButton.onClick = [&] {
        auto element = juce::XmlElement("state");
        processorRef.getMappingCenter()->exportToXml(element);
        juce::SystemClipboard::copyTextToClipboard(element.toString());
    };

    inputEnabledButton.addListener(this);
    addAndMakeVisible(inputEnabledButton);
    inputEnabledButton.setButtonText("Gestural Input Enabled");
    inputEnabledButton.setColour(juce::ToggleButton::ColourIds::textColourId, JoinForcesLookFeel::getTextColour());
    inputEnabledButton.setColour(juce::ToggleButton::ColourIds::tickColourId, JoinForcesLookFeel::getTextColour());


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (processorRef.editorRememberedSize.getX(), processorRef.editorRememberedSize.getY());

    startTimerHz(30);

    setResizable (true, true);
}

PluginEditor::~PluginEditor()
{
    mappingViewport.setViewedComponent(nullptr);
    processorRef.editorRememberedSize.setXY(getWidth(), getHeight());

}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (JoinForcesLookFeel::getBackgroundColour());

    g.setColour(JoinForcesLookFeel::getShadowColour());
    g.fillRect(mappingViewport.getBounds());
}

void PluginEditor::resized()
{
    inspectButton.setBounds({0,0,150, 40});
    importFromClipboardButton.setBounds(inspectButton.getBounds().withX(inspectButton.getRight() + 10));
    exportToClipboardButton.setBounds(importFromClipboardButton.getBounds().withX(importFromClipboardButton.getRight() + 10));
    inputEnabledButton.setBounds(exportToClipboardButton.getBounds().withX(exportToClipboardButton.getRight() + 10));

    auto usableArea = getLocalBounds().withSizeKeepingCentre(getWidth() - 20, getHeight() - 20).withTop(inspectButton.getBottom() + 10);

    createMappingButton.setBounds(usableArea.withHeight(30).withWidth(150));
    addPluginButton.setBounds(createMappingButton.getBounds().withY(createMappingButton.getBottom() + 10));

    mappingViewport.setBounds(usableArea.withTrimmedTop(addPluginButton.getBottom() + 10));

    forceAmountSliders.setBounds (getLocalBounds()
        .withLeft (createMappingButton.getRight() + 10)
        .withBottom (mappingViewport.getY() - 10)
        .withTop (importFromClipboardButton.getBottom() + 10)
        .withRight (inputEnabledButton.getRight()));
}

void PluginEditor::buttonClicked(juce::Button *) {
}

void PluginEditor::buttonStateChanged(juce::Button *button) {
    processorRef.getMappingCenter()->inputEnabled = inputEnabledButton.getToggleState();

}

void PluginEditor::timerCallback() {
    inputEnabledButton.setToggleState(processorRef.getMappingCenter()->inputEnabled, juce::dontSendNotification);
}
