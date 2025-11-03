//
// Created by Arden on 2025-11-02.
//

#ifndef JOINFORCES_PARAMETERNAMESCOLUMN_H
#define JOINFORCES_PARAMETERNAMESCOLUMN_H

#include "juce_gui_basics/juce_gui_basics.h"

class MappingCenter;

class ParameterNamesColumn : public juce::Component, public juce::Timer {
public:
    ParameterNamesColumn(MappingCenter&);
    ~ParameterNamesColumn() override;
private:
    MappingCenter& mappingCenter;
    void resized() override;
    void paint(juce::Graphics &g) override;
    void timerCallback() override;
};


#endif //JOINFORCES_PARAMETERNAMESCOLUMN_H