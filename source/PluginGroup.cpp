//
// Created by Arden on 2025-10-09.
//

#include "PluginGroup.h"

#include <utility>

PluginGroup::PluginGroup(juce::AudioProcessor::BusesLayout layout) : hasChanged(false), busesLayout(std::move(layout)) {
    formatManager.addDefaultFormats();
    sampleRate = 0;
    samplesPerBlock = 0;
    nodeIDs.clear();
}

PluginGroup::~PluginGroup() = default;

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


void PluginGroup::prepareToPlay(int input, int output, double _sampleRate, int _samplesPerBlockExpected) {
    std::cout << "prepare to play group" << this << "\n";
    sampleRate = _sampleRate;
    samplesPerBlock = _samplesPerBlockExpected;
    numInputChannels = input;
    numOutputChannels = output;
    graph.clear();
    nodeIDs.clear();

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
    graph.setPlayConfigDetails(numInputChannels, numOutputChannels, sampleRate, samplesPerBlock);
    graph.prepareToPlay(sampleRate, samplesPerBlock);
}

void PluginGroup::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer& m) {
    updateGraph();
    graph.processBlock(buffer, m);
}

juce::AudioProcessor* PluginGroup::addPlugin(const juce::File& file, juce::String& errorMessage) {
    auto typesFound = juce::OwnedArray<juce::PluginDescription>();
    knownPlugins.scanAndAddDragAndDroppedFiles(formatManager, {file.getFullPathName()}, typesFound);
    auto description = knownPlugins.getTypeForFile(file.getFullPathName());
    if (description == nullptr) {
        errorMessage =  "Couldn't get plugin description: " + file.getFullPathName();
        return nullptr;
    }

    std::unique_ptr<juce::AudioProcessor> pluginInstance = formatManager.createPluginInstance(
        *description, sampleRate, samplesPerBlock, errorMessage);
    pluginInstance->setBusesLayout(busesLayout);
    pluginInstance->prepareToPlay(sampleRate, samplesPerBlock);

    for (auto& parameter : pluginInstance->getParameters()) {
        parameter->setValueNotifyingHost(parameter->getDefaultValue());
    }

    auto newNode = graph.addNode(std::move(pluginInstance));

    nodeIDs.push_back(newNode->nodeID);

    hasChanged = true;

    return newNode->getProcessor();
}

void PluginGroup::releaseResources() {
    graph.releaseResources();
}

void PluginGroup::resetAllPlugins() {
    prepareToPlay(numInputChannels, numOutputChannels, sampleRate, samplesPerBlock);
}

