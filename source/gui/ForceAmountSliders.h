//
// Created by Arden on 2025-11-16.
//

#ifndef JOINFORCES_FORCEAMOUNTSLIDERS_H
#define JOINFORCES_FORCEAMOUNTSLIDERS_H

#include "juce_gui_basics/juce_gui_basics.h"

class MappingCenter;

class ForceAmountSliders : public juce::Component, public juce::Timer, public juce::Slider::Listener
{
public:
    ForceAmountSliders(MappingCenter&);
    ~ForceAmountSliders();
private:
    void resized() override;
    void timerCallback() override;
    void paint(juce::Graphics& g) override;

    juce::Slider amountPointForceSlider;
    juce::Slider amountWallForceSlider;

    juce::Label pointForceLabel;
    juce::Label wallForceLabel;

    void sliderValueChanged(juce::Slider*) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderDragStarted(juce::Slider*) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ForceAmountSliders);
    MappingCenter& mappingCenter;
};

#endif //JOINFORCES_FORCEAMOUNTSLIDERS_H
