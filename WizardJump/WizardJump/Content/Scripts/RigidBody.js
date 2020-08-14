var self = new Component();

self.doesUpdate = true;



self.OnBegin = function () {

    self.grounded = false;
    self.characterComponent = self.entity.GetComponents("characterComponent")[0];
    self.quadCollider = self.entity.GetComponents("characterCollider")[0];
    self.groundCollider = self.entity.GetComponents("groundCollider")[0];

    self.groundCollider.SetOnCollideStartFunction(toObject(function (c) { 
        if (c.tag === "ground") {
            self.characterComponent.SetBool("grounded", true);
            self.grounded = true;
            console.log("On Ground - ");
        }
    }));

    self.groundCollider.SetOnCollideEndFunction(toObject(function (c) { 
        if (c.tag === "ground") {
            self.characterComponent.SetBool("grounded", false);
            self.grounded = false;
            console.log("Off Ground - ");
        }
    } ));

    self.quadCollider.SetOnCollideStartFunction(toObject(function (c) { 
       // console.debug("Collision Start");    
    }));

    self.quadCollider.SetOnCollideEndFunction(toObject(function (c) { 
       // console.debug("Collision End");
    } ));
}

self.OnUpdate = function(deltaTime) {

    var velocity = self.quadCollider.GetLinearVelocity();
    var maxSpeed = 2;
    var acceleration = 5;
    var jumpForce = 100;

    if (self.grounded && Input.IsKeyPressed("space")) {
        
        self.quadCollider.IncreaseLinearVelocity(0, jumpForce*deltaTime);
    } 
    if (self.grounded && Input.IsKeyPressed("left") ) {
        if (velocity.x + -acceleration*deltaTime >= -maxSpeed)
            self.quadCollider.IncreaseLinearVelocity(-acceleration*deltaTime, 0);
    }
     if (self.grounded && Input.IsKeyPressed("right")) {
        if (velocity.x + acceleration*deltaTime <= maxSpeed)
            self.quadCollider.IncreaseLinearVelocity(acceleration*deltaTime, 0);
    }
}

self.OnEnd = function () {
   self.quadCollider.RemoveOnCollideFunction();
   self.quadCollider.RemoveEndCollideFunction();
   console.log("Destroying Rigid self")
}

self;