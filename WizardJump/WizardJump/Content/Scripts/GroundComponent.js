var self = new Component();

self.doesUpdate = false;

self.pos = {x:0, y:0};
self.scale = {x:1, y:1};
self.zOrder = 0;

self.OnBegin = function () {

    self.quadCollider = QuadColliderComponent(false, true, Vector2(self.pos.x,self.pos.y), Vector2(self.scale.x,self.scale.y), 0);
    self.spriteComponent = SpriteComponent();
    self.entity.AddComponent(self.quadCollider);
    self.entity.AddComponent(self.spriteComponent);
    
    self.collisionSprite = self.spriteComponent.CreateQuad(Vector3(self.pos.x,self.pos.y,self.zOrder), 0,
        Vector3(self.scale.x,self.scale.y,1), Vector4(0,0,0,1), null, 1);

    self.quadCollider.SetOnCollideStartFunction(toObject(function (c) { } ));
    self.quadCollider.SetOnCollideEndFunction(toObject(function (c) {  } ));

}

self.OnUpdate = function(deltaTime) {

}

self.OnEnd = function () {
   console.log("Destroying Ground self")
   self.quadCollider.RemoveOnCollideFunction();
   self.quadCollider.RemoveEndCollideFunction();
}

self;