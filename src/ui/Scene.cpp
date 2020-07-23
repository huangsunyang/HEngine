#include "ui/Scene.hpp"
#include "ui/Director.hpp"

void Scene::setCurrentScene() {
    Director::instance()->setScene(this);
}