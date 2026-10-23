#include "TerrainMaterial.hpp"
#include "../core/ShaderProgram.hpp"
#include "../config.hpp"
#include "../core/Mesh.hpp"
#include "../core/World.hpp"

ShaderProgram* TerrainMaterial::_shader = nullptr;

TerrainMaterial::TerrainMaterial(Texture* pHeightMap, Texture* pSplatMap, Texture* pTex1, Texture* pTex2, Texture* pTex3, Texture* pTex4)
    : _heightMap(pHeightMap), _splatMap(pSplatMap), _texture1(pTex1), _texture2(pTex2), _texture3(pTex3), _texture4(pTex4) {
    _lazyInitializeShader();
}

void TerrainMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.fs");
        _shader->finalize();
    }
}

void TerrainMaterial::render(World* pWorld, Mesh* pMesh,
                             const glm::mat4& pModelMatrix,
                             const glm::mat4& pViewMatrix,
                             const glm::mat4& pProjectionMatrix) {
    _shader->use();

    // send matrixes
    GLint modelLoc = _shader->getUniformLocation("modelMatrix");
    GLint viewLoc = _shader->getUniformLocation("viewMatrix");
    GLint projLoc = _shader->getUniformLocation("projectionMatrix");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));

    // set heigth scale
    GLint heightScaleLoc = _shader->getUniformLocation("heightScale");
    //glUniform1f(heightScaleLoc, 50.0f);
    glUniform1f(heightScaleLoc, 10.0f);

    // //get textures uniform location
    GLint heightMapLoc = _shader->getUniformLocation("heightMap");
    GLint splatMapLoc = _shader->getUniformLocation("splatMap");
    GLint tex1Loc = _shader->getUniformLocation("texture1");
    GLint tex2Loc = _shader->getUniformLocation("texture2");
    GLint tex3Loc = _shader->getUniformLocation("texture3");
    GLint tex4Loc = _shader->getUniformLocation("texture4");

    // bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _heightMap->getId());
    glUniform1i(heightMapLoc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _splatMap->getId());
    glUniform1i(splatMapLoc, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texture1->getId());
    glUniform1i(tex1Loc, 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _texture2->getId());
    glUniform1i(tex2Loc, 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _texture3->getId());
    glUniform1i(tex3Loc, 4);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, _texture4->getId());
    glUniform1i(tex4Loc, 5);

    // tell mesh which attributes to use
    pMesh->streamToOpenGL(
       _shader->getAttribLocation("vertex"),
       _shader->getAttribLocation("normal"),
       _shader->getAttribLocation("uv")
   );
}

TerrainMaterial::~TerrainMaterial() {

}