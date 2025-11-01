//
// Created by Arden on 2025-10-30.
//

#include "SliderColumn.h"

#include "public.sdk/source/vst/vstparameters.h"

SliderColumn::SliderColumn(MappingCenter& mc, MappingCenter::MappingPoint& mp)
: editable(false), mappingCenter(mc), mappingPoint(mp), positionVisualizer(mappingPoint.position) {
    addAndMakeVisible(positionVisualizer);
    rebuildParameterPanels();
    startTimerHz(60);
}

SliderColumn::~SliderColumn() {
}

void SliderColumn::resized() {
    Component::resized();
}

void SliderColumn::paint(juce::Graphics &g) {
    Component::paint(g);
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

    for (int i = 0; i < mappingPoint.pluginParameters.size(); i++) {
        parameterPanels.emplace_back(std::make_unique<PluginParametersPanel>(mappingPoint.pluginParameters[i]));
    }

    for (auto& p : parameterPanels) {
        addAndMakeVisible(p.get());
    }
    resized();
}
