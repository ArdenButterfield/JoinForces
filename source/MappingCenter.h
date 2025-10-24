//
// Created by Arden on 2025-10-17.
//

#ifndef JOINFORCES_MAPPINGCENTER_H
#define JOINFORCES_MAPPINGCENTER_H

#include <vector>

#include "ForceFeedbackInterface.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_opengl/juce_opengl.h"

class PluginGroup;

class MappingCenter {
public:
    struct PluginParameterSet {
        juce::AudioProcessor& processor;
        std::vector<float> parameterValues;
    };

    struct MappingPoint {
        juce::Vector3D<float> position;
        std::vector<PluginParameterSet> pluginParameters;
    };

    MappingCenter(PluginGroup& group, ForceFeedbackInterface& ffInterface);
    ~MappingCenter();
    void createMappingAtCurrentState();
    const std::vector<MappingPoint>& getMappings() {
        return mappings;
    }

    PluginGroup& group;
    ForceFeedbackInterface& ffInterface;
private:
    std::vector<MappingPoint> mappings;
};


#endif //JOINFORCES_MAPPINGCENTER_H