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
    Component::resized();
}

void PluginParametersPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkblue);
    g.setColour(juce::Colours::lightblue);
    g.drawRect(getLocalBounds(), 3);
}

void PluginParametersPanel::updateDisplay() {
    auto processor = graph.getNodeForId(nodeID);
    if (processor) {
        std::cout << "processor params\n";
        for (auto& param : processor->getProcessor()->getParameters()) {
            std::cout << param->getName(1024) << "\n";
        }
    }
}

int PluginParametersPanel::getDesiredHeight() {
    return 20;
}
