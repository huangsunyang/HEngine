//
// Created by 00 on 2021/11/22.
//

#ifndef HENGINE_CAMERAMANAGER_H
#define HENGINE_CAMERAMANAGER_H

#include <vector>
#include "camera/Camera.hpp"

using std::vector;
using std::pair;

class CameraManager {
public:
    static CameraManager * getInstance();
    void addCamera(Camera * camera);
    vector<Camera *> getCameras() {return m_cameras;}

private:
    CameraManager(): m_cameras() {}

    static CameraManager * s_instance;
    vector<Camera*> m_cameras;
};


#endif //HENGINE_CAMERAMANAGER_H
