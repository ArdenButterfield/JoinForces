//
// Created by Arden on 2025-10-14.
//

#ifndef JOINFORCES_PARAMETERROW_H
#define JOINFORCES_PARAMETERROW_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"

#include "CurrentParameterValueSlider.h"

#include "../MappingCenter.h"

class ParameterRow : public juce::Component {
public:
    ParameterRow(Parameter&);
    ~ParameterRow() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    Parameter& parameter;
    CurrentParameterValueSlider currentValueSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterRow)
};


#endif //JOINFORCES_PARAMETERROW_H