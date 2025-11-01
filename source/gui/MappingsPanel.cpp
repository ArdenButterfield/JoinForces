//
// Created by Arden on 2025-10-12.
//

#include "../MappingCenter.h"
#include "MappingsPanel.h"
#include "SliderColumn.h"

#include "JoinForcesLookFeel.h"


MappingsPanel::MappingsPanel(MappingCenter& mc) : mappingCenter(mc) {
    currentColumn = std::make_unique<SliderColumn>(mappingCenter, mappingCenter.getCurrentMapping());
    addAndMakeVisible(currentColumn.get());
    setSize(JoinForcesLookFeel::getMappingPanelRequiredWidth(mappingCenter),
            JoinForcesLookFeel::getColumnRequiredHeight(mappingCenter));
    startTimerHz(30);
}

MappingsPanel::~MappingsPanel() {

}

void MappingsPanel::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 5, 3);
}

void MappingsPanel::resized() {
    currentColumn->setTopLeftPosition(0,0);
    for (int i = 0; i < mappingPointColumns.size(); i++) {
        mappingPointColumns[i]->setTopLeftPosition(JoinForcesLookFeel::getColumnWidth() * (i + 1), 0);
    }
}

void MappingsPanel::timerCallback() {
    if (mappingCenter.getMappings().size() != mappingPointColumns.size()) {
        mappingPointColumns.clear();

        for (auto& mapping: mappingCenter.getMappings()) {
            mappingPointColumns.push_back(std::make_unique<SliderColumn>(mappingCenter, mapping));
        }

        for (auto& column : mappingPointColumns) {
            addAndMakeVisible(column.get());
        }
    }
    setSize(JoinForcesLookFeel::getMappingPanelRequiredWidth(mappingCenter),
        JoinForcesLookFeel::getColumnRequiredHeight(mappingCenter));

}
