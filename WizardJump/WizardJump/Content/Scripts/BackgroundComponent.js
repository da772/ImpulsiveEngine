var component = new Component();

component.doesUpdate = true;

component.OnBegin = function () {

    console.log("CREATING BACKGROUND");

    component.spriteComponent = SpriteComponent();

    component.entity.AddComponent(component.spriteComponent);
   
    component.backgroundSprite = component.spriteComponent.CreateQuad(
        Vector3(0,0,0), 
        0,
        Vector3(10000,10000,1),
        Vector4(.25,.25,.25,1),
        CreateTexture2D("Content/Textures/Checkerboard.png",
        2), 10000);
}

component.OnUpdate = function(deltaTime) {
    
}

component.OnEnd = function () {
   console.log("")
}

component;