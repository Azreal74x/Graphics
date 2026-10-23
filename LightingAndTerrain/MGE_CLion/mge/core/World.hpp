#ifndef WORLD_HPP
#define WORLD_HPP

#include "GameObject.hpp"

class Camera;
class Light;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();

        //only used internally, do not use from outside
        void registerLight (Light* pLight);
        void unregisterLight (Light* pLight);

        Light* getLightAt (int pIndex);
        int getLightCount();

		void setAmbientLight(const glm::vec3& color, float intensity);
		glm::vec3 getAmbientColor() const;
		float getAmbientIntensity() const;

	private:
	    Camera* _mainCamera;
	    std::vector<Light*> _lights;

        World(const World&);
        World& operator=(const World&);

		glm::vec3 _ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
		float _ambientIntensity = 0.3f;
};


#endif // WORLD_HPP
