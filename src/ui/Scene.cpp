#include "ui/Scene.hpp"
#include "base/Director.hpp"

void Scene::setCurrentScene() {
    Director::instance()->setScene(this);
}