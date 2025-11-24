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
    auto colour = parameter.parameter.isDiscrete() ?
        JoinForcesLookFeel::getDiscreteParameterColour() :
        JoinForcesLookFeel::getContinuousParameterColour();
    colour = isDoingAnything ? colour : colour.withAlpha(0.1f);
    currentValueSlider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    currentValueSlider.setColour(juce::Slider::textBoxTextColourId,
        isDoingAnything ? JoinForcesLookFeel::getBackgroundColour() : JoinForcesLookFeel::getTextColour());
    currentValueSlider.setColour(juce::Slider::thumbColourId,
        isDoingAnything ? JoinForcesLookFeel::getBackgroundColour() : JoinForcesLookFeel::getTextColour());
    currentValueSlider.setColour (juce::Slider::trackColourId,
        (isDoingAnything ? JoinForcesLookFeel::getBackgroundColour() : JoinForcesLookFeel::getTextColour()).withAlpha(0.2f));


    g.fillAll(colour);
    g.setColour(JoinForcesLookFeel::getBorderColour());
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
