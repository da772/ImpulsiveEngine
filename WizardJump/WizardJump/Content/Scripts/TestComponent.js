var component = new Component();

component.doesUpdate = true;

var hasDeleted = false;

component.OnBegin = function () {
    console.log("Component On Begin!");
    component.e = Entity();
    component.e.AddComponent(ScriptComponent("Content/Scripts/ImportComp.js"));
}

component.OnUpdate = function(deltaTime) {
    if (!hasDeleted) {
        component.e.Destroy();
        hasDeleted = true;
        component.e = null;
    }
}

component.OnEnd = function () {
   
}

component;