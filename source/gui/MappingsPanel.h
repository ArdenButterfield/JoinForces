//
// Created by Arden on 2025-10-12.
//

#ifndef JOINFORCES_MAPPINGSPANEL_H
#define JOINFORCES_MAPPINGSPANEL_H

#include "PluginParametersPanel.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "../PluginGroup.h"

#include <vector>
#include <memory>

class MappingsPanel : public juce::Component, public PluginGroup::Listener {
public:
    MappingsPanel(PluginGroup& group);
    ~MappingsPanel() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
    void updateDisplay();
    void groupUpdated(const PluginGroup &group) override;
private:
    std::vector<std::unique_ptr<PluginParametersPanel>> pluginPanels;
    PluginGroup& group;
};


#endif //JOINFORCES_MAPPINGSPANEL_H