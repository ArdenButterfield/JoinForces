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

class ForceFeedbackInterface {
public:
    ForceFeedbackInterface();
    ~ForceFeedbackInterface();
    void init();
    juce::Vector3D<float> getCurrentPosition();
    void deInit();
private:
    HHD ghHD = HD_INVALID_HANDLE;
};


#endif //JOINFORCES_FORCEFEEDBACKINTERFACE_H