//
// Created by Arden on 2025-10-30.
//

#ifndef JOINFORCES_SLIDERCOLUMN_H
#define JOINFORCES_SLIDERCOLUMN_H

#include "../MappingCenter.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "PositionVisualizer.h"
#include <vector>
#include <memory>

#include "PluginParametersPanel.h"

class SliderColumn : public juce::Component, public juce::Timer {
public:
    SliderColumn(MappingCenter&, MappingCenter::MappingPoint&);
    ~SliderColumn();
    void resized() override;
    void paint(juce::Graphics &g) override;
    void setEditable(bool);
    void timerCallback() override;
private:
    void rebuildParameterPanels();
    bool editable;
    MappingCenter& mappingCenter;
    MappingCenter::MappingPoint& mappingPoint;
    PositionVisualizer positionVisualizer;
    std::vector<std::unique_ptr<PluginParametersPanel>> parameterPanels;
};


#endif //JOINFORCES_SLIDERCOLUMN_H