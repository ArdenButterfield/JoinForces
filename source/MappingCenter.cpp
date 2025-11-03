//
// Created by Arden on 2025-10-17.
//

#include "MappingCenter.h"
#include "ForceFeedbackInterface.h"

#include "PluginGroup.h"

MappingCenter::MappingCenter(PluginGroup &_group, ForceFeedbackInterface &_interface) : group(_group), ffInterface(_interface) {
    group.addListener(this);
}

MappingCenter::~MappingCenter() {
    group.removeListener(this);
}

void MappingCenter::createMappingAtCurrentState() {
    criticalSection.enter();
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
    mappings.push_back(newMapping);
    criticalSection.exit();
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


void MappingCenter::groupUpdated(const PluginGroup &g) {

    if (&g != &group) {
        return;
    }

    auto numPlugins = 0;

    for (auto nodeId : group.getNodes()) {
        auto processor = group.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
        for (auto& parameter : processor->getParameters()) {
            if (parameter->isAutomatable()) {
                numPlugins++;
            }
        }
    }

    if (numPlugins > mappings.size() + 1) {
        insertInto(currentMapping);
        for (auto& mapping : mappings) {
            insertInto(mapping);
        }
    } else if (numPlugins < mappings.size() - 1) {
        removeFrom(currentMapping);
        for (auto& mapping : mappings) {
            removeFrom(mapping);
        }
    }
}

void MappingCenter::insertInto(MappingPoint &mapping) {
    criticalSection.enter();
    auto it = mapping.pluginParameters.begin();
    for (auto nodeId : group.getNodes()) {
        auto processor = group.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
        if (it == mapping.pluginParameters.end() || processor != &(it->processor)) {
            PluginParameterSet parameterSet = {*processor, {}};
            for (auto& parameter : processor->getParameters()) {
                if (parameter->isAutomatable()) {
                    parameterSet.parameters.push_back({parameter->getValue(), *parameter});
                }
            }
            it = mapping.pluginParameters.insert(it, parameterSet);
        }
        ++it;
    }
}

void MappingCenter::removeFrom(MappingPoint &mapping) {
    criticalSection.enter();
    auto it = mapping.pluginParameters.begin();
    for (auto nodeId : group.getNodes()) {
        auto processor = group.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
        if (processor != &(it->processor)) {
            it = mapping.pluginParameters.erase(it);
        } else {
            ++it;
        }
    }
    criticalSection.exit();
}

void MappingCenter::calculateCurrentMapping() {
    for (auto& plugin : currentMapping.pluginParameters) {
        for (auto& param : plugin.parameters) {
            param.value = 0.5; // TODO: actual algorithm!
        }
    }
}

