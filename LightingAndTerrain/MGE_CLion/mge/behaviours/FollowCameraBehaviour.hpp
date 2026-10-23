#ifndef FOLLOWCAMERABEHAVIOUR_HPP
#define FOLLOWCAMERABEHAVIOUR_HPP

#include "AbstractBehaviour.hpp"
#include "glm/glm.hpp"

class GameObject;

/**
 * FollowCameraBehaviour
 * ---------------------
 * Makes the attached camera follow a target GameObject from
 * a fixed offset and viewing angle.
 *
 * Optionally, you can add smoothing or rotation later.
 */
class FollowCameraBehaviour : public AbstractBehaviour
{
public:

    FollowCameraBehaviour(GameObject* pTarget);

    virtual ~FollowCameraBehaviour();

    // Called every frame by the engine
    virtual void update(float pStep);

private:
    GameObject* _target;   // The object being followed
    glm::vec3 _offset = glm::vec3(0, 5, 7);  // Default offset (up and back)
    float _angle = 0.2f; //in radian
    bool rotationSet = false;
};

#endif // FOLLOWCAMERABEHAVIOUR_HPP