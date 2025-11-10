//
// Created by Arden on 2025-10-12.
//

#ifndef JOINFORCES_PLUGINPARAMETERSPANEL_H
#define JOINFORCES_PLUGINPARAMETERSPANEL_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

#include "ParameterRow.h"
#include "SubpluginWindow.h"

#include "../MappingCenter.h"

class PluginParametersPanel : public juce::Component {
public:
    PluginParametersPanel(PluginParameterSet&);
    ~PluginParametersPanel() override;
    void resized() override;
    void paint(juce::Graphics &g) override;
    void updateDisplay();

    std::vector<std::unique_ptr<ParameterRow>> parameterRows;

private:
    PluginParameterSet& parameterSet;

    juce::TextButton openEditorButton;
    const juce::AudioProcessorGraph::NodeID nodeID;
    std::unique_ptr<SubpluginWindow> subpluginWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginParametersPanel)
};


#endif //JOINFORCES_PLUGINPARAMETERSPANEL_H