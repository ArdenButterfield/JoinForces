//
// Created by Arden on 2025-10-12.
//

#include "MappingsPanel.h"

MappingsPanel::MappingsPanel(PluginGroup& _group) : group(_group) {
    group.addListener(this);
}

MappingsPanel::~MappingsPanel() {
    group.removeListener(this);
}

void MappingsPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 5, 3);
}

void MappingsPanel::resized() {
    Component::resized();
}

void MappingsPanel::updateDisplay() {
    for (auto& panel : pluginPanels) {
        removeChildComponent(panel.get());
    }
    pluginPanels.clear();
    for (auto& id : group.getNodes()) {
        pluginPanels.push_back(std::make_unique<PluginParametersPanel>(id, group.getAudioProcessorGraph()));
    }
    for (auto& panel : pluginPanels) {
        addAndMakeVisible(panel.get());
    }
    resized();
}

void MappingsPanel::groupUpdated(const PluginGroup &_group) {
    updateDisplay();
}
