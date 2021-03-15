#include "gepch.h"
#include "Public/Core/Application/Components/TransformComponent.h"

namespace GEngine {

    Transform::Transform(Entity* e) : Component(e) {
        go_tag = "Transform Component";
    }

    Transform::~Transform() {
        
    }

    Vector3f Transform::GetPosition() const {
        return this->position;
    };

    Vector3f Transform::GetRotation() const {
        return this->rotation;
    };

    Vector3f Transform::GetScale() const {
        return this->scale;
    };

    void Transform::SetScale(const Vector3f& scale) {
        this->scale = scale;
    }

    void Transform::SetPosition(const Vector3f& position) {
        this->position = position;
    }

    void Transform::SetRotation(const Vector3f& rotation) {
        this->rotation = rotation;
    }


}