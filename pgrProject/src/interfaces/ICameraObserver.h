#pragma once
class Camera;

class ICameraObserver {
public:
    virtual ~ICameraObserver() = default;
    virtual void onCameraChanged(const Camera& camera) = 0;
};