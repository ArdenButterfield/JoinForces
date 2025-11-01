//
// Created by Arden on 2025-10-30.
//

#include "JoinForcesLookFeel.h"

#include "../MappingCenter.h"
#include "../PluginGroup.h"

int JoinForcesLookFeel::getColumnRequiredHeight(MappingCenter &mappingCenter) {
    int h = 0;
    for (auto& id : mappingCenter.group.getNodes()) {
        auto processor = mappingCenter.group.getAudioProcessorGraph().getNodeForId(id)->getProcessor();
        for (auto& )
    }
}
