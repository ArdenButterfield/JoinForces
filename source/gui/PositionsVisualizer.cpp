//
// Created by Arden on 2025-11-06.
//

#include "PositionsVisualizer.h"

#include "../MappingCenter.h"

PositionsVisualizer::PositionsVisualizer(MappingCenter& mc) : mappingCenter(mc) {
    startTimerHz(30);
}

PositionsVisualizer::~PositionsVisualizer() {
}

void PositionsVisualizer::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 3);

    auto currentPos = mappingCenter.getCurrentMapping().position;

    for (auto point : mappingCenter.getMappings()) {
        if (point.position.z <= currentPos.z) {
            drawSymbol(g, point.position, false);
        }
    }
    drawSymbol(g, currentPos, true);

    for (auto point : mappingCenter.getMappings()) {
        if (point.position.z > currentPos.z) {
            drawSymbol(g, point.position, false);
        }
    }
}

void PositionsVisualizer::drawSymbol(juce::Graphics &g, juce::Vector3D<float> &position, bool isCurrentPosition) {
    auto x = position.x;
    auto y = position.y;
    auto hue = position.z;

    x *= usableBounds.getWidth();
    y *= usableBounds.getHeight();
    x += usableBounds.getX();
    y += usableBounds.getY();

    g.setColour(juce::Colours::red.withHue(hue * 0.6));

    if (isCurrentPosition) {
        g.fillRect(x - 8, y - 2, 16.f, 4.f);
        g.fillRect(x - 2, y - 8, 4.f, 16.f);
    } else {
        g.drawEllipse(x - 8, y - 8, 16, 16, 4);
    }
}

void PositionsVisualizer::resized() {
    usableBounds = getLocalBounds().reduced(20, 20).withCentre(getLocalBounds().getCentre());
}

void PositionsVisualizer::timerCallback() {
    repaint();
}
