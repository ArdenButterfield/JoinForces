//
// Created by Arden on 2025-10-15.
//

#include "CurrentParameterValueSlider.h"

CurrentParameterValueSlider::CurrentParameterValueSlider(juce::AudioProcessorParameter &p) : parameter(p) {
    setSliderStyle(juce::Slider::LinearHorizontal);
    setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, 40);
    setRange(0, 1, 1.0 / (parameter.getNumSteps() - 1.0));
}

CurrentParameterValueSlider::~CurrentParameterValueSlider() {
}

double CurrentParameterValueSlider::getValueFromText(const juce::String &text) {
    return parameter.getValueForText(text);
}

juce::String CurrentParameterValueSlider::getTextFromValue(double value) {
    return parameter.getText(value, 64);
}

void CurrentParameterValueSlider::timerCallback() {
    setValue(parameter.getValue());
    updateText();
}

void CurrentParameterValueSlider::resized() {
    Slider::resized();
    setTextBoxStyle(juce::Slider::TextBoxRight, true, 100, getHeight() - 2);
}

void CurrentParameterValueSlider::valueChanged() {
    parameter.setValue(getValue());
}
