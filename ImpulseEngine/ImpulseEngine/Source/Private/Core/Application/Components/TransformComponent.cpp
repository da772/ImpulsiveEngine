#include "gepch.h"
#include "Public/Core/Application/Components/TransformComponent.h"

namespace GEngine {

    Transform::Transform() : Component() {
        m_tag = "Transform";
    }

    Transform::~Transform() {
        
    }

    Vector3 Transform::GetPosition() const {
        return this->position;
    };

    Vector3 Transform::GetRotation() const {
        return this->rotation;
    };

    Vector3 Transform::GetScale() const {
        return this->scale;
    };

    void Transform::SetScale(const Vector3& scale) {
        this->scale = scale;
    }

    void Transform::SetPosition(const Vector3& position) {
        this->position = position;
    }

    void Transform::SetRotation(const Vector3& rotation) {
        this->rotation = rotation;
    }


}