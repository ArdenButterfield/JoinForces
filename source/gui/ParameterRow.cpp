//
// Created by Arden on 2025-10-14.
//

#include "ParameterRow.h"

ParameterRow::ParameterRow(juce::AudioProcessorParameter &param) : parameter(param), currentParamSlider(param) {
    addAndMakeVisible(currentParamSlider);
}

ParameterRow::~ParameterRow() {

}

void ParameterRow::paint(juce::Graphics &g) {
    g.fillAll(parameter.isDiscrete() ? juce::Colours::darkgreen : juce::Colours::brown);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 3);
    g.drawText(parameter.getName(1024), getLocalBounds().withTrimmedLeft(10), juce::Justification::left);
}

void ParameterRow::resized() {
    currentParamSlider.setBounds(getLocalBounds().withTrimmedLeft(100));
}
