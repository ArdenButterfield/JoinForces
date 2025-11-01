//
// Created by Arden on 2025-10-12.
//

#ifndef JOINFORCES_MAPPINGSPANEL_H
#define JOINFORCES_MAPPINGSPANEL_H

#include "PluginParametersPanel.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "MappingPointParametersPanel.h"
#include "CurrentStateColumn.h"

#include <vector>
#include <memory>

class MappingCenter;

class MappingsPanel : public juce::Component {
public:
    explicit MappingsPanel(MappingCenter&);
    ~MappingsPanel() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    std::unique_ptr<CurrentStateColumn> currentStateColumn;
    std::vector<std::unique_ptr<MappingPointParametersPanel>> mappingPointPanels;
    PluginGroup& group;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingsPanel)
};


#endif //JOINFORCES_MAPPINGSPANEL_H