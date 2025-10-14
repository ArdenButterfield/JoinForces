//
// Created by Arden on 2025-10-14.
//

#include "ParameterRow.h"

ParameterRow::ParameterRow(juce::AudioProcessorParameter &param) : parameter(param) {

}

ParameterRow::~ParameterRow() {

}

void ParameterRow::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkgreen);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 3);
    g.drawText(parameter.getName(1024), getLocalBounds(), juce::Justification::left);
}

void ParameterRow::resized() {

}

