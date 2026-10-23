#ifndef TERRAINMATERIAL_HPP
#define TERRAINMATERIAL_HPP

#include "AbstractMaterial.hpp"
#include "../core/Texture.hpp"
#include "../core/ShaderProgram.hpp"

class TerrainMaterial : public AbstractMaterial {
public:
    TerrainMaterial(Texture* pHeightMap, Texture* pSplatMap, Texture* pTex1, Texture* pTex2, Texture* pTex3, Texture* pTex4);
    virtual ~TerrainMaterial();

    virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix,
                        const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

private:
    static ShaderProgram* _shader;
    static void _lazyInitializeShader();

    Texture* _heightMap;
    Texture* _splatMap;
    Texture* _texture1;
    Texture* _texture2;
    Texture* _texture3;
    Texture* _texture4;
};

#endif