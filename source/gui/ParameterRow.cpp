//
// Created by Arden on 2025-10-14.
//

#include "ParameterRow.h"

ParameterRow::ParameterRow(MappingCenter::Parameter& p) : parameter(p) {
    setSliderStyle(juce::Slider::LinearHorizontal);
    setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 40);
    setRange(0, 1, 1.0 / (parameter.getNumSteps() - 1.0));
}

ParameterRow::~ParameterRow() = default;

void ParameterRow::paint(juce::Graphics &g) {
    g.fillAll(parameter.isDiscrete() ? juce::Colours::darkgreen : juce::Colours::brown);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 3);
    g.drawText(parameter.getName(1024), getLocalBounds().withTrimmedLeft(10), juce::Justification::left);
}

void ParameterRow::resized() {
    currentParamSlider.setBounds(getLocalBounds().withTrimmedLeft(120));
}
