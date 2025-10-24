//
// Created by Arden on 2025-10-18.
//

#include "PositionVisualizer.h"

#include "../ForceFeedbackInterface.h"

PositionVisualizer::PositionVisualizer(ForceFeedbackInterface &_ffinterface) : ffInterface(_ffinterface) {
    startTimerHz(60);

    for (auto& slider : sliders) {
        slider.setSliderStyle(juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(slider);
    }
}

PositionVisualizer::~PositionVisualizer() {

}

void PositionVisualizer::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::pink);
    g.drawRect(getLocalBounds(), 3);
}

void PositionVisualizer::resized() {
    auto usablebounds = getLocalBounds().withSizeKeepingCentre(getWidth() - 20, getHeight() - 20);
    for (int i = 0; i < sliders.size(); ++i) {
        sliders[i].setBounds(usablebounds.withHeight(usablebounds.getHeight() / 3).withY(usablebounds.getHeight() * i / 3 + usablebounds.getY()));
    }
}

void PositionVisualizer::timerCallback() {
    auto pos = ffInterface.getCurrentPosition();
    sliders[0].setValue(pos.x);
    sliders[1].setValue(pos.y);
    sliders[2].setValue(pos.z);
}

