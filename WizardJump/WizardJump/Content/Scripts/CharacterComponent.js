var self = new Component();

self.doesUpdate = true;
self.spriteDir = 0;

self.colliderPos = {x:.00, y:.00}
self.colliderScale = {x:.25, y:1}

self.groundPos = {x:.00, y:-.45}
self.groundScale = {x:.25, y:.15}

self.dir = 1;
self.lastWalkFame = 0;
self.grounded = false;


self.OnBegin = function () {

    self.tag = "characterComponent";
    self.spriteComponent = SpriteComponent();
    self.spriteAnimComponent = SpriteAnimationComponent();
    self.spriteCollider = QuadColliderComponent(true, true, Vector2(self.colliderPos.x, self.colliderPos.y),
         Vector2(self.colliderScale.x,self.colliderScale.y), 0);
    self.groundCollider = QuadColliderComponent(true, false, Vector2(self.groundPos.x, self.groundPos.y),
         Vector2(self.groundScale.x,self.groundScale.y), 0);
    self.groundCollider.tag = "groundCollider";
    self.spriteCollider.tag = "characterCollider";
    
    self.rigidBody = ScriptComponent("Content/Scripts/RigidBody.js");

    self.entity.AddComponent(self.spriteCollider);
    self.entity.AddComponent(self.groundCollider);
    self.entity.AddComponent(self.spriteComponent);
    self.entity.AddComponent(self.rigidBody);

    self.collisionSprite = self.spriteComponent.CreateQuad(Vector3(self.colliderPos.x,self.colliderPos.y,2), 0,
         Vector3(self.colliderScale.x,self.colliderScale.y,1), Vector4(1,0,0,.25), null, 1);

    self.groundSprite = self.spriteComponent.CreateQuad(Vector3(self.groundPos.x,self.groundPos.y,2), 0,
         Vector3(self.groundScale.x,self.groundScale.y,2), Vector4(0,1,0,.25), null, 1);
   
    self.characterSprite = self.spriteComponent.CreateQuadAtlas(
        Vector3(0,0,1), 
        0,
        Vector3(1.15,1.15,1),
        Vector4(1,1,1,1),
        CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(0, 0), Vector2(74,74), Vector2(1,1)),
        1);

    self.entity.AddComponent(self.spriteAnimComponent);
    self.spriteAnimComponent.SetFrameAnimation(8, 8, true, toObject(
        function(frame) {
            var vel = self.spriteCollider.GetLinearVelocity();
            if (self.grounded) {
                if (vel.x < -.01 || (vel.x < 0 && self.dir == -1 && (self.lastWalkFame != 7 && self.lastWalkFame != 2) )) {
                    self.spriteComponent.SetAtlasTexture(self.characterSprite,
                        CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(frame, 1), Vector2(74,74), Vector2(-1,1)));
                    self.dir = -1;
                    self.lastWalkFame = frame;
                } else if (vel.x > .01 || (vel.x > 0 && self.dir == 1 && (self.lastWalkFame != 7 && self.lastWalkFame != 2) )) {
                    self.spriteComponent.SetAtlasTexture(self.characterSprite,
                        CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(frame-1,1), Vector2(74,74), Vector2(1,1))); 
                    self.dir = 1;
                    self.lastWalkFame = frame;
                } else if ( (self.lastWalkFame == 7 || self.lastWalkFame == 2) && vel.x > -.01 || vel.x < .01) {
                    self.spriteComponent.SetAtlasTexture(self.characterSprite,
                        CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2(-self.dir < 0 ? 0 : -self.dir,0), Vector2(74,74), Vector2(self.dir,1)));  
                }
            } else {
                self.dir = vel.x > 0 ? 1 : vel.x === 0 ? self.dir : -1;
                self.spriteComponent.SetAtlasTexture(self.characterSprite,
                    CreateTexture2DSub(CreateTexture2D("Content/Textures/wizard.png",0), Vector2( self.dir >= 0 ? 4 : 5,0), Vector2(74,74), Vector2(self.dir,1)));    
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
