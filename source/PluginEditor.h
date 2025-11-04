#pragma once

#include "PluginProcessor.h"
#include "gui/MappingsPanel.h"
#include "gui/PositionVisualizer.h"

#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::TextButton createMappingButton;
private:
    std::unique_ptr<juce::FileChooser> fileChooser;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    MappingsPanel mappingsPanel;
    juce::TextButton addPluginButton;
    juce::Viewport mappingViewport;

    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };
    juce::TextButton exportToClipboardButton { "Export to Clipboard" };
    juce::TextButton importFromClipboardButton { "Import from Clipboard" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
