//
// Created by Arden on 2025-10-17.
//

#include "MappingCenter.h"
#include "ForceFeedbackInterface.h"

#include "PluginGroup.h"

MappingCenter::MappingCenter(PluginGroup &_group, ForceFeedbackInterface &_interface) : group(_group), ffInterface(_interface) {
}

MappingCenter::~MappingCenter() {
}

void MappingCenter::createMappingAtCurrentState() {
    auto newMapping = MappingPoint();
    newMapping.position = ffInterface.getCurrentPosition();
    for (auto nodeId : group.getNodes()) {
        auto processor = group.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
        PluginParameterSet parameterSet = {*processor, {}};
        for (auto& parameter : processor->getParameters()) {
            if (parameter->isAutomatable()) {
                parameterSet.parameters.push_back({parameter->getValue(), *parameter});
            }
        }
        newMapping.pluginParameters.push_back(parameterSet);
    }
}

void MappingCenter::processBlock() {
    currentMapping.position = ffInterface.getCurrentPosition();

    calculateCurrentMapping();

    for (auto& plugin : currentMapping.pluginParameters) {
        for (auto& param : plugin.parameters) {
            param.parameter.setValue(param.value);
        }
    }
}

void MappingCenter::calculateCurrentMapping() {
    for (auto& plugin : currentMapping.pluginParameters) {
        for (auto& param : plugin.parameters) {
            param.value = 0.5; // TODO: actual algorithm!
        }
    }
}

