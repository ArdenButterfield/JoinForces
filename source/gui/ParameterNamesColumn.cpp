//
// Created by Arden on 2025-11-02.
//

#include "ParameterNamesColumn.h"
#include "JoinForcesLookFeel.h"
#include "../MappingCenter.h"

ParameterNamesColumn::ParameterNamesColumn(MappingCenter& mc) : mappingCenter(mc) {
    startTimerHz(60);
}

ParameterNamesColumn::~ParameterNamesColumn() {
}

void ParameterNamesColumn::resized() {

}

void ParameterNamesColumn::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::lightgrey);
    g.setColour(juce::Colours::black);
    auto y = JoinForcesLookFeel::getPositionVizHeight();
    auto currentMapping = mappingCenter.getCurrentMapping();
    auto textBounds = juce::Rectangle<int>(
        JoinForcesLookFeel::getDefaultMargin(), 0,
        getWidth() - 2 * JoinForcesLookFeel::getDefaultMargin(),
        JoinForcesLookFeel::getParameterRowHeight());

    for (auto& plugin : currentMapping.pluginParameters) {
        int yy = 0;
        for (auto& parameter : plugin.parameters) {
            g.drawText(parameter.parameter.getName(1024), textBounds.withY(y + yy), juce::Justification::right);
            yy += JoinForcesLookFeel::getParameterRowHeight();
        }
        y += JoinForcesLookFeel::getProcessorRequiredHeight(plugin);
    }
}

void ParameterNamesColumn::timerCallback() {
    auto h = JoinForcesLookFeel::getColumnRequiredHeight(mappingCenter);
    if (h != getHeight()) {
        setSize(JoinForcesLookFeel::getNamesColumnWidth(), h);
    }
}
