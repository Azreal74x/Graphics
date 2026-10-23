#include <iostream>
#include <string>

#include "../glm.hpp"

#include "core/Renderer.hpp" //
#include "core/Mesh.hpp" //
#include "core/World.hpp" //
#include "core/Texture.hpp" //
#include "core/Light.hpp" //
#include "core/Camera.hpp"  //
#include "core/GameObject.hpp" //
#include "materials/AbstractMaterial.hpp" //
#include "materials/ColorMaterial.hpp" //
#include "materials/TextureMaterial.hpp" //
#include "behaviours/RotatingBehaviour.hpp" //
#include "behaviours/KeysBehaviour.hpp" //
#include "util/DebugHud.hpp" //
#include "config.hpp" //
#include "MGE2.hpp" //

#include "behaviours/FollowCameraBehaviour.hpp"
#include "materials/LightMaterial.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGE2::MGE2():AbstractGame (),_hud(0)
{
}

void MGE2::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MGE2::_initializeScene()
{
    //MESHES

    //load a bunch of meshes we will be using throughout this MGE2
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* sphereMeshS = Mesh::load (config::MGE_MODEL_PATH+"sphere_smooth.obj");

    //MATERIALS

    //create some materials to display the cube, the plane and the light
    //AbstractMaterial* lightMaterial = new ColorMaterial (glm::vec3(1,1,0));
    //AbstractMaterial* lightMaterial = new LightMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
    LightMaterial* sphereLightMaterial = new LightMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
    sphereLightMaterial->setSpecular(0.8f, 8.0f);

    // LightMaterial* sphere2LightMaterial = new LightMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
    // sphere2LightMaterial->setSpecular(0.8f, 4.0f);

    AbstractMaterial* redColorMaterial = new ColorMaterial (glm::vec3(1,0,0));
    AbstractMaterial* blueColorMaterial = new ColorMaterial (glm::vec3(0,0,1));
    AbstractMaterial* stoneMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"runicfloor.png"));
    AbstractMaterial* grassMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"land.jpg"));
    AbstractMaterial* brickMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"));

    //SCENE SETUP

    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,6,8));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

    _world->setAmbientLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);

    //add the floor
    GameObject* floorPlane = new GameObject ("plane", glm::vec3(0,-1,0));
    floorPlane->scale(glm::vec3(24,24,24));
    floorPlane->setMesh(planeMeshDefault);
    floorPlane->setMaterial(grassMaterial);
    _world->add(floorPlane);

    GameObject* wallPlane = new GameObject ("plane", glm::vec3(0,-1,-24));
    wallPlane->scale(glm::vec3(24,24,24));
    wallPlane->rotate(glm::radians(90.0f), glm::vec3(1,0,0));
    wallPlane->setMesh(planeMeshDefault);
    wallPlane->setMaterial(brickMaterial);
    _world->add(wallPlane);

    GameObject* leftWallPlane = new GameObject ("plane", glm::vec3(-24,-1,0));
    leftWallPlane->scale(glm::vec3(24,24,24));
    leftWallPlane->rotate(glm::radians(90.0f), glm::vec3(0,0,-1));
    leftWallPlane->setMesh(planeMeshDefault);
    leftWallPlane->setMaterial(brickMaterial);
    _world->add(leftWallPlane);

    GameObject* rightWallPlane = new GameObject ("plane", glm::vec3(24,-1,0));
    rightWallPlane->scale(glm::vec3(24,24,24));
    rightWallPlane->rotate(glm::radians(90.0f), glm::vec3(0,0,1));
    rightWallPlane->setMesh(planeMeshDefault);
    rightWallPlane->setMaterial(brickMaterial);
    _world->add(rightWallPlane);

    // GameObject* leftCube = new GameObject ("cube", glm::vec3(-4.5f,0,-4));
    // leftCube->scale(glm::vec3(0.5f,0.5f,0.5f));
    // leftCube->rotate(glm::radians(45.0f), glm::vec3(0,1,0));
    // leftCube->setMesh(cubeMeshF);
    // leftCube->setMaterial(redColorMaterial);
    // _world->add(leftCube);

    // GameObject* rightCube = new GameObject ("cube", glm::vec3(4.5f,0,-4));
    // rightCube->scale(glm::vec3(0.5f,0.5f,0.5f));
    // rightCube->rotate(glm::radians(45.0f), glm::vec3(0,-1,0));
    // rightCube->setMesh(cubeMeshF);
    // rightCube->setMaterial(blueColorMaterial);
    // _world->add(rightCube);

    //add a spinning sphere
    GameObject* sphere = new GameObject ("sphere", glm::vec3(0,0,0));
    sphere->scale(glm::vec3(1,1,1));
    sphere->setMesh (sphereMeshS);
    sphere->setMaterial(sphereLightMaterial);
    //sphere->setBehaviour (new RotatingBehaviour());
    sphere->setBehaviour(new KeysBehaviour(10));
    //for (int i = 0; i < sphere->getChildCount(); i++) {
        //sphere->setBehaviour(new KeysBehaviour(10));
    //}

    _world->add(sphere);

    GameObject* sphere2 = new GameObject ("sphere", glm::vec3(1,0,0));
    sphere2->setParent(sphere);
    //sphere2->setLocalPosition(sphere2->getParent()->getLocalPosition());
    //sphere2->setBehaviour(new KeysBehaviour(10));
    //sphere2->SetChildBehaviourAsParent();
    sphere2->scale(glm::vec3(1,1,1));
    sphere2->setMesh (sphereMeshS);
    //sphere2->setMaterial(sphere2LightMaterial);
    //sphere2->setBehaviour(new KeysBehaviour(10));
    //_world->add(sphere2);

    camera->setBehaviour(new FollowCameraBehaviour(sphere));

    //add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
    //It's here as a place holder to get you started.
    //Note how the texture material is able to detect the number of lights in the scene
    //even though it doesn't implement any lighting yet!

    Light* light = new Light("light", glm::vec3(0,4,0));
    light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
    light->setMesh(cubeMeshF);
    //light->setMaterial(sphereLightMaterial);
    light->setBehaviour(new KeysBehaviour(25));
    _world->add(light);

    // Light* light2 = new Light("light", glm::vec3(2,4,0));
    // light2->scale(glm::vec3(0.1f, 0.1f, 0.1f));
    // light2->setMesh(cubeMeshF);
    // //light2->setMaterial(sphere2LightMaterial);
    // light2->setBehaviour(new KeysBehaviour(25));
    // _world->add(light2);

}

void MGE2::_render() {
    AbstractGame::_render();
    //_updateHud();
}

void MGE2::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    //_hud->setDebugInfo(debugInfo);
    //_hud->draw();
}

MGE2::~MGE2()
{
	//dtor
}
