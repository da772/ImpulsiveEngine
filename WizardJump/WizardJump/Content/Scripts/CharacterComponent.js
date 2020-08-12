var component = new Component();

component.doesUpdate = true;
component.spriteDir = 0;

component.colliderPos = {x:.04, y:.00}
component.colliderScale = {x:.4, y:.85}

component.OnBegin = function () {

    component.spriteComponent = SpriteComponent();
    component.spriteAnimComponent = SpriteAnimationComponent();
    component.spriteCollider = QuadColliderComponent(true, Vector2(component.colliderPos.x, component.colliderPos.y),
         Vector2(component.colliderScale.x,component.colliderScale.y), 0);
    component.spriteCollider.tag = "characterCollider";
    component.rigidBody = ScriptComponent("Content/Scripts/RigidBody.js");

    component.entity.AddComponent(component.spriteCollider);
    component.entity.AddComponent(component.spriteComponent);
    component.entity.AddComponent(component.rigidBody);

    component.collisionSprite = component.spriteComponent.CreateQuad(Vector3(component.colliderPos.x,component.colliderPos.y,2), 0,
         Vector3(component.colliderScale.x,component.colliderScale.y,1), Vector4(1,0,0,.25), null, 1);
   
    component.characterSprite = component.spriteComponent.CreateQuadAtlas(
        Vector3(0,0,1), 
        0,
        Vector3(1,1,1),
        Vector4(1,1,1,1),
        CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(0, 0), Vector2(64,74), Vector2(1,1)),
        1);

    component.entity.AddComponent(component.spriteAnimComponent);
    component.spriteAnimComponent.SetFrameAnimation(8, 8, true, toObject(
        function(frame) {
            if (component.spriteDir < 0) {
                component.spriteComponent.SetAtlasTexture(component.characterSprite, CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(frame, 0), Vector2(64,74), Vector2(-1,1)));  
            } else {
                component.spriteComponent.SetAtlasTexture(component.characterSprite, CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(frame-1, 0), Vector2(64,74), Vector2(1,1)));  
            } 
        }
    ));

    
}

component.OnUpdate = function(deltaTime) {
    
}

component.OnEnd = function () {
   console.log("Destroying Imported Component")
   component.spriteAnimComponent.RemoveFrameAnimation();
}

component;
