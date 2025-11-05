//
// Created by Arden on 2025-10-12.
//

#include "../MappingCenter.h"
#include "MappingsPanel.h"
#include "SliderColumn.h"
#include "ParameterNamesColumn.h"

#include "JoinForcesLookFeel.h"


MappingsPanel::MappingsPanel(MappingCenter& mc) : mappingCenter(mc) {
    currentColumn = std::make_unique<SliderColumn>(mappingCenter, mappingCenter.getCurrentMapping(), 0);
    addAndMakeVisible(currentColumn.get());

    parameterNamesColumn = std::make_unique<ParameterNamesColumn>(mappingCenter);
    addAndMakeVisible(parameterNamesColumn.get());

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
    int x = 0;
    parameterNamesColumn->setTopLeftPosition(x,0);
    x += JoinForcesLookFeel::getNamesColumnWidth();
    currentColumn->setTopLeftPosition(x,0);
    x += JoinForcesLookFeel::getColumnWidth();
    for (const auto & mappingPointColumn : mappingPointColumns) {
        mappingPointColumn->setTopLeftPosition(x, 0);
        x += JoinForcesLookFeel::getColumnWidth();
    }
}

void MappingsPanel::timerCallback() {
    if (mappingCenter.getMappings().size() != mappingPointColumns.size()) {
        mappingPointColumns.clear();

        int i = 1;
        for (auto& mapping: mappingCenter.getMappings()) {
            mappingPointColumns.push_back(std::make_unique<SliderColumn>(mappingCenter, mapping, i));
            ++i;
        }

        for (auto& column : mappingPointColumns) {
            addAndMakeVisible(column.get());
        }
    }
    setSize(JoinForcesLookFeel::getMappingPanelRequiredWidth(mappingCenter),
        JoinForcesLookFeel::getColumnRequiredHeight(mappingCenter));

}
