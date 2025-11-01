//
// Created by Arden on 2025-10-15.
//

#include "CurrentParameterValueSlider.h"

CurrentParameterValueSlider::CurrentParameterValueSlider(Parameter& p) : parameter(p) {
    setSliderStyle(juce::Slider::LinearHorizontal);
    setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 40);
    setRange(0, 1, 1.0 / (parameter.parameter.getNumSteps() - 1.0));
}

CurrentParameterValueSlider::~CurrentParameterValueSlider() {
}

double CurrentParameterValueSlider::getValueFromText(const juce::String &text) {
    return parameter.parameter.getValueForText(text);
}

juce::String CurrentParameterValueSlider::getTextFromValue(double value) {
    return parameter.parameter.getText(static_cast<float>(value), 64);
}

void CurrentParameterValueSlider::timerCallback() {
    setValue(parameter.value);
    updateText();
}

void CurrentParameterValueSlider::resized() {
    Slider::resized();
    setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, getHeight() - 2);
}

void CurrentParameterValueSlider::valueChanged() {
    parameter.value = static_cast<float>(getValue());
}
