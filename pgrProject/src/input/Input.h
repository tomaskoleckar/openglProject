#pragma once
#include "../Camera/Camera.h"
#include "../Window/Window.h"
#include "../light/ShadingMode.h"
#include "../Application/Application.h"
class Input {
public:
    static void update(Window& window, Camera& camera,ShadingMode& shading, SceneType& sceneType,bool& bugMode,bool& flashlight, float dt);
    static int pickedPixelX;
    static int pickedPixelY;
    static bool requestPickEvent;
    static bool requestDeleteEvent;
};
