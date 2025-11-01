//
// Created by Arden on 2025-10-30.
//

#include "JoinForcesLookFeel.h"

#include "../MappingCenter.h"
#include "../PluginGroup.h"

int JoinForcesLookFeel::getColumnRequiredHeight(MappingCenter &mappingCenter) {
    int h = 0;
    for (auto& pps : mappingCenter.getCurrentMapping().pluginParameters) {
        h += getProcessorRequiredHeight(pps);
        h += getDefaultMargin();
    }
    return h;
}

int JoinForcesLookFeel::getMappingPanelRequiredWidth(MappingCenter &mappingCenter) {
    return getColumnWidth() * (mappingCenter.getMappings().size() + 1) + getDefaultMargin();
}

int JoinForcesLookFeel::getProcessorRequiredHeight(const PluginParameterSet& pluginParameterSet) {
    return pluginParameterSet.parameters.size() * getParameterRowHeight();
}
