//
// Created by Arden on 2025-10-12.
//

#ifndef JOINFORCES_MAPPINGSPANEL_H
#define JOINFORCES_MAPPINGSPANEL_H

#include "juce_gui_basics/juce_gui_basics.h"

#include <vector>
#include <memory>

class MappingCenter;
class SliderColumn;
class ParameterNamesColumn;
class PositionsVisualizer;

class MappingsPanel : public juce::Component, public juce::Timer, public juce::Button::Listener {
public:
    explicit MappingsPanel(MappingCenter&);
    ~MappingsPanel() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    void buttonClicked(juce::Button *) override;
    void buttonStateChanged(juce::Button *) override;
    void timerCallback() override;
    std::unique_ptr<SliderColumn> currentColumn;
    std::vector<std::unique_ptr<SliderColumn>> mappingPointColumns;
    std::unique_ptr<ParameterNamesColumn> parameterNamesColumn;
    MappingCenter& mappingCenter;
    std::unique_ptr<PositionsVisualizer> positionsVisualizer;
    void setDoingAnythings();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingsPanel)
};


#endif //JOINFORCES_MAPPINGSPANEL_H