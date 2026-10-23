#include "LightMaterial.hpp"
#include "../core/Texture.hpp"
#include "../core/Light.hpp"
#include "../core/World.hpp"
#include "../core/Mesh.hpp"
#include "../core/GameObject.hpp"
#include "../core/ShaderProgram.hpp"
#include "../config.hpp"
#include "../core/Camera.hpp"
#include "../../glm.hpp"

ShaderProgram *LightMaterial::_shader = NULL;

GLint LightMaterial::_uModelMatrix = 0;
GLint LightMaterial::_uViewMatrix = 0;
GLint LightMaterial::_uProjectionMatrix = 0;

//GLint LightMaterial::_uMVPMatrix = 0;

GLint LightMaterial::_uDiffuseTexture = 0;

GLint LightMaterial::_aVertex = 0;
GLint LightMaterial::_aNormal = 0;
GLint LightMaterial::_aUV = 0;

LightMaterial::LightMaterial(Texture *pDiffuseTexture): _diffuseTexture(pDiffuseTexture) {
    _lazyInitializeShader();
}

LightMaterial::~LightMaterial() {
}

void LightMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        //_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "light.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "light.fs");
        _shader->finalize();

        //get uniform internal id
        _uModelMatrix = _shader->getUniformLocation("modelMatrix");
        _uViewMatrix = _shader->getUniformLocation("viewMatrix");
        _uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");

        //_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");

        _uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV = _shader->getAttribLocation("uv");
    }
}

void LightMaterial::setDiffuseTexture(Texture *pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void LightMaterial::render(World *pWorld, Mesh *pMesh, const glm::mat4 &pModelMatrix, const glm::mat4 &pViewMatrix,
                           const glm::mat4 &pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

    //get uniform internal id
    GLint ambientColorLocation = _shader->getUniformLocation("ambientLightColor");
    GLint ambientIntensityLocation = _shader->getUniformLocation("ambientIntensity");

    //get first (should be ambient) light
    Light *firstLight = pWorld->getLightAt(0);

    //get ambient color and intensity from world
    glm::vec3 ambientColor = pWorld->getAmbientColor();
    float ambientIntensity = pWorld->getAmbientIntensity();

    //send uniform internal id
    glUniform3f(ambientColorLocation, ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    //get position, color, intensity from light
    glm::vec3 lightPosition = firstLight->getLocalPosition();
    glm::vec3 lightColor = firstLight->getLightColor();
    float lightIntensity = firstLight->getLightIntensity();

    //get uniforms internal id
    GLint lightPosLocation = _shader->getUniformLocation("lightPos");
    GLint lightColorLocation = _shader->getUniformLocation("lightColor");
    GLint lightIntensityLocation = _shader->getUniformLocation("lightIntensity");

    //send values to uniforms
    glUniform3f(lightPosLocation, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
    glUniform1f(lightIntensityLocation, lightIntensity);

    //get uniforms internal id
    GLint cameraPosLoc = _shader->getUniformLocation("cameraPos");
    GLint specularStrengthLoc = _shader->getUniformLocation("specularStrength");
    GLint shininessLoc = _shader->getUniformLocation("shininess");

    glm::vec3 camPos = pWorld->getMainCamera()->getLocalPosition();

    glUniform3f(cameraPosLoc, camPos.x, camPos.y, camPos.z);
    //shininess
    glUniform1f(specularStrengthLoc, _specularStrength);
    glUniform1f(shininessLoc, _shininess);

    //Print the number of lights in the scene and the position of the first light.
    //It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
    //if (pWorld->getLightCount() > 0) {
    //    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
    //}

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    //tell the shader the texture slot for the diffuse texture is slot 0
    glUniform1i(_uDiffuseTexture, 0);

    //pass in a precalculate mvp matrix (see texture material for the opposite)
    //glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    //glUniformMatrix4fv ( _uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
    glUniformMatrix4fv(_uViewMatrix, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv(_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));

    GLint constantLoc = _shader->getUniformLocation("constantAttenuation");
    GLint linearLoc = _shader->getUniformLocation("linearAttenuation");
    GLint quadraticLoc = _shader->getUniformLocation("quadraticAttenuation");

    glUniform1f(constantLoc, firstLight->getConstantAttenuation());
    glUniform1f(linearLoc, firstLight->getLinearAttenuation());
    glUniform1f(quadraticLoc, firstLight->getQuadraticAttenuation());

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}

void LightMaterial::setSpecular(float strength, float shininess) {
    _specularStrength = strength;
    _shininess = shininess;
}

