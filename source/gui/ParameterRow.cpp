//
// Created by Arden on 2025-10-14.
//

#include "ParameterRow.h"
#include "JoinForcesLookFeel.h"

ParameterRow::ParameterRow(Parameter& p) : parameter(p), currentValueSlider(p), isDoingAnything((true)) {
    addAndMakeVisible(currentValueSlider);
    setSize(JoinForcesLookFeel::getParameterRowWidth(), JoinForcesLookFeel::getParameterRowHeight());
}

ParameterRow::~ParameterRow() = default;

void ParameterRow::paint(juce::Graphics &g) {
    auto colour = parameter.parameter.isDiscrete() ? juce::Colours::darkgreen : juce::Colours::brown;
    colour = isDoingAnything ? colour : colour.withSaturation(0.1f);
    g.fillAll(colour);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 3);
}

void ParameterRow::resized() {
    currentValueSlider.setBounds(getLocalBounds());
}

void ParameterRow::setIsDoingAnything(bool newValue) {
    if (newValue != isDoingAnything) {
        isDoingAnything = newValue;
        repaint();
    }
}
