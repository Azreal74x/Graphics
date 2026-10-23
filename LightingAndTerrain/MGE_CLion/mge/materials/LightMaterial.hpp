#ifndef LIGHTMATERIAL_HPP
#define LIGHTMATERIAL_HPP

#include <GL/glew.h>
#include "AbstractMaterial.hpp"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class LightMaterial : public AbstractMaterial {
public:
    LightMaterial(Texture *pDiffuseTexture);

    virtual ~LightMaterial();

    virtual void render(World *pWorld, Mesh *pMesh, const glm::mat4 &pModelMatrix, const glm::mat4 &pViewMatrix,
                        const glm::mat4 &pProjectionMatrix) override;

    void setDiffuseTexture(Texture *pDiffuseTexture);

    void setSpecular(float strength, float shininess);

protected:
private:
    static ShaderProgram *_shader;

    static void _lazyInitializeShader();

    //create seperate matrices
    static GLint _uModelMatrix;
    static GLint _uViewMatrix;
    static GLint _uProjectionMatrix;

    //static GLint _uMVPMatrix;

    static GLint _uDiffuseTexture;

    static GLint _aVertex;
    static GLint _aNormal;
    static GLint _aUV;

    Texture *_diffuseTexture;

    LightMaterial(const LightMaterial &);

    LightMaterial &operator=(const LightMaterial &);

    float _specularStrength = 0.5f;
    float _shininess = 32.0f;

};

#endif // TEXTUREMATERIAL_HPP
