//
// Created by Arden on 2025-10-12.
//

#include "PluginParametersPanel.h"

PluginParametersPanel::PluginParametersPanel(
    juce::AudioProcessorGraph::NodeID _nodeID, juce::AudioProcessorGraph& _graph) :
    nodeID(_nodeID), graph(_graph) {
    updateDisplay();
}

PluginParametersPanel::~PluginParametersPanel() {
}

void PluginParametersPanel::resized() {
    for (int i = 0; i < parameterRows.size(); i++) {
        parameterRows[i]->setBounds(getLocalBounds().withHeight(parameterRowHeight).withY(parameterRowHeight * i));
    }
}

void PluginParametersPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkblue);
    g.setColour(juce::Colours::lightblue);
    g.drawRect(getLocalBounds(), 3);
}

void PluginParametersPanel::updateDisplay() {
    for (auto& row : parameterRows) {
        removeChildComponent(row.get());
    }

    auto processor = graph.getNodeForId(nodeID);
    parameterRows.clear();
    if (processor) {
        for (auto& param : processor->getProcessor()->getParameters()) {
            if (param->isAutomatable()) {
                parameterRows.push_back(std::make_unique<ParameterRow>(*param));
            }
        }
        for (auto& row : parameterRows) {
            addAndMakeVisible(row.get());
        }
        resized();
    }
}

int PluginParametersPanel::getDesiredHeight() const {
    return parameterRows.size() * parameterRowHeight;
}
