var component = new Component();

component.doesUpdate = true;

component.OnBegin = function () {
    console.log("Component Imported!");

}

component.OnUpdate = function(deltaTime) {

}

component.OnEnd = function () {
   console.log("Destroying Imported Component")
}

component;