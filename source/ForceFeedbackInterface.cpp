//
// Created by Arden on 2025-10-17.
//

#include "ForceFeedbackInterface.h"

ForceFeedbackInterface::ForceFeedbackInterface() {
}

ForceFeedbackInterface::~ForceFeedbackInterface() {
}

void ForceFeedbackInterface::init() {
    HDErrorInfo error;
    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);

    if (HD_DEVICE_ERROR(error = hdGetError())) {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
    }
}

juce::Vector3D<float> ForceFeedbackInterface::getCurrentPosition() {
    return {0.f,0.f,0.f}; // TEMP
}

void ForceFeedbackInterface::deInit() {
}
