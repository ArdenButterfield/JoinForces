//
// Created by Arden on 2025-10-12.
//

#include "../MappingCenter.h"
#include "MappingsPanel.h"
#include "SliderColumn.h"
#include "ParameterNamesColumn.h"
#include "PositionsVisualizer.h"

#include "JoinForcesLookFeel.h"


MappingsPanel::MappingsPanel(MappingCenter& mc) : mappingCenter(mc) {
    currentColumn = std::make_unique<SliderColumn>(mappingCenter, mappingCenter.getCurrentMapping(), 0);
    addAndMakeVisible(currentColumn.get());

    parameterNamesColumn = std::make_unique<ParameterNamesColumn>(mappingCenter);
    addAndMakeVisible(parameterNamesColumn.get());

    positionsVisualizer = std::make_unique<PositionsVisualizer>(mappingCenter);
    addAndMakeVisible(positionsVisualizer.get());

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
    positionsVisualizer->setBounds(parameterNamesColumn->getBounds().withHeight(JoinForcesLookFeel::getPositionVizHeight()));
}

void MappingsPanel::buttonClicked(juce::Button *) {
}

void MappingsPanel::buttonStateChanged(juce::Button*) {
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
    setDoingAnythings();
}

void MappingsPanel::setDoingAnythings() {

    for (int pluginI = 0; pluginI < currentColumn->parameterPanels.size(); ++pluginI) {
        for (int paramI = 0; paramI < currentColumn->parameterPanels[pluginI]->parameterRows.size(); ++paramI) {
            bool doingAnything = false;
            auto v = currentColumn->parameterPanels[pluginI]->parameterRows[paramI]->currentValueSlider.getValue();
            if (mappingPointColumns.size() >= 2) {
                for (auto& column : mappingPointColumns) {
                    if (pluginI < column->parameterPanels.size() &&
                        paramI < column->parameterPanels[pluginI]->parameterRows.size() &&
                        std::abs(v - column->parameterPanels[pluginI]->parameterRows[paramI]->currentValueSlider.getValue()) > 0.005) {

                        doingAnything = true;
                        break;
                    }
                }
            }

            for (auto& column : mappingPointColumns) {
                if (pluginI < column->parameterPanels.size() &&
                    paramI < column->parameterPanels[pluginI]->parameterRows.size()) {
                    column->parameterPanels[pluginI]->parameterRows[paramI]->setIsDoingAnything(doingAnything);
                    }
            }

            currentColumn->parameterPanels[pluginI]->parameterRows[paramI]->setIsDoingAnything(doingAnything);
        }
    }
}
