//
// Created by Arden on 2025-10-17.
//

#ifndef JOINFORCES_FORCEFEEDBACKINTERFACE_H
#define JOINFORCES_FORCEFEEDBACKINTERFACE_H

#include <juce_opengl/juce_opengl.h>

#if JUCE_WINDOWS

#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>

#endif

class MappingCenter;

class ForceFeedbackInterface {
public:
    ForceFeedbackInterface();
    ~ForceFeedbackInterface();
    void init();
    juce::Vector3D<float> getCurrentPosition();
    void deInit();
#if JUCE_WINDOWS
    HDCallbackCode callback();
#endif
    bool isInitialized();

    void setMappingCenter(MappingCenter*);

private:
    MappingCenter* mappingCenter;
    bool initialized;
    juce::Vector3D<float> position;
#if JUCE_WINDOWS
    HHD ghHD = HD_INVALID_HANDLE;
    HDSchedulerHandle schedulerHandle;
#endif
    const float positionEpsilon = 0.05f;

    int samplesWithoutSync;

};
#if JUCE_WINDOWS
HDCallbackCode ffCallback(void *data);
#endif
#endif //JOINFORCES_FORCEFEEDBACKINTERFACE_H