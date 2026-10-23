#include "FollowCameraBehaviour.hpp"
#include "../core/GameObject.hpp"

FollowCameraBehaviour::FollowCameraBehaviour(GameObject *pTarget)
    : AbstractBehaviour(), _target(pTarget) {
    //ctor
}

FollowCameraBehaviour::~FollowCameraBehaviour() {
    //dtor
}

void FollowCameraBehaviour::update(float pStep) {
    glm::vec3 targetPosition = _target->getLocalPosition();
    _owner->setLocalPosition(targetPosition + _offset);

    if (!rotationSet) {
        _owner->rotate(_angle, glm::vec3(1,0,0));
        rotationSet = true;
    }
}
