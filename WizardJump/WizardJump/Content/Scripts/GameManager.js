var component = new Component();

component.doesUpdate = true;

component.OnBegin = function () {
    SetCameraPosition(Vector3(0,2.25,0));
    SetCameraZoom(3);

    component.entities = [ Entity(), Entity(), Entity(), Entity(), Entity() ];
    component.entities[0].AddComponent(ScriptComponent("Content/Scripts/BackgroundComponent.js"))
    component.entities[1].SetPosition(Vector3(0,1.5,1));
    component.entities[1].AddComponent(ScriptComponent("Content/Scripts/CharacterComponent.js"))
    component.entities[2].AddComponent(ScriptComponent("Content/Scripts/GroundComponent.js"))
    component.entities[3].SetPosition(Vector3(1,1,1));
    component.entities[3].AddComponent(ScriptComponent("Content/Scripts/GroundComponent.js"))

    component.entities[4].SetPosition(Vector3(-2.5,1,1));
    component.entities[4].SetScale(Vector3(4,1,1));
    component.entities[4].AddComponent(ScriptComponent("Content/Scripts/GroundComponent.js"))

   
}

component.OnUpdate = function(deltaTime) {
   
}

component.OnEnd = function () {
   
}

component;