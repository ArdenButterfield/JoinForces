//
// Created by Arden on 2025-10-12.
//

#ifndef JOINFORCES_PLUGINPARAMETERSPANEL_H
#define JOINFORCES_PLUGINPARAMETERSPANEL_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

#include "ParameterRow.h"

class PluginParametersPanel : public juce::Component {
public:
    PluginParametersPanel(juce::AudioProcessorGraph::NodeID nodeID, juce::AudioProcessorGraph& graph);
    ~PluginParametersPanel();
    void resized() override;
    void paint(juce::Graphics &g) override;
    void updateDisplay();
    int getDesiredHeight() const;
private:
    int parameterRowHeight = 30;
    std::vector<std::unique_ptr<ParameterRow>> parameterRows;
    const juce::AudioProcessorGraph::NodeID nodeID;
    juce::AudioProcessorGraph& graph;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginParametersPanel)
};


#endif //JOINFORCES_PLUGINPARAMETERSPANEL_H