#ifndef COMPONENT_TRANSFORM
#define COMPONENT_TRANSFORM
#include "Public/Core/Application/Component.h"


namespace GEngine {
    
    struct TransformData {
        Vector3f position;
        Vector3f rotation;
        Vector3f scale;
    };

    class Transform : public Component {
        public:
            Transform(Entity* e);
            ~Transform();

            Vector3f GetPosition() const;
            Vector3f GetRotation() const;
            Vector3f GetScale() const;

            void SetPosition(const Vector3f& position);
            void SetRotation(const Vector3f& rotation);
            void SetScale(const Vector3f& scale);

        private:
            Vector3f position = Vector3f(0.f);
            Vector3f rotation = Vector3f(0.f);
            Vector3f scale = Vector3f(1.f);



    };
}

#endif