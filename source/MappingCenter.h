//
// Created by Arden on 2025-10-17.
//

#ifndef JOINFORCES_MAPPINGCENTER_H
#define JOINFORCES_MAPPINGCENTER_H

#include <vector>

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_opengl/juce_opengl.h"

class PluginGroup;
class ForceFeedbackInterface;

struct Parameter {
    float value;
    juce::AudioProcessorParameter& parameter;
};

struct PluginParameterSet {
    juce::AudioProcessor& processor;
    std::vector<Parameter> parameters;
};

struct MappingPoint {
    juce::Vector3D<float> position;
    std::vector<PluginParameterSet> pluginParameters;
};

class MappingCenter {
public:
    MappingCenter(PluginGroup& group, ForceFeedbackInterface& ffInterface);
    ~MappingCenter();
    void createMappingAtCurrentState();
    std::vector<MappingPoint>& getMappings() {
        return mappings;
    }

    MappingPoint& getCurrentMapping() {
        return currentMapping;
    }

    PluginGroup& group;
    ForceFeedbackInterface& ffInterface;

    void processBlock();
private:
    std::vector<MappingPoint> mappings;
    MappingPoint currentMapping;
    void calculateCurrentMapping();
};


#endif //JOINFORCES_MAPPINGCENTER_H