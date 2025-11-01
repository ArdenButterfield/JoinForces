//
// Created by Arden on 2025-10-30.
//

#ifndef JOINFORCES_CURRENTSTATECOLUMN_H
#define JOINFORCES_CURRENTSTATECOLUMN_H

#include <vector>
#include <memory>
#include "juce_gui_basics/juce_gui_basics.h"

#include "PositionVisualizer.h"
#include "PluginParametersPanel.h"
#include "../PluginGroup.h"


class MappingCenter;

class CurrentStateColumn : public juce::Component, PluginGroup::Listener {
public:
    CurrentStateColumn(MappingCenter&);
    ~CurrentStateColumn() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    void groupUpdated(const PluginGroup &group) override;
    void updateDisplay();
    MappingCenter& mappingCenter;
    PositionVisualizer positionVisualizer;
    std::vector<std::unique_ptr<PluginParametersPanel>> pluginPanels;
};


#endif //JOINFORCES_CURRENTSTATECOLUMN_H