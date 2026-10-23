#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../../glm.hpp"
#include "GameObject.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class Light : public GameObject {
public:
    Light(const std::string &aName = nullptr, const glm::vec3 &aPosition = glm::vec3(2.0f, 10.0f, 5.0f));

    virtual ~Light();

    void setLightColor(const glm::vec3 &pColor);
    glm::vec3 getLightColor() const;

    void setLightIntensity(float pIntensity);
    float getLightIntensity() const;


    //override set parent to register/deregister light...
    virtual void _setWorldRecursively(World *pWorld) override;

    void setAttenuation(float constantAttenuation, float linearAttenuation, float quadraticAttenuation);
    float getConstantAttenuation() const;
    float getLinearAttenuation() const; //reduces the light linearly
    float getQuadraticAttenuation() const; //reduces the light quadratically

private:
    glm::vec3 _lightColor;
    float _lightIntensity;

    float _constantAttenuation = 1.0f;
    float _linearAttenuation  = 0.09f;
    float _quadraticAttenuation = 0.032f;
};

#endif // LIGHT_HPP
