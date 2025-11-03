//
// Created by Arden on 2025-10-17.
//

#include "ForceFeedbackInterface.h"

ForceFeedbackInterface::ForceFeedbackInterface() : initialized(false) {
}

ForceFeedbackInterface::~ForceFeedbackInterface() {
}

void ForceFeedbackInterface::init() {
    HDErrorInfo error;
    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);

    if (HD_DEVICE_ERROR(error = hdGetError())) {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        return;
    }
    std::cout << "Device initialized" << std::endl;
    schedulerHandle = hdScheduleAsynchronous(
        ffCallback, this,
        HD_MAX_SCHEDULER_PRIORITY);

    hdEnable(HD_FORCE_OUTPUT);
    hdStartScheduler();

    initialized = true;

}

juce::Vector3D<float> ForceFeedbackInterface::getCurrentPosition() {
    return position;
}

void ForceFeedbackInterface::deInit() {
}

HDCallbackCode ForceFeedbackInterface::callback() {
    HHD hHD = hdGetCurrentDevice();
    hdBeginFrame(hHD);

    hduVector3Dd p;

    /* Get the current position of the device. */
    hdGetDoublev(HD_CURRENT_POSITION, p);

    auto positionPoints = static_cast<double *>(p);


    auto convertToMinMax = [] (float v) {
        auto low = -100.f;
        auto high = 100.f;

        v = std::min(std::max(low, v), high);
        v -= low;
        v /= (high - low);
        return v;
    };

    position.x = convertToMinMax(positionPoints[0]);
    position.y = convertToMinMax(positionPoints[1]);
    position.z = convertToMinMax(positionPoints[2]);

    /* End haptics frame. */
    hdEndFrame(hHD);

    return HD_CALLBACK_CONTINUE;
}

bool ForceFeedbackInterface::isInitialized() {
    return initialized;
}

HDCallbackCode ffCallback(void *data) {
    return static_cast<ForceFeedbackInterface*>(data)->callback();
}
