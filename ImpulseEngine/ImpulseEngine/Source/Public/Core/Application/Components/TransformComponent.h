#ifndef COMPONENT_TRANSFORM
#define COMPONENT_TRANSFORM
#include "Public/Core/Application/Component.h"


namespace GEngine {
    
    struct TransformData {
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
    };

    class Transform : public Component {
        public:
            Transform();
            ~Transform();

            Vector3 GetPosition() const;
            Vector3 GetRotation() const;
            Vector3 GetScale() const;

            void SetPosition(const Vector3& position);
            void SetRotation(const Vector3& rotation);
            void SetScale(const Vector3& scale);

        private:
            Vector3 position = Vector3(0);
            Vector3 rotation = Vector3(0);
            Vector3 scale = Vector3(1);



    };
}

#endif