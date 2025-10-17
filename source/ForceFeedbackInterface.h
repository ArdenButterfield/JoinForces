//
// Created by Arden on 2025-10-17.
//

#ifndef JOINFORCES_FORCEFEEDBACKINTERFACE_H
#define JOINFORCES_FORCEFEEDBACKINTERFACE_H

#include <juce_opengl/juce_opengl.h>

class ForceFeedbackInterface {
public:
    ForceFeedbackInterface();
    ~ForceFeedbackInterface();
    juce::Vector3D<float> getCurrentPosition();
};


#endif //JOINFORCES_FORCEFEEDBACKINTERFACE_H