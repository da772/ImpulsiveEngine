var component = new Component();

component.doesUpdate = false;

component.pos = {x:0, y:0};
component.scale = {x:1, y:1};
component.zOrder = 0;

component.OnBegin = function () {

    component.quadCollider = QuadColliderComponent(false, Vector2(component.pos.x,component.pos.y), Vector2(component.scale.x,component.scale.y), 0);
    component.spriteComponent = SpriteComponent();
    component.entity.AddComponent(component.quadCollider);
    component.entity.AddComponent(component.spriteComponent);
    
    component.collisionSprite = component.spriteComponent.CreateQuad(Vector3(component.pos.x,component.pos.y,component.zOrder), 0,
        Vector3(component.scale.x,component.scale.y,1), Vector4(0,0,0,1), null, 1);

    component.quadCollider.SetOnCollideStartFunction(toObject(function (c) { console.log("Collision Start"); } ));
    component.quadCollider.SetOnCollideEndFunction(toObject(function (c) { console.log("Collision End"); } ));

}

component.OnUpdate = function(deltaTime) {

}

component.OnEnd = function () {
   console.log("Destroying Ground Component")
   component.quadCollider.RemoveOnCollideFunction();
   component.quadCollider.RemoveEndCollideFunction();
}

component;