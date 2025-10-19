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
    int y = 0;
    for (auto& panel : pluginPanels) {
        auto panelDesiredHeight = panel->getDesiredHeight();
        panel->setBounds(getLocalBounds().withY(y).withHeight(panelDesiredHeight));
        y += panelDesiredHeight;
    }
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
    auto neededHeight = 0;
    for (auto& panel : pluginPanels) {
        neededHeight += panel->getDesiredHeight();
    }
    setSize(300, neededHeight);
}

void MappingsPanel::groupUpdated(const PluginGroup &_group) {
    updateDisplay();
}
