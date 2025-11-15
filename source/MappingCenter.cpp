//
// Created by Arden on 2025-10-17.
//

#include "MappingCenter.h"

#include <utility>
#include "ForceFeedbackInterface.h"

#include "PluginGroup.h"
#include "juce_dsp/juce_dsp.h"

MappingCenter::MappingCenter(const juce::AudioProcessor::BusesLayout &layout, ForceFeedbackInterface &_interface) :
        xParam(new juce::AudioParameterFloat(juce::ParameterID{"x", 1}, "x", 0, 1, 0)),
        yParam(new juce::AudioParameterFloat( juce::ParameterID{"y", 1}, "y", 0, 1, 0)),
        zParam(new juce::AudioParameterFloat(juce::ParameterID{"z", 1}, "z", 0, 1, 0)),
        busesLayout(layout), pluginGroup(layout), ffInterface(_interface), inputEnabled(true), mappingPointAttractionForce(0,0,0), wallPushbackForce(0,0,0) {
    xParam->addListener(this);
    yParam->addListener(this);
    zParam->addListener(this);

    ffInterface.setMappingCenter(this);

    startTimerHz(60);
}

MappingCenter::~MappingCenter() {
    xParam->removeListener(this);
    yParam->removeListener(this);
    zParam->removeListener(this);
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
        newMapping.pluginParameters.push_back(parameterSet);
    }
    mappings.push_back(newMapping);
    criticalSection.exit();
}

void MappingCenter::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages) {
    if (ffInterface.isInitialized()) {
        currentMapping.position = ffInterface.getCurrentPosition();
    }
    if (inputEnabled) {
        xParam->beginChangeGesture();
        yParam->beginChangeGesture();
        zParam->beginChangeGesture();

        xParam->setValueNotifyingHost(currentMapping.position.x);
        yParam->setValueNotifyingHost(currentMapping.position.y);
        zParam->setValueNotifyingHost(currentMapping.position.z);

        xParam->endChangeGesture();
        yParam->endChangeGesture();
        zParam->endChangeGesture();
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
    if (res != nullptr) {
        insertInto(currentMapping, file);
        for (auto& mapping : mappings) {
            insertInto(mapping, file);
        }
    }
    return res == nullptr;
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

float MappingCenter::closeness(juce::Vector3D<float> a, juce::Vector3D<float> b) {
    return
        juce::dsp::FastMathApproximations::exp(-((a.x - b.x) * (a.x - b.x))) *
        juce::dsp::FastMathApproximations::exp(-((a.y - b.y) * (a.y - b.y))) *
        juce::dsp::FastMathApproximations::exp(-((a.z - b.z) * (a.z - b.z)));
}


void MappingCenter::calculateCurrentMapping() {
    std::vector<float> distances;
    auto minDistance = 9999999.9f;
    auto minIndex = -1;
    for (auto& mapping : mappings) {
        auto difference = mapping.position - currentMapping.position;
        distances.push_back(difference.lengthSquared());
        if (distances.back() < minDistance) {
            minDistance = distances.back();
            minIndex = distances.size() - 1;
        }
    }

    std::vector<float> weights;
    weights.resize(distances.size());

    if (minDistance < 0.00001) {
        for (int i = 0; i < distances.size(); i++) {
            weights[i] = (i == minIndex) ? 1.0f : 0.0f;
        }
    } else {
        auto weightSum = 0.0f;
        for (int i = 0; i < distances.size(); i++) {
            weights[i] = 1.f / distances[i];
            weightSum += weights[i];
        }
        for (float& weight : weights) {
            weight /= weightSum;
        }
    }


    if (!mappings.empty()) {
        auto currentMappingIt = currentMapping.pluginParameters.begin();
        std::vector<std::list<PluginParameterSet>::iterator> mappingPointIterators;
        {
            int i = 0;
            for (auto& mapping : mappings) {
                mappingPointIterators.push_back(mapping.pluginParameters.begin());
                mapping.contributionWeight = weights[i];
                ++i;
            }
        }
        while (currentMappingIt != currentMapping.pluginParameters.end()) {
            for (int i = 0; i < currentMappingIt->parameters.size(); ++i) {
                currentMappingIt->parameters[i].value = 0;
                for (int j = 0; j < mappingPointIterators.size(); ++j) {
                    currentMappingIt->parameters[i].value += mappingPointIterators[j]->parameters[i].value * weights[j];
                }
            }
            ++currentMappingIt;
            for (auto& it: mappingPointIterators) {
                ++it;
            }
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
        p->setAttribute("path", plugin.file.getFullPathName());
        auto par = p->createNewChildElement("params");
        for (auto& param: plugin.parameters) {
            par->setAttribute("param_" + juce::String(param.parameter.getParameterIndex()), param.value);
        }
    }
    auto mapPositions = mapping->createNewChildElement("map-positions");
    for (auto& mapPoint : mappings) {
        auto mp = mapPositions->createNewChildElement("map-point");
        auto mpPosition = mp->createNewChildElement("position");
        mpPosition->setAttribute("x", mapPoint.position.x);
        mpPosition->setAttribute("y", mapPoint.position.y);
        mpPosition->setAttribute("z", mapPoint.position.z);
        auto mpPplugins = mp->createNewChildElement("plugins");
        for (auto& plugin : mapPoint.pluginParameters) {
            auto p = mpPplugins->createNewChildElement("plugin");
            p->setAttribute("path", plugin.file.getFullPathName());
            auto par = p->createNewChildElement("params");
            for (auto& param : plugin.parameters) {
                par->setAttribute("param_" + juce::String(param.parameter.getParameterIndex()), param.value);
            }
        }
    }
}

void MappingCenter::importFromXml(const juce::XmlElement &xml) {
    criticalSection.enter();

    auto mapping = xml.getChildByName("mapping");
    if (mapping == nullptr) {
        criticalSection.exit();
        return;
    }
    auto currentState = mapping->getChildByName("current-state");
    if (currentState == nullptr) {
        criticalSection.exit();
        return;
    }
    auto currentPosition = currentState->getChildByName("position");
    if (currentPosition == nullptr) {
        criticalSection.exit();
        return;
    }
    currentMapping.position.x = static_cast<float>(currentPosition->getDoubleAttribute("x", 0));
    currentMapping.position.x = static_cast<float>(currentPosition->getDoubleAttribute("x", 0));
    currentMapping.position.x = static_cast<float>(currentPosition->getDoubleAttribute("x", 0));
    pluginGroup.resetAllPlugins();
    auto currentPlugins = currentState->getChildByName("plugins");
    if (currentPlugins != nullptr) {
        currentMapping.pluginParameters.clear();
        for (auto* p : currentPlugins->getChildWithTagNameIterator("plugin")) {
            auto path = p->getStringAttribute("path", "");
            if (path != "") {
                juce::String error = "";
                auto processor = pluginGroup.addPlugin(path, error);
                if (error != "" || processor == nullptr) {
                    std::cout << error << std::endl;
                    criticalSection.exit();
                    return;
                }
                PluginParameterSet parameterSet = {*processor, path, {}};

                auto par = p->getChildByName("params");
                if (par == nullptr) {
                    continue;
                }
                for (auto& param : processor->getParameters()) {
                    if (param->isAutomatable()) {
                        auto pValue = static_cast<float>(par->getDoubleAttribute("param_" + juce::String(param->getParameterIndex()), param->getDefaultValue()));
                        parameterSet.parameters.push_back({pValue, *param});
                    }
                }
                currentMapping.pluginParameters.push_back(parameterSet);
            }
        }
    }
    mappings.clear();
    auto mapPositions = mapping->getChildByName("map-positions");
    if (mapPositions == nullptr) {
        criticalSection.exit();
        return;
    }
    for (auto& mapPoint : mapPositions->getChildWithTagNameIterator("map-point")) {
        auto newMapping = MappingPoint();
        auto mapPosition = mapPoint->getChildByName("position");
        if (mapPosition == nullptr) {
            continue;
        }
        newMapping.position.x = static_cast<float>(mapPosition->getDoubleAttribute("x", 0));
        newMapping.position.x = static_cast<float>(mapPosition->getDoubleAttribute("x", 0));
        newMapping.position.x = static_cast<float>(mapPosition->getDoubleAttribute("x", 0));
        auto plugs = mapPoint->getChildByName("plugins");
        if (plugs != nullptr) {
            newMapping.pluginParameters.clear();
            int i = 0;
            for (auto* p : plugs->getChildWithTagNameIterator("plugin")) {
                auto path = p->getStringAttribute("path", "");
                if (path != "") {
                    auto processor = getNthProcessor(i);
                    if (processor == nullptr) {
                        continue;
                    }
                    PluginParameterSet parameterSet = {*processor, path, {}};
                    ++i;

                    auto par = p->getChildByName("params");
                    if (par == nullptr) {
                        continue;
                    }
                    for (auto& param : processor->getParameters()) {
                        if (param->isAutomatable()) {
                            auto pValue = static_cast<float>(par->getDoubleAttribute("param_" + juce::String(param->getParameterIndex()), param->getDefaultValue()));
                            parameterSet.parameters.push_back({pValue, *param});
                        }
                    }
                    newMapping.pluginParameters.push_back(parameterSet);
                }
            }
        }
        mappings.push_back(newMapping);
    }
    criticalSection.exit();
}

juce::Vector3D<float> MappingCenter::getMappingPointAttractionForce() const {
    return mappingPointAttractionForce;
}

juce::Vector3D<float> MappingCenter::getWallPushbackForce() const {
    return wallPushbackForce;
}

void MappingCenter::timerCallback() {
    recalculateForces();
}

juce::Vector3D<float> MappingCenter::calculateMappingPointAttractionForce(const juce::Vector3D<float>& atPoint) const {
    const float eyeRadius = 0.01f;
    const float eyeRadiusSquared = eyeRadius * eyeRadius;

    auto force = juce::Vector3D<float>(0,0,0);

    for (const auto& mapping : mappings) {
        auto vectorToPos = mapping.position - atPoint;
        auto lSquared = vectorToPos.lengthSquared();

        if (lSquared < eyeRadiusSquared) {
            return {0,0,0};
        }

        force += vectorToPos * eyeRadius / lSquared;
    }
    return force;

}

juce::Vector3D<float> MappingCenter::calculateWallPushbackForce(const juce::Vector3D<float>& atPoint) const {
    return {0,0,0};
}

void MappingCenter::recalculateForces() {
    mappingPointAttractionForce = calculateMappingPointAttractionForce(currentMapping.position);
    wallPushbackForce = calculateWallPushbackForce(currentMapping.position);
}

void MappingCenter::parameterValueChanged(int parameterIndex, float newValue) {
    if (!inputEnabled) {
        currentMapping.position.x = *xParam;
        currentMapping.position.y = *yParam;
        currentMapping.position.z = *zParam;
    }
}

void MappingCenter::parameterGestureChanged(int parameterIndex, bool gestureIsStarting) {

}

juce::AudioProcessor* MappingCenter::getNthProcessor(int n) {
    int i = 0;
    for (auto& plugin : currentMapping.pluginParameters) {
        if (i == n) {
            return &plugin.processor;
        }
        ++i;
    }
    return nullptr;
}

