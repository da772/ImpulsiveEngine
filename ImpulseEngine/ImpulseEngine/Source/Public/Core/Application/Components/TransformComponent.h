#ifndef COMPONENT_TRANSFORM
#define COMPONENT_TRANSFORM
#include "Public/Core/Application/Components/Scripts/NativeScript.h"


namespace GEngine {
    
    struct TransformData {
		Vector3f localposition = Vector3f(0.f);
		Vector3f localrotation = Vector3f(0.f);
		Vector3f localscale = Vector3f(1.f);
		Vector3f relativeposition = Vector3f(0.f);
		Vector3f relativerotation = Vector3f(0.f);
		Vector3f relativescale = Vector3f(1.f);

		Vector3f GetWorldPosition() const;
		Vector3f GetWorldRotation() const;
		Vector3f GetWorldScale() const;

    };

    class GE_API Transform : public NativeScript {
        public:
            Transform(Entity* e);
            ~Transform();


			Vector3f GetWorldPosition() const;
			Vector3f GetWorldRotation() const;
			Vector3f GetWorldScale() const;

            Vector3f GetLocalPosition() const;
            Vector3f GetLocalRotation() const;
            Vector3f GetLocalScale() const;

			Vector3f GetRelativePosition() const;
			Vector3f GetRelativeRotation() const;
			Vector3f GetRelativeScale() const;

			void SetRelativePosition(const Vector3f& position);
			void SetRelativeRotation(const Vector3f& rotation);
			void SetRelativeScale(const Vector3f& scale);

            void SetLocalPosition(const Vector3f& position);
            void SetLocalRotation(const Vector3f& rotation);
            void SetLocalScale(const Vector3f& scale);


			void AddTransformCallback(ObjectHash, std::function<void(Transform*, TransformData)> func);
			void RemoveTransformCallback(ObjectHash c);

			void ResetRelativeTransform();

        private:
            void BroadcastTransform();

        private:
            Vector3f localposition = Vector3f(0.f);
            Vector3f localrotation = Vector3f(0.f);
            Vector3f localscale = Vector3f(1.f);

			Vector3f relativeposition = Vector3f(0.f);
			Vector3f relativerotation = Vector3f(0.f);
			Vector3f relativescale = Vector3f(1.f);


            TransformData lastData = {localposition, localrotation, localscale, relativeposition, relativerotation, relativescale};

            std::unordered_map<ObjectHash, std::function<void(Transform*, TransformData)>> m_TransformCallback;



    };
}

#endif