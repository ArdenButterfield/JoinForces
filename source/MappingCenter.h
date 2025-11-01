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
    std::vector<Parameter> parameters;
};

struct MappingPoint {
    juce::Vector3D<float> position;
    std::list<PluginParameterSet> pluginParameters;
};

class MappingCenter : public PluginGroup::Listener {
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
    void groupUpdated(const PluginGroup &group) override;

    void insertInto(MappingPoint& mapping);
    void removeFrom(MappingPoint& mapping);

    std::vector<MappingPoint> mappings;
    MappingPoint currentMapping;
    void calculateCurrentMapping();
};


#endif //JOINFORCES_MAPPINGCENTER_H