//
// Created by Arden on 2025-10-09.
//

#include "PluginGroup.h"

#include <utility>

void PluginGroup::Listener::groupUpdated(const PluginGroup &group) {
}

PluginGroup::PluginGroup(juce::AudioProcessor::BusesLayout layout) : hasChanged(false), busesLayout(std::move(layout)) {
    formatManager.addDefaultFormats();
    sampleRate = 0;
    samplesPerBlock = 0;
    nodeIDs.clear();
}

PluginGroup::~PluginGroup() {
}

void PluginGroup::updateGraph() {
    if (hasChanged) {
        hasChanged = false;
        for (auto& connection : graph.getConnections()) {
            graph.removeConnection(connection);
        }

        if (nodeIDs.empty()) {
            for (int channel = 0; channel < 2; channel++) {
                graph.addConnection({
                    {audioInputNode->nodeID, channel},
                    {audioOutputNode->nodeID, channel}});
            }
        } else {
            for (int channel = 0; channel < 2; channel++) {
                graph.addConnection({
                    {audioInputNode->nodeID, channel},
                        {nodeIDs.front(), channel}});
            }
            for (int index = 0; index < nodeIDs.size() - 1; index++) {
                for (int channel = 0; channel < 2; channel++) {
                    graph.addConnection({
                        {nodeIDs[index], channel},
                        {nodeIDs[index + 1], channel} });
                }
            }
            for (int channel = 0; channel < 2; channel++) {
                graph.addConnection({
                    {nodeIDs.back(), channel},
                        {audioOutputNode->nodeID, channel}});
            }
        }
    }
}


void PluginGroup::prepareToPlay(int inputChannels, int outputChannels, double _sampleRate, int _samplesPerBlockExpected) {
    sampleRate = _sampleRate;
    samplesPerBlock = _samplesPerBlockExpected;

    graph.clear();
    audioInputNode = graph.addNode (std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>
        (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = graph.addNode (std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>
        (juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    midiInputNode = graph.addNode (std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>
        (juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode));
    midiOutputNode = graph.addNode (std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>
        (juce::AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode));

    audioInputNode->getProcessor()->setBusesLayout(busesLayout);
    audioOutputNode->getProcessor()->setBusesLayout(busesLayout);
    midiInputNode->getProcessor()->setBusesLayout(busesLayout);
    midiOutputNode->getProcessor()->setBusesLayout(busesLayout);

    for (int channel = 0; channel < 2; ++channel) {
        graph.addConnection(
            { { audioInputNode->nodeID, channel },
            { audioOutputNode->nodeID, channel } });
    }
    graph.addConnection (
        { { midiInputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex },
            { midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
    graph.setPlayConfigDetails(inputChannels, outputChannels, sampleRate, samplesPerBlock);
    graph.prepareToPlay(sampleRate, samplesPerBlock);
}

void PluginGroup::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer& m) {
    updateGraph();
    graph.processBlock(buffer, m);
}

int PluginGroup::addPlugin(const juce::File& file, juce::String& errorMessage) {
    auto typesFound = juce::OwnedArray<juce::PluginDescription>();
    knownPlugins.scanAndAddDragAndDroppedFiles(formatManager, {file.getFullPathName()}, typesFound);
    auto description = knownPlugins.getTypeForFile(file.getFullPathName());
    if (description == nullptr) {
        errorMessage =  "Couldn't get plugin description: " + file.getFullPathName();
        return 1;
    }

    std::unique_ptr<juce::AudioProcessor> pluginInstance = formatManager.createPluginInstance(
        *description, sampleRate, samplesPerBlock, errorMessage);
    pluginInstance->setBusesLayout(busesLayout);
    pluginInstance->prepareToPlay(sampleRate, samplesPerBlock);
    auto newNode = graph.addNode(std::move(pluginInstance));

    nodeIDs.push_back(newNode->nodeID);

    hasChanged = true;
    for (auto& listener : listeners) {
        listener->groupUpdated(*this);
    }

    return 0;
}

void PluginGroup::releaseResources() {
    graph.releaseResources();
}

void PluginGroup::addListener(Listener* listener) {
    listeners.insert(listener);
}

void PluginGroup::removeListener(Listener* listener) {
    listeners.erase(listener);
}

