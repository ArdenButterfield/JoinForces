//
// Created by Arden on 2025-10-30.
//

#include "CurrentStateColumn.h"
#include "../MappingCenter.h"

CurrentStateColumn::CurrentStateColumn(MappingCenter& mc) : mappingCenter(mc), positionVisualizer (mappingCenter.ffInterface) {
    mappingCenter.group.addListener(this);
    addAndMakeVisible(positionVisualizer);
}

CurrentStateColumn::~CurrentStateColumn() {
    mappingCenter.group.removeListener(this);
}

void CurrentStateColumn::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkmagenta);
    g.setColour(juce::Colours::magenta);
    g.drawRect(getLocalBounds(), 3);
}

void CurrentStateColumn::resized() {
    positionVisualizer.setBounds(getLocalBounds().withHeight(40));

    int y = 0;
    for (auto& panel : pluginPanels) {
        auto panelDesiredHeight = panel->getDesiredHeight();
        panel->setBounds(getLocalBounds().withY(y).withHeight(panelDesiredHeight));
        y += panelDesiredHeight;
    }
}

void CurrentStateColumn::groupUpdated(const PluginGroup &group) {
    juce::ignoreUnused(group);
    updateDisplay();

}

void CurrentStateColumn::updateDisplay() {
    for (auto& panel : pluginPanels) {
        removeChildComponent(panel.get());
    }
    pluginPanels.clear();
    for (auto& id : mappingCenter.group.getNodes()) {
        pluginPanels.push_back(std::make_unique<PluginParametersPanel>(id, mappingCenter.group.getAudioProcessorGraph()));
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
