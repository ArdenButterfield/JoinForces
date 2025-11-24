//
// Created by Arden on 2025-10-30.
//

#ifndef JOINFORCES_JOINFORCESLOOKFEEL_H
#define JOINFORCES_JOINFORCESLOOKFEEL_H

#include "juce_gui_basics/juce_gui_basics.h"

class MappingCenter;
struct PluginParameterSet;

class JoinForcesLookFeel : juce::LookAndFeel_V4 {
public:
    static int getColumnRequiredHeight(MappingCenter& mappingCenter);
    static int getMappingPanelRequiredWidth(MappingCenter& mappingCenter);
    static int getProcessorRequiredHeight(const PluginParameterSet&);

    static int getNamesColumnWidth() {
        return 200;
    }

    static int getDefaultMargin() {
        return 10;
    }

    static int getPositionVizHeight() {
        return 170;
    }

    static int getSliderWidth() {
        return 200;
    }

    static int getParameterRowWidth() {
        return getSliderWidth();
    }

    static int getColumnWidth() {
        return getParameterRowWidth() + getDefaultMargin() * 2;
    }

    static int getParameterRowHeight() {
        return 25;
    }

    static juce::Colour getBackgroundColour()
    {
        return {229, 255, 222};
    }

    static juce::Colour getShadowColour()
    {
        return {187,203,203};
    }

    static juce::Colour getBorderColour()
    {
        return {99, 75, 102};
    }

    static juce::Colour getMidColour()
    {
        return {149,144,168};
    }

    static juce::Colour getTextColour()
    {
        return {24,2,12};
    }

    static juce::Colour getDiscreteParameterColour()
    {
        return juce::Colours::brown;
    }

    static juce::Colour getContinuousParameterColour()
    {
        return juce::Colours::darkgreen;
    }
};


#endif //JOINFORCES_JOINFORCESLOOKFEEL_H