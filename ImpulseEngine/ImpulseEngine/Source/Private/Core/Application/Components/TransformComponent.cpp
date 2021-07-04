#include "gepch.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Components/TransformComponent.h"

namespace GEngine {

    Transform::Transform(Entity* e) : Component(e) {
        go_tag = "Transform Component";
    }

    Transform::~Transform() {
        
    }

	GEngine::Vector3f Transform::GetWorldPosition() const
	{
        return relativeposition + localposition;
	}

	GEngine::Vector3f Transform::GetWorldRotation() const
	{
        return relativerotation + localrotation;
	}

	GEngine::Vector3f Transform::GetWorldScale() const
	{
        return relativescale * localscale;
	}



	GEngine::Vector3f TransformData::GetWorldPosition() const
	{
		return relativeposition + localposition;
	}

	GEngine::Vector3f TransformData::GetWorldRotation() const
	{
		return relativerotation + localrotation;
	}

	GEngine::Vector3f TransformData::GetWorldScale() const
	{
		return relativescale * localscale;
	}

	Vector3f Transform::GetLocalPosition() const {
        return this->localposition;
    };

    Vector3f Transform::GetLocalRotation() const {
        return this->localrotation;
    };

    Vector3f Transform::GetLocalScale() const {
        return this->localscale;
    };

	GEngine::Vector3f Transform::GetRelativePosition() const
	{
		return this->relativeposition;
	}

	GEngine::Vector3f Transform::GetRelativeRotation() const
	{
		return this->relativerotation;
	}

	GEngine::Vector3f Transform::GetRelativeScale() const
	{
		return this->relativescale;
	}

	void Transform::SetRelativePosition(const Vector3f& position)
	{
        this->relativeposition = position;
		BroadcastTransform();
	}

	void Transform::SetRelativeRotation(const Vector3f& rotation)
	{
        this->relativerotation = rotation;
		BroadcastTransform();
	}

	void Transform::SetRelativeScale(const Vector3f& scale)
	{
        this->relativescale = scale;
		BroadcastTransform();
	}

	void Transform::SetLocalScale(const Vector3f& scale) {
        this->localscale = scale;
		BroadcastTransform();
    }

	void Transform::SetLocalPosition(const Vector3f& position) {
		this->localposition = position;
		BroadcastTransform();
	}

	void Transform::SetLocalRotation(const Vector3f& rotation) {
		this->localrotation = rotation;
		BroadcastTransform();
	}

	void Transform::AddTransformCallback(uint64_t id, std::function<void(Transform*, TransformData)> func)
	{
		m_TransformCallback[id] = func;
	}

	void Transform::RemoveTransformCallback(uint64_t c)
	{
		m_TransformCallback.erase(c);
	}


	void Transform::ResetRelativeTransform()
	{
		this->relativeposition = Vector3f(0.f);
		this->relativerotation= Vector3f(0.f);
		this->relativescale = Vector3f(1.f);

		BroadcastTransform();
	}

	void Transform::BroadcastTransform()
	{
		for (const auto& p : m_TransformCallback) {
			p.second(this, lastData);
		}
		lastData = { localposition, localrotation, localscale, relativeposition, relativerotation, relativescale };
	}




}