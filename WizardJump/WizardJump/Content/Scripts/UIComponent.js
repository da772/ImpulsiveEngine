var component = new Component();

component.doesUpdate = true;



component.OnBegin = function () {
   
   component.button1 = ButtonComponent(Vector3(-.75,-.75,0), 0, Vector2(.25,.15), Vector4(1,0,0,1), 1);
   component.button2 = ButtonComponent(Vector3(.75,-.75,0), 0, Vector2(.25,.15), Vector4(1,0,0,1), 1);
   component.button3 = ButtonComponent(Vector3(0,-.75,0), 0, Vector2(.25,.15), Vector4(1,0,0,1), 1);
   

   component.button1.SetMouseEnterFunction(toObject(function (f1, f2) {
        console.log("Button Hovered!");
    }));
    component.button1.SetMouseExitFunction(toObject(function (f1, f2) {
        console.log("Button Hovered!");
    }));

    console.log(component.entity);

   component.entity.AddComponent(component.button1);
   component.entity.AddComponent(component.button2);
   component.entity.AddComponent(component.button3);
   
  
  

}

component.OnUpdate = function(deltaTime) {

}

component.OnEnd = function () {
   console.log("Destroying Imported Component")
   component.button1.RemoveMouseEnterFunction();
   component.button1.RemoveMouseExitFunction();
   component.button2.RemoveMouseEnterFunction();
   component.button2.RemoveMouseExitFunction();
   component.button3.RemoveMouseEnterFunction();
   component.button3.RemoveMouseExitFunction();
}

component;
