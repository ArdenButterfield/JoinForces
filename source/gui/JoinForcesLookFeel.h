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
        return 100;
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
};


#endif //JOINFORCES_JOINFORCESLOOKFEEL_H