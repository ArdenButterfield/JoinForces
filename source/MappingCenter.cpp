//
// Created by Arden on 2025-10-17.
//

#include "MappingCenter.h"

#include "PluginGroup.h"

MappingCenter::MappingCenter(PluginGroup &_group, ForceFeedbackInterface &_interface) : group(_group), interface(_interface) {
}

MappingCenter::~MappingCenter() {
}

void MappingCenter::createMappingAtCurrentState() {
    auto newMapping = MappingPoint();
    newMapping.position = interface.getCurrentPosition();
    for (auto nodeId : group.getNodes()) {
        auto processor = group.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
        PluginParameterSet parameterSet = {*processor, {}};
        for (auto& parameter : processor->getParameters()) {
            if (parameter->isAutomatable()) {
                parameterSet.parameterValues.push_back(parameter->getValue());
            }
        }
        newMapping.pluginParameters.push_back(parameterSet);
    }
}

