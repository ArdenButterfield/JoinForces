//
// Created by Arden on 2025-10-30.
//

#ifndef JOINFORCES_JOINFORCESLOOKFEEL_H
#define JOINFORCES_JOINFORCESLOOKFEEL_H

#include "juce_gui_basics/juce_gui_basics.h"

class MappingCenter;

class JoinForcesLookFeel : juce::LookAndFeel_V4 {
public:
    static int getColumnRequiredHeight(MappingCenter& mappingCenter);
    static int getDefaultMargin() {
        return 10;
    }

    static int getPositionVizHeight() {
        return 100;
    }

    static int getSliderWidth() {
        return 140;
    }

    static int getParameterRowWidth() {
        return getSliderWidth() + 100;
    }

    static int getParameterRowHeight() {
        return 15;
    }
};


#endif //JOINFORCES_JOINFORCESLOOKFEEL_H