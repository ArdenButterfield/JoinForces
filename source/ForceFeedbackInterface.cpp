//
// Created by Arden on 2025-10-17.
//

#include "ForceFeedbackInterface.h"

#include "MappingCenter.h"

ForceFeedbackInterface::ForceFeedbackInterface()
: mappingCenter(nullptr), initialized(false), schedulerHandle(0) {

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

    HDdouble force[3] = {0,0,0};

    auto attractionForce = getMappingPointAttractionForce();
    force[0] += attractionForce.x;
    force[1] += attractionForce.y;
    force[2] += attractionForce.z;

    hdSetDoublev(HD_CURRENT_FORCE,force);

    /* End haptics frame. */
    hdEndFrame(hHD);

    return HD_CALLBACK_CONTINUE;
}

bool ForceFeedbackInterface::isInitialized() {
    return initialized;
}

void ForceFeedbackInterface::setMappingCenter(MappingCenter* mc) {
    mappingCenter = mc;
}

juce::Vector3D<float> ForceFeedbackInterface::getMappingPointAttractionForce() const {
    auto force = juce::Vector3D<float>(0,0,0);
    if (mappingCenter == nullptr) {
        return force;
    }

    auto currentPos = mappingCenter->getCurrentMapping().position;


    for (auto mapping : mappingCenter->getMappings()) {
        auto vectorToPos = mapping.position - currentPos;
        auto lSquared = vectorToPos.lengthSquared();

        if (lSquared < eyeRadiusSquared) {
            return {0,0,0};
        }

            force += vectorToPos * eyeRadius / lSquared;
    }
    return force;
}

HDCallbackCode ffCallback(void *data) {
    return static_cast<ForceFeedbackInterface*>(data)->callback();
}
