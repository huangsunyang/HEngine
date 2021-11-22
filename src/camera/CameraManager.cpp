//
// Created by 00 on 2021/11/22.
//

#include "CameraManager.h"

using std::pair;

CameraManager * CameraManager::s_instance = nullptr;

CameraManager * CameraManager::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new CameraManager;
    }
    return s_instance;
}

void CameraManager::addCamera(Camera *camera) {
    m_cameras.push_back(camera);
}
