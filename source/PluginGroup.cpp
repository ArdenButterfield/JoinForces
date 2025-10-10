//
// Created by Arden on 2025-10-09.
//

#include "PluginGroup.h"

#include <utility>

PluginGroup::PluginGroup(juce::AudioProcessor::BusesLayout layout) : busesLayout(std::move(layout)) {
    formatManager.addDefaultFormats();
    sampleRate = 0;
    samplesPerBlock = 0;
}

PluginGroup::~PluginGroup() {
}

void PluginGroup::updateGraph() {
    hasChanged = false;
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

int PluginGroup::addPlugin(const juce::String& path, juce::String& errorMessage) {
    auto typesFound = juce::OwnedArray<juce::PluginDescription>();
    knownPlugins.scanAndAddDragAndDroppedFiles(formatManager, {path}, typesFound);
    auto description = knownPlugins.getTypeForFile(path);
    if (description == nullptr) {
        errorMessage =  "Couldn't get plugin description: " + path;
        return 1;
    }


    auto node = formatManager.createPluginInstance(*description, sampleRate, samplesPerBlock, errorMessage);
    /*
    if (instances.back() == nullptr) {
        instances.pop_back();
        return 1;
    }
    */
    return 0;
}

void PluginGroup::releaseResources() {
    graph.releaseResources();
}

