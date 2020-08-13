var self = new Component();

self.doesUpdate = true;
self.spriteDir = 0;

self.colliderPos = {x:.02, y:.00}
self.colliderScale = {x:.4, y:.85}

self.OnBegin = function () {

    
    self.spriteComponent = SpriteComponent();
    self.spriteAnimComponent = SpriteAnimationComponent();
    self.spriteCollider = QuadColliderComponent(true, true, Vector2(self.colliderPos.x, self.colliderPos.y),
         Vector2(self.colliderScale.x,self.colliderScale.y), 0);
    self.spriteCollider.tag = "characterCollider";
    self.rigidBody = ScriptComponent("Content/Scripts/RigidBody.js");
    //self.rigidBody.SetString("testString", "Hello World!");

    self.entity.AddComponent(self.spriteCollider);
    self.entity.AddComponent(self.spriteComponent);
    self.entity.AddComponent(self.rigidBody);

    self.collisionSprite = self.spriteComponent.CreateQuad(Vector3(self.colliderPos.x,self.colliderPos.y,2), 0,
         Vector3(self.colliderScale.x,self.colliderScale.y,1), Vector4(1,0,0,.25), null, 1);
   
    self.characterSprite = self.spriteComponent.CreateQuadAtlas(
        Vector3(0,0,1), 
        0,
        Vector3(1,1,1),
        Vector4(1,1,1,1),
        CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(0, 0), Vector2(64,74), Vector2(1,1)),
        1);

    self.entity.AddComponent(self.spriteAnimComponent);
    self.spriteAnimComponent.SetFrameAnimation(8, 8, true, toObject(
        function(frame) {
            var vel = self.spriteCollider.GetLinearVelocity();
            if (vel.x < -.01) {
                self.spriteComponent.SetPosition(self.characterSprite, Vector2(.04,0));
                self.spriteComponent.SetAtlasTexture(self.characterSprite, CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(frame, 0), Vector2(64,74), Vector2(-1,1)));  
            } else if (vel.x > .01) {
                self.spriteComponent.SetPosition(self.characterSprite, Vector2(.00,0));
                self.spriteComponent.SetAtlasTexture(self.characterSprite, CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(frame-1, 0), Vector2(64,74), Vector2(1,1)));  
            } 

        }
    ));

    
}

self.OnUpdate = function(deltaTime) {
    SetCameraPosition(Vector3(self.entity.GetPosition().x, self.entity.GetPosition().y + .5, 0 ) );
}

self.OnEnd = function () {
   console.log("Destroying Imported self")
   self.spriteAnimComponent.RemoveFrameAnimation();
}

self;
