//
// Created by Arden on 2025-10-18.
//

#include "PositionVisualizer.h"
#include "JoinForcesLookFeel.h"

PositionVisualizer::PositionVisualizer(juce::Vector3D<float>& p) : position(p) {
    startTimerHz(60);

    for (auto& slider : sliders) {
        slider.setSliderStyle(juce::Slider::LinearHorizontal);
        slider.setRange(0,1);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.addListener(this);
        addAndMakeVisible(slider);
    }

    setSize(JoinForcesLookFeel::getParameterRowWidth(), JoinForcesLookFeel::getPositionVizHeight());
}

PositionVisualizer::~PositionVisualizer() = default;

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
    sliders[0].setValue(position.x, juce::dontSendNotification);
    sliders[1].setValue(position.y, juce::dontSendNotification);
    sliders[2].setValue(position.z, juce::dontSendNotification);
}

void PositionVisualizer::sliderValueChanged(juce::Slider* s) {
    if (s == & sliders[0]) {
        position.x = sliders[0].getValue();
    } else if (s == & sliders[1]) {
        position.y = sliders[1].getValue();
    } else if (s == & sliders[2]) {
        position.z = sliders[2].getValue();
    }
}

