//
// Created by Arden on 2025-10-30.
//

#include "SliderColumn.h"
#include "JoinForcesLookFeel.h"

SliderColumn::SliderColumn(MappingCenter& mc, MappingPoint& mp, int ci)
: editable(false), mappingCenter(mc), mappingPoint(mp), positionSliders(mappingPoint.position), columnIndex(ci), isCurrentParamColumn(columnIndex == 0) {
    addAndMakeVisible(positionSliders);
    rebuildParameterPanels();
    startTimerHz(60);
}

SliderColumn::~SliderColumn() = default;

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
    if (isCurrentParamColumn) {
        g.fillAll(juce::Colours::darkblue);
    } else {
        g.fillAll(juce::Colours::darkgreen.withSaturation(mappingPoint.contributionWeight));
    }
}

void SliderColumn::setEditable(bool) {
    
}

void SliderColumn::timerCallback() {
    rebuildParameterPanels();
    repaint();
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
