#include <iostream>
#include <string>

#include "../glm.hpp"

#include "core/Mesh.hpp" //
#include "core/World.hpp" //
#include "core/Texture.hpp" //
#include "core/Camera.hpp"  //
#include "core/GameObject.hpp" //
#include "materials/TerrainMaterial.hpp" //
#include "materials/ColorMaterial.hpp" //
#include "util/DebugHud.hpp" //
#include "config.hpp" //
#include "MGEDemo.hpp" //

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{

    // terrain setup
    Mesh* terrainMesh = Mesh::load(config::MGE_MODEL_PATH + "plane20x20_2048tris_aligned_uvs.obj");

    // new terrain material
    AbstractMaterial* terrainMaterial = new TerrainMaterial(
        Texture::load(config::MGE_TEXTURE_PATH + "heightmap.png"),  // height
        Texture::load(config::MGE_TEXTURE_PATH + "splatmap.png"),   // splat map
        Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg"),   // grass
        Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg"),   // sand
        Texture::load(config::MGE_TEXTURE_PATH + "diffuse3.jpg"),    // rock
        Texture::load(config::MGE_TEXTURE_PATH + "diffuse4.jpg")   // snow
    );

    // terrain obj
    GameObject* terrain = new GameObject("terrain", glm::vec3(0, 0, 0));
    terrain->setMesh(terrainMesh);
    terrain->setMaterial(terrainMaterial);
    terrain->scale(glm::vec3(1, 1, 1));
    terrain->rotate(glm::radians(-90.0f), glm::vec3(0,1,0));
    _world->add(terrain);

    // camera
    Camera* camera = new Camera ("camera", glm::vec3(0,10,15));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

}

void MGEDemo::_render() {
    AbstractGame::_render();
    //_updateHud();
}

void MGEDemo::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    //_hud->setDebugInfo(debugInfo);
    //_hud->draw();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
