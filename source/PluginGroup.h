//
// Created by Arden on 2025-10-09.
//

#ifndef JOINFORCES_PLUGINGROUP_H
#define JOINFORCES_PLUGINGROUP_H

#include <vector>
#include <set>
#include <memory>

#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_audio_formats/juce_audio_formats.h"

class PluginGroup {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void groupUpdated(const PluginGroup& group);
    };

    explicit PluginGroup(juce::AudioProcessor::BusesLayout layout);
    ~PluginGroup();
    int addPlugin(const juce::File& file, juce::String& errorMessage);
    void prepareToPlay(int inputChannels, int outputChannels, double sampleRate, int samplesPerBlockExpected);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& m);
    void releaseResources();
    const std::vector<juce::AudioProcessorGraph::NodeID>& getNodes() {
        return nodeIDs;
    }
    juce::AudioProcessorGraph& getAudioProcessorGraph() {
        return graph;
    }

    void addListener(Listener* listener);
    void removeListener(Listener* listener);

private:
    std::set<Listener*> listeners;
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