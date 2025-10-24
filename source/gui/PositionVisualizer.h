//
// Created by Arden on 2025-10-18.
//

#ifndef JOINFORCES_POSITIONVISUALIZER_H
#define JOINFORCES_POSITIONVISUALIZER_H

#include "juce_gui_basics/juce_gui_basics.h"
#include <array>

class ForceFeedbackInterface;

class PositionVisualizer : public juce::Component, public juce::Timer {
public:
    PositionVisualizer(ForceFeedbackInterface& ffInterface);
    ~PositionVisualizer();
    void paint(juce::Graphics &g) override;
    void resized() override;
    void timerCallback() override;
private:
    std::array<juce::Slider, 3> sliders;
    ForceFeedbackInterface& ffInterface;
};

#endif //JOINFORCES_POSITIONVISUALIZER_H
