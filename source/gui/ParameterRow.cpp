//
// Created by Arden on 2025-10-14.
//

#include "ParameterRow.h"
#include "JoinForcesLookFeel.h"

ParameterRow::ParameterRow(Parameter& p) : parameter(p), currentValueSlider(p) {
    addAndMakeVisible(currentValueSlider);
    setSize(JoinForcesLookFeel::getParameterRowWidth(), JoinForcesLookFeel::getParameterRowHeight());
}

ParameterRow::~ParameterRow() = default;

void ParameterRow::paint(juce::Graphics &g) {
    g.fillAll(parameter.parameter.isDiscrete() ? juce::Colours::darkgreen : juce::Colours::brown);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 3);
}

void ParameterRow::resized() {
    currentValueSlider.setBounds(getLocalBounds());
}
