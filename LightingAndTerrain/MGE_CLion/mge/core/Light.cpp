#include "Light.hpp"
#include "World.hpp"

Light::Light(const std::string &pName,
    const glm::vec3 &pPosition):GameObject(pName, pPosition),
    _lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
    _lightIntensity(1.0f)
{}

Light::~Light() {
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void Light::_setWorldRecursively(World *pWorld) {
    //store the previous and new world before and after storing the given world
    World *previousWorld = _world;
    GameObject::_setWorldRecursively(pWorld);
    World *newWorld = _world;

    //check whether we need to register or unregister
    if (previousWorld != nullptr) previousWorld->unregisterLight(this);
    if (newWorld != nullptr) newWorld->registerLight(this);
}

// ----------------------------------------------------
// New getters and setters
// ----------------------------------------------------
void Light::setLightColor(const glm::vec3& pLightColor) {
    _lightColor = pLightColor;
}

glm::vec3 Light::getLightColor() const {
    return _lightColor;
}

void Light::setLightIntensity(float pLightIntensity) {
    _lightIntensity = pLightIntensity;
}

float Light::getLightIntensity() const {
    return _lightIntensity;
}

void Light::setAttenuation(float constantAttenuation, float linearAttenuation, float quadraticAttenuation) {
    _constantAttenuation = constantAttenuation;
    _linearAttenuation = linearAttenuation;
    _quadraticAttenuation = quadraticAttenuation;
}

float Light::getConstantAttenuation() const { return _constantAttenuation; }
float Light::getLinearAttenuation() const { return _linearAttenuation; }
float Light::getQuadraticAttenuation() const { return _quadraticAttenuation; }
