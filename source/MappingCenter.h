//
// Created by Arden on 2025-10-17.
//

#ifndef JOINFORCES_MAPPINGCENTER_H
#define JOINFORCES_MAPPINGCENTER_H

#include <list>
#include <vector>

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_opengl/juce_opengl.h"

#include "PluginGroup.h"

class ForceFeedbackInterface;

struct Parameter {
    float value;
    juce::AudioProcessorParameter& parameter;
};

struct PluginParameterSet {
    juce::AudioProcessor& processor;
    juce::File file;
    std::vector<Parameter> parameters;
};

struct MappingPoint {
    juce::Vector3D<float> position;
    std::list<PluginParameterSet> pluginParameters;
};

class MappingCenter : public juce::AudioProcessorParameter::Listener {
public:
    MappingCenter(const juce::AudioProcessor::BusesLayout &layout, ForceFeedbackInterface& ffInterface);
    ~MappingCenter();
    void createMappingAtCurrentState();
    std::list<MappingPoint>& getMappings() {
        return mappings;
    }

    MappingPoint& getCurrentMapping() {
        return currentMapping;
    }

    ForceFeedbackInterface& ffInterface;

    void processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages);

    int addPlugin(const juce::File& file, juce::String& errorMessage);

    PluginGroup pluginGroup;

    void exportToXml(juce::XmlElement& xml);
    void importFromXml(const juce::XmlElement& xml);

    juce::AudioParameterFloat* xParam;
    juce::AudioParameterFloat* yParam;
    juce::AudioParameterFloat* zParam;
private:
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
    static float closeness(juce::Vector3D<float> a, juce::Vector3D<float>b);
    void insertInto(MappingPoint& mapping, const juce::File& file);
    void removeFrom(MappingPoint& mapping);

    std::list<MappingPoint> mappings;
    MappingPoint currentMapping;
    void calculateCurrentMapping();

    juce::CriticalSection criticalSection;
    juce::AudioProcessor::BusesLayout busesLayout;
    juce::AudioProcessor* getNthProcessor(int n);
};


#endif //JOINFORCES_MAPPINGCENTER_H