var component = new Component();

component.doesUpdate = true;

component.entityPos = {x:-0 ,y:5, z:0};



component.physics = {
    gravity: -1,
    velocity : { x:0, y:0},
    grounded : false,
    groundedObj : null
}


component.OnBegin = function () {

    component.quadCollider = component.entity.GetComponents("characterCollider")[0];
    component.entityPos.y = component.entity.GetPosition().y;

    if (component.quadCollider) {
        console.log("Sprite Collider Found - " + component.entity.GetComponents("characterCollider"));
    }

    component.quadCollider.SetOnCollideStartFunction(toObject(function (c) { 
        console.log("Hit Object: " + c.GetPosition().x + ", " + c.GetPosition().y + " - " + c.GetScale().x + ", " + c.GetScale().y); 
        console.log("My Object: " + component.quadCollider.GetPosition().x + ", " + component.quadCollider.GetPosition().y + " - " + component.quadCollider.GetScale().x + ", " + component.quadCollider.GetScale().y);

        var norm = Vector2(c.GetPosition().x - component.quadCollider.GetPosition().x, c.GetPosition().y - component.quadCollider.GetPosition().y);
        var v = Math.sqrt( (norm.x*norm.x) + (norm.y*norm.y) );
        norm.x = norm.x/v;
        norm.y = norm.y/v;
        console.log("Direction: " + norm.x + ", " + norm.y);
        console.log("Entity: " + component.entityPos.x + ", " + component.entityPos.y);
        component.physics.velocity.y = 0;
        component.physics.grounded = true
        component.physics.groundedObj = c
        if (norm.y >= .9 || norm.y <= -.9) {
            console.log(Math.sign(component.physics.velocity.y) + ":" + Math.sign(norm.y));
            if (Math.sign(component.physics.velocity.y) === Math.sign(norm.y)) {
               
            }
            
        }

        if (norm.x >= .9 || norm.x <= -.9) {
            component.physics.velocity.x = 0;
        }
    
    }));

    component.quadCollider.SetOnCollideEndFunction(toObject(function (c) { 
        console.log("Collision End");
        if (component.physics.grounded == true) {
            console.log("Grouned False!");
            if (component.physics.groundedObj.self === c.self) {
                console.log("Grouned dasda!");
                component.physics.groundedObj = null;
                component.physics.grounded = false;
            }
        }
    } ));
}

component.OnUpdate = function(deltaTime) {

    
    var jumped = false;
    if (component.physics.grounded) {
        component.physics.velocity.y = 0;
    } 
    if (Input.IsKeyPressed("space") && component.physics.grounded) {
        component.physics.velocity.y += 100*deltaTime;
        jumped = true;
    } 

    if (Input.IsKeyPressed("left") && component.physics.grounded) {
        component.physics.velocity.x = -1;
    } else if (Input.IsKeyPressed("right") && component.physics.grounded) {
        component.physics.velocity.x = 1;
    } else {
        component.physics.velocity.x += -Math.sign(component.physics.velocity.x)*3*deltaTime;
    }

    if (component.physics.grounded === false && !jumped) {
        component.physics.velocity.y += deltaTime*component.physics.gravity;
    }
    if (component.physics.velocity.x !== 0 || component.physics.velocity.y !== 0) {
        component.entityPos.y += deltaTime*component.physics.velocity.y;
        component.entityPos.x += deltaTime*component.physics.velocity.x;

        //console.log("Velocity: (" + component.physics.velocity.x +","+component.physics.velocity.y+")");
        
        component.entity.SetPosition(Vector3(component.entityPos.x, component.entityPos.y, component.entityPos.z));
    }
}

component.OnEnd = function () {
   component.quadCollider.RemoveOnCollideFunction();
   component.quadCollider.RemoveEndCollideFunction();
   console.log("Destroying Rigid Component")
}

component;