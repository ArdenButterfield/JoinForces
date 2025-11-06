//
// Created by Arden on 2025-10-18.
//

#ifndef JOINFORCES_POSITIONVISUALIZER_H
#define JOINFORCES_POSITIONVISUALIZER_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_opengl/juce_opengl.h"
#include <array>

class PositionSliders : public juce::Component, public juce::Timer, public juce::Slider::Listener {
public:
    PositionSliders(juce::Vector3D<float>&);
    ~PositionSliders();
    void paint(juce::Graphics &g) override;
    void resized() override;
    void timerCallback() override;
private:
    void sliderValueChanged(juce::Slider *) override;
    std::array<juce::Slider, 3> sliders;
    juce::Vector3D<float>& position;
};

#endif //JOINFORCES_POSITIONVISUALIZER_H
