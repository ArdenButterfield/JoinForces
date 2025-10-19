#pragma once

#include "PluginProcessor.h"
#include "gui/MappingsPanel.h"
#include "gui/PositionVisualizer.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<juce::FileChooser> fileChooser;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    MappingsPanel mappingsPanel;
    juce::TextButton addPluginButton;
    juce::Viewport mappingViewport;
    juce::TextButton createMappingButton;
    PositionVisualizer positionVisualizer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
