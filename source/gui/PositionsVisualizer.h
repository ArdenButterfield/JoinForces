//
// Created by Arden on 2025-11-06.
//

#ifndef JOINFORCES_POSITIONSVISUALIZER_H
#define JOINFORCES_POSITIONSVISUALIZER_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_opengl/juce_opengl.h"

class MappingCenter;

class PositionsVisualizer : public juce::Component, public juce::Timer {
public:
    PositionsVisualizer(MappingCenter&);
    ~PositionsVisualizer();
private:
    MappingCenter& mappingCenter;
    void paint(juce::Graphics &g) override;
    void drawSymbol(juce::Graphics &g, juce::Vector3D<float>& position, bool isCurrentPosition);
    void resized() override;
    void timerCallback() override;
    juce::Rectangle<int> usableBounds;
};


#endif //JOINFORCES_POSITIONSVISUALIZER_H