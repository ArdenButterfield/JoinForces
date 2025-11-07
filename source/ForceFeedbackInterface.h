//
// Created by Arden on 2025-10-17.
//

#ifndef JOINFORCES_FORCEFEEDBACKINTERFACE_H
#define JOINFORCES_FORCEFEEDBACKINTERFACE_H

#include <juce_opengl/juce_opengl.h>

#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>

#include <HLU/hlu.h>

class MappingCenter;

class ForceFeedbackInterface {
public:
    ForceFeedbackInterface();
    ~ForceFeedbackInterface();
    void init();
    juce::Vector3D<float> getCurrentPosition();
    void deInit();
    HDCallbackCode callback();

    bool isInitialized();

    void setMappingCenter(MappingCenter*);
private:
    MappingCenter* mappingCenter;
    juce::Vector3D<float> getMappingPointAttractionForce() const;
    bool initialized;
    juce::Vector3D<float> position;
    HHD ghHD = HD_INVALID_HANDLE;
    HDSchedulerHandle schedulerHandle;

    const float eyeRadius = 0.01f;
    const float eyeRadiusSquared = eyeRadius * eyeRadius;
    const float positionEpsilon = 0.05f;

    int samplesWithoutSync;

};

HDCallbackCode ffCallback(void *data);

#endif //JOINFORCES_FORCEFEEDBACKINTERFACE_H