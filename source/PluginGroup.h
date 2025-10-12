//
// Created by Arden on 2025-10-09.
//

#ifndef JOINFORCES_PLUGINGROUP_H
#define JOINFORCES_PLUGINGROUP_H

#include <vector>
#include <list>
#include <memory>

#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_audio_formats/juce_audio_formats.h"

class PluginGroup {
public:
    PluginGroup(juce::AudioProcessor::BusesLayout layout);
    ~PluginGroup();
    int addPlugin(const juce::File& file, juce::String& errorMessage);
    void prepareToPlay(int inputChannels, int outputChannels, double sampleRate, int samplesPerBlockExpected);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& m);
    void releaseResources();
private:
    bool hasChanged;
    void updateGraph();
    std::vector<juce::AudioProcessorGraph::NodeID> nodeIDs;
    std::unique_ptr<juce::AudioFormatReader> inputReader;
    juce::AudioPluginFormatManager formatManager;
    juce::KnownPluginList knownPlugins;
    double sampleRate;
    int samplesPerBlock;
    juce::AudioProcessorGraph graph;
    juce::AudioProcessorGraph::Node::Ptr audioInputNode;
    juce::AudioProcessorGraph::Node::Ptr audioOutputNode;
    juce::AudioProcessorGraph::Node::Ptr midiInputNode;
    juce::AudioProcessorGraph::Node::Ptr midiOutputNode;
    const juce::AudioProcessor::BusesLayout busesLayout;
};


#endif //JOINFORCES_PLUGINGROUP_H