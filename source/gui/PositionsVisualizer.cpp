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
    auto currentPos = mappingCenter.getCurrentMapping().position;

    g.fillAll(juce::Colours::black);

    constexpr auto gridResolution = 2;
    const auto invW = 1.f / static_cast<float>(usableBounds.getWidth());
    const auto invH = 1.f / static_cast<float>(usableBounds.getHeight());
    auto colour = getColour(currentPos);
    for (int x = 0; x < usableBounds.getWidth(); x += gridResolution) {
        for (int y = 0; y < usableBounds.getHeight(); y += gridResolution) {
            auto probingPosition = juce::Vector3D<float>((static_cast<float>(x) + gridResolution * 0.5f) * invW,
                    (static_cast<float>(y) + gridResolution * 0.5f) * invH,
                    currentPos.z);

            auto pushbackForce = mappingCenter.calculateWallPushbackForce(probingPosition);
            auto attractionForce = mappingCenter.calculateMappingPointAttractionForce (probingPosition);

            g.setColour(juce::Colours::white.withBrightness(pushbackForce.length() * 0.1));

            int w = std::min((x + gridResolution), usableBounds.getWidth()) - x;
            int h = std::min((y + gridResolution), usableBounds.getHeight()) - y;

            g.fillRect(x + usableBounds.getX(), usableBounds.getBottom() - y - gridResolution, w, h);
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
    auto sidelengh = std::min(getWidth(), getHeight());
    usableBounds = getLocalBounds().withSizeKeepingCentre (sidelengh, sidelengh);
}

void PositionsVisualizer::timerCallback() {
    repaint();
}

juce::Colour PositionsVisualizer::getColour(juce::Vector3D<float> &position) {
    return juce::Colours::red.withHue(position.z * 0.6f);
}
