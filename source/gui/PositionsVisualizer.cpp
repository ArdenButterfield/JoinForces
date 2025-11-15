//
// Created by Arden on 2025-11-06.
//

#include "PositionsVisualizer.h"

#include "../MappingCenter.h"
#include "juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"

PositionsVisualizer::PositionsVisualizer(MappingCenter& mc) : mappingCenter(mc) {
    startTimerHz(30);
}

PositionsVisualizer::~PositionsVisualizer() {
}

void PositionsVisualizer::paint(juce::Graphics &g) {
    auto currentPos = mappingCenter.getCurrentMapping().position;

    g.fillAll(juce::Colours::black);

    const auto gridResolution = 5.f;
    auto colour = getColour(currentPos);
    for (float x = 0; x < getWidth(); x += gridResolution) {
        for (float y = 0; y < getHeight(); y += gridResolution) {
            auto force = mappingCenter.calculateMappingPointAttractionForce(
                {(x + gridResolution * 0.5f) / getWidth(), (y + gridResolution * 0.5f) / getHeight(), currentPos.z});
            auto l = force.length();
            g.setColour(colour.withBrightness(l));
            int w = std::min(static_cast<int>(x + gridResolution), getWidth());
            int h = std::min(static_cast<int>(y + gridResolution), getHeight());
            g.drawRect(static_cast<int>(x), static_cast<int>(y), w, h);
        }
    }

    g.setColour(juce::Colours::white);

    g.drawRect(getLocalBounds(), 3);


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
    auto y = 1 - position.y; // in juce, positive y direction is down

    x *= usableBounds.getWidth();
    y *= usableBounds.getHeight();
    x += usableBounds.getX();
    y += usableBounds.getY();

    g.setColour(getColour(position));

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

juce::Colour PositionsVisualizer::getColour(juce::Vector3D<float> &position) {
    return juce::Colours::red.withHue(position.z * 0.6f);
}
