//
// Created by Arden on 2025-10-17.
//

#include "MappingCenter.h"

#include <utility>
#include "ForceFeedbackInterface.h"

#include "PluginGroup.h"

MappingCenter::MappingCenter(juce::AudioProcessor::BusesLayout layout, ForceFeedbackInterface &_interface) : pluginGroup(std::move(layout)), ffInterface(_interface) {
}

MappingCenter::~MappingCenter() {
}

void MappingCenter::createMappingAtCurrentState() {
    criticalSection.enter();
    auto newMapping = MappingPoint();
    newMapping.position = ffInterface.getCurrentPosition();
    for (auto plugin : currentMapping.pluginParameters) {
        PluginParameterSet parameterSet = {plugin.processor, plugin.file, {}};
        for (auto& p : plugin.parameters) {
            parameterSet.parameters.push_back({p.value, p.parameter});
        }
    }
    mappings.push_back(newMapping);
    criticalSection.exit();
}

void MappingCenter::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages) {
    if (ffInterface.isInitialized()) {
        currentMapping.position = ffInterface.getCurrentPosition();
    }

    calculateCurrentMapping();

    for (auto& plugin : currentMapping.pluginParameters) {
        for (auto& param : plugin.parameters) {
            param.parameter.setValue(param.value);
        }
    }

    pluginGroup.processBlock(buffer, midiMessages);
}

int MappingCenter::addPlugin(const juce::File &file, juce::String &errorMessage) {
    auto res = pluginGroup.addPlugin(file, errorMessage);
    if (res == 0) {
        insertInto(currentMapping, file);
        for (auto& mapping : mappings) {
            insertInto(mapping, file);
        }
    }
    return res;
}

/*
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
*/

void MappingCenter::insertInto(MappingPoint &mapping, const juce::File& file) {
    criticalSection.enter();
    auto it = mapping.pluginParameters.begin();
    for (auto nodeId : pluginGroup.getNodes()) {
        auto processor = pluginGroup.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
        if (it == mapping.pluginParameters.end() || processor != &(it->processor)) {
            PluginParameterSet parameterSet = {*processor, file, {}};
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
    for (auto nodeId : pluginGroup.getNodes()) {
        auto processor = pluginGroup.getAudioProcessorGraph().getNodeForId(nodeId)->getProcessor();
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

void MappingCenter::exportToXml(juce::XmlElement &xml) {
    auto mapping = xml.createNewChildElement("mapping");
    auto currentState = mapping->createNewChildElement("current-state");
    auto currentPosition = currentState->createNewChildElement("position");
    currentPosition->setAttribute("x", currentMapping.position.x);
    currentPosition->setAttribute("y", currentMapping.position.y);
    currentPosition->setAttribute("z", currentMapping.position.z);
    auto currentPlugins = currentState->createNewChildElement("plugins");
    for (auto& plugin : currentMapping.pluginParameters) {
        auto p = currentPlugins->createNewChildElement("plugin");
        auto par = p->createNewChildElement("params");
        for (auto& param: plugin.parameters) {
            par->setAttribute(juce::String(param.parameter.getParameterIndex()), param.value);
        }
    }
    auto mapPositions = mapping->createNewChildElement("map-positions");
    for (auto& mapPoint : mappings) {
        auto mp = mapPositions->createNewChildElement("map-point");
        auto mpPosition = mp->createNewChildElement("position");
        mpPosition->setAttribute("x", mapPoint.position.x);
        mpPosition->setAttribute("y", mapPoint.position.y);
        mpPosition->setAttribute("z", mapPoint.position.z);
        auto mpPplugins = mpPosition->createNewChildElement("plugins");
        for (auto& plugin : mapPoint.pluginParameters) {
            auto p = mpPplugins->createNewChildElement("plugin");
            auto par = p->createNewChildElement("params");
            for (auto& param : plugin.parameters) {
                par->setAttribute(juce::String(param.parameter.getParameterIndex()), param.value);
            }
        }
    }
}

void MappingCenter::importFromXml(const juce::XmlElement &xml) {
    criticalSection.enter();

    criticalSection.exit();
}

