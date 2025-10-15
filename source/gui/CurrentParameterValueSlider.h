//
// Created by Arden on 2025-10-15.
//

#ifndef JOINFORCES_CURRENTPARAMETERVALUESLIDER_H
#define JOINFORCES_CURRENTPARAMETERVALUESLIDER_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

class CurrentParameterValueSlider : public juce::Slider, public juce::Timer {
public:
    CurrentParameterValueSlider(juce::AudioProcessorParameter& p);
    ~CurrentParameterValueSlider() override;
    double getValueFromText(const juce::String &text) override;
    juce::String getTextFromValue(double value) override;
    void timerCallback() override;
    void resized() override;
    void valueChanged() override;
private:
    juce::AudioProcessorParameter& parameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurrentParameterValueSlider);
};


#endif //JOINFORCES_CURRENTPARAMETERVALUESLIDER_H