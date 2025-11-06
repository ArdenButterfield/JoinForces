//
// Created by Arden on 2025-10-30.
//

#include "SliderColumn.h"
#include "JoinForcesLookFeel.h"

SliderColumn::SliderColumn(MappingCenter& mc, MappingPoint& mp, int ci)
: editable(false), mappingCenter(mc), mappingPoint(mp), positionSliders(mappingPoint.position), columnIndex(ci) {
    addAndMakeVisible(positionSliders);
    rebuildParameterPanels();
    startTimerHz(60);
}

SliderColumn::~SliderColumn() {
}

void SliderColumn::resized() {
    int y = 0;
    positionSliders.setTopLeftPosition(0,0);
    y += positionSliders.getHeight();
    for (auto& panel : parameterPanels) {
        panel->setTopLeftPosition(0, y);
        y += panel->getHeight();
    }
}

void SliderColumn::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkgreen);
}

void SliderColumn::setEditable(bool) {
    
}

void SliderColumn::timerCallback() {
    rebuildParameterPanels();
}

void SliderColumn::rebuildParameterPanels() {
    if (mappingPoint.pluginParameters.size() == parameterPanels.size()) {
        return;
    }

    parameterPanels.clear();

    for (auto& p : mappingPoint.pluginParameters) {
        parameterPanels.emplace_back(std::make_unique<PluginParametersPanel>(p));
    }

    for (auto& p : parameterPanels) {
        addAndMakeVisible(p.get());
    }

    setSize(JoinForcesLookFeel::getColumnWidth(), JoinForcesLookFeel::getColumnRequiredHeight(mappingCenter));
}
