//
// Created by Arden on 2025-10-17.
//

#include "ForceFeedbackInterface.h"

ForceFeedbackInterface::ForceFeedbackInterface() {
}

ForceFeedbackInterface::~ForceFeedbackInterface() {
}

juce::Vector3D<float> ForceFeedbackInterface::getCurrentPosition() {
    return {0.f,0.f,0.f}; // TEMP
}
