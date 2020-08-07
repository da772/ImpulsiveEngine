//var obj = require("Content/scripts/TestObject.js").obj;
//var obj2 = require("Content/scripts/TestObject2.js").obj;

var component = new Component();

component.doesUpdate = true;
component.shouldDestroy = false;
component.myNumber = 2;
component.moveSpeed = 5;
/*
component.obj = obj;
component.obj.child.house = "1111 house street";
component.obj1 = obj2;
component.obj1.child.house = "splashScreen";
*/
component.spriteComponent = SpriteComponent();
component.UIComponent = UIComponent();
component.spriteAnimComponent = SpriteAnimationComponent();

component.storage = {degree : 0, comp : SpriteComponent()};
component.spriteDir = -1;

component.OnBegin = function () {
    console.log("Component On Begin!");
    console.log("Does update: " + component.doesUpdate + " Will Destroy? : " + component.shouldDestroy);
    console.log("Component Obj old: " + JSON.stringify(component.obj));
    console.log("Component Obj1 new: " + JSON.stringify(component.obj1));
    
    var point1 = Vector3(0,0,1e5);
    var point2 = Vector3(.5,.5,.5);
    var point3 = point1.Subtract(point2);

    console.log("{"+point3.x+","+point3.y+","+point3.z+"}");

    console.trace("Adding Component: ");
    // Sprites
    component.entity.AddComponent(component.spriteComponent);
    
    var tex = CreateTexture2D("Content/Textures/Anchor.png", 0);
    var meat = "hello";
    var myRes = component.spriteComponent.CreateQuad(Vector3(0,0,0), 0, Vector3(1,1,1), Vector4(1,1,1,1), tex);
    component.spriteComponent.CreateQuad(Vector3(0,.5,0), 0, Vector3(1,1,1), Vector4(1,0,1,1), tex);
    component.spriteComponent.RemoveQuad(myRes);

    component.atlasTex2 = CreateTexture2D("Content/Textures/wizard.png", 0);
    var spText = CreateTexture2DSub(component.atlasTex2, Vector2(0, 0), Vector2(32,32), Vector2(2,1));

    var tex2 = CreateTexture2D("Content/Textures/testLevel01.png", 0);
    component.spriteComponent.CreateQuad(Vector3(0,5.4,-2),0, Vector3(45,15, 1), Vector4(1,1,1,1), tex2);
   


    component.atlasTex = CreateTexture2D("Content/Textures/guard1.png", 0);
    //var spriteTex = CreateTexture2DSub(atlasTex, Vector2(0,0), Vector2(16,16), Vector2(1,1));

    component.mySprite = component.spriteComponent.CreateQuadAtlas(Vector3(-4, 0,0), 0, Vector3(2,2,1), Vector4(1,1,1,1), spText, 1);
    component.mySprite2 = component.spriteComponent.CreateQuadAtlas(Vector3(-4, -2,0), 0, Vector3(2,2,1), Vector4(1,1,1,1), spText, 1);
    component.mySpriteBox1 = component.spriteComponent.CreateQuad(Vector3(-4.1, 0,0), 0, Vector3(1,2,1), Vector4(1,1,1,.25), null);
    //component.spriteComponent.CreateQuad(Vector3(-2,-1.1,-1), 0, Vector3(.5,.6,1), Vector4(0,0,0,1), null);
    SetCameraPosition(Vector3(0,5,0));
    //UI

    var font = CreateFont("Content/Fonts/OpenSans-Regular.ttf", 60)
    component.entity.AddComponent(component.UIComponent);
   // var uiquad = component.UIComponent.CreateQuad(Vector3(0,0,0), 0, Vector3(.2,.2,1), Vector4(1,0,1,1), tex);
    var text = component.UIComponent.CreateText("Javascript\nis\nso\ncool!", font, Vector3(-.5,0,0), Vector3(0,0,0), Vector4(0,1,0,1));
    //console.error(text);
    //component.UIComponent.Remove(text);
    //component.UIComponent.Remove(uiquad);

    // Sprite Anim
    var spriteAnimFunc = function(frame) {
        //component.spriteComponent.RemoveQuad(component.mySprite);
        if (component.spriteDir < 0) {
        var tex = CreateTexture2DSub(component.atlasTex2, Vector2(frame, 0), Vector2(64,74), Vector2(-1,1));
        component.spriteComponent.SetAtlasTexture(component.mySprite, tex);  
        } else {
            var tex = CreateTexture2DSub(component.atlasTex2, Vector2(frame-1, 0), Vector2(64,74), Vector2(1,1));
            component.spriteComponent.SetAtlasTexture(component.mySprite, tex);  
        }
      
        var tex2 = CreateTexture2DSub(component.atlasTex2, Vector2( frame-1, 0), Vector2(64,74), Vector2(1,1));     
        component.spriteComponent.SetAtlasTexture(component.mySprite2, tex2);     
    };

    component.particleProp = ParticleProps();
    component.particleComp = ParticleComponent(component.particleProp);
    component.entity.AddComponent(component.particleComp);

    console.log("Adding anim component!");
    component.entity.AddComponent(component.spriteAnimComponent);
    component.spriteAnimComponent.SetFrameAnimation(8, 8, true, toObject(spriteAnimFunc));

    console.log("Adding collider component");
    component.spriteComponent.CreateQuad(Vector3(4,1,0), 0, Vector3(1,1,1), Vector4(1,1,1,1), null);

    component.quadCollider = QuadColliderComponent(false, Vector2(4, 1), Vector2(1,1), 0);
    component.quadCollider.SetOnCollideStartFunction(toObject(function (c) {  } ));
    component.quadCollider.SetOnCollideEndFunction(toObject(function (c) {  } ));
    
    component.entity.AddComponent(component.quadCollider);
    

    component.spritePos = Vector2(-4, 0);

}

component.OnUpdate = function(deltaTime) {
   
    component.storage.degree += 1 * deltaTime;
    if (component.storage.degree >= 360)
        component.storage.degree  = 0;

    var xPos = .5 * Math.cos(component.storage.degree);
    var yPos = .5 * Math.sin(component.storage.degree);
    
    //console.log(xPos + ", " + yPos);

    //component.entity.SetPosition(Vector3(xPos, yPos, 0));
    

    var mouseX = Input.GetMouseX();
    var mouseY = Input.GetMouseY();
    var mousePos = Input.GetMousePosition();
    var mButton = Input.IsMouseButtonPressed(0);
    var button = Input.IsKeyPressed("1"); // Check that number 1 is pressed
    var touchCount = Input.GetTouchCount();
    var touches = Input.GetTouches();

    if (mButton) {
        //console.log(component.particleComp.Count());
        component.particleComp.Emit(GMath.ScreenPosToWorldPos(Vector2(mouseX, mouseY)), 1);
    }

    if (touchCount > 0) {
        //console.log("Touches: " + touchCount);
        //console.log("Paticles: " + component.particleComp.Count());
        component.particleComp.Emit(GMath.ScreenPosToWorldPos(Vector2(touches[0].GetX(), touches[0].GetY())), 1);
    }

    if (Input.IsKeyPressed("Left")) { //Left        
        component.spriteDir = -1;
        component.spritePos.x -= component.moveSpeed *deltaTime;
        component.spriteComponent.SetPosition(component.mySpriteBox1, Vector2(component.spritePos.x-.1, 0));
        component.spriteComponent.SetPosition(component.mySprite, component.spritePos);
        //console.log(component.spritePos.x);
    }

    if (Input.IsKeyPressed("Right")) { //Right
        component.spriteDir = 1;
        component.spritePos.x += component.moveSpeed *deltaTime;
        component.spriteComponent.SetPosition(component.mySpriteBox1, Vector2(component.spritePos.x+.1, 0));
        component.spriteComponent.SetPosition(component.mySprite, component.spritePos);
        //console.log(component.spritePos.x);
    }


    if (Input.IsKeyPressed("up")) {
        SceneManager.SetScene("emptyScene");
    }

    

    //console.log("MouseX: " + mouseX + " MouseY: " + mouseY + " MousePos: (" + mousePos.x + "," + mousePos.y
    // + ") MouseButton(0): " + mButton + " Button(49): " + button + " TouchCount: " + touchCount + " touches: " + JSON.stringify(touches) );

     //console.log(touches[0].GetID());



   // console.log("Will Destroy? : " + component.shouldDestroy);
    if (component.shouldDestroy) {
       
        //console.warn("destroying!");
        //component.entity.RemoveComponent(component.spriteComponent);
        //component.spriteComponent = null;
        //component.entity.RemoveComponent(component.spriteAnimComponent);
        //component.spriteAnimComponent = null;
        //component.OnBegin = null;
        //component.OnUpdate = null;
        //component.entity.RemoveComponent(component);
        //component.shouldDestroy = false;
        //component.entity.Destroy();
        //SceneManager.SetScene("emptyScene");
    }

}

component.OnEnd = function () {
    component.quadCollider.RemoveOnCollideFunction();
    component.quadCollider.RemoveEndCollideFunction();
    component.spriteAnimComponent.RemoveFrameAnimation();
    console.log("Component Ending");
}

component;