var self = new Component();

self.doesUpdate = true;



self.OnBegin = function () {

    self.grounded = false;
    self.characterComponent = self.entity.GetComponents("characterComponent")[0];
    self.quadCollider = self.entity.GetComponents("characterCollider")[0];
    self.groundCollider = self.entity.GetComponents("groundCollider")[0];
    self.groundedCount = 0;

    self.groundCollider.SetOnCollideStartFunction(toObject(function (c) { 
        if (c.tag === "ground") {
            self.groundedCount++;
            self.grounded = self.groundedCount > 0;
            self.characterComponent.SetBool("grounded", true);
            console.log("On Ground - ");
        }
    }));

    self.groundCollider.SetOnCollideEndFunction(toObject(function (c) { 
        if (c.tag === "ground") {
            self.groundedCount--;
            self.grounded = self.groundedCount > 0;
            self.characterComponent.SetBool("grounded", self.grounded);
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
    var jumpForce = 1000;

    if (self.grounded && Input.IsKeyPressed("space") && velocity.y <= .1 && velocity.y >= -.1) {
        self.hasJumped = true;
        self.quadCollider.IncreaseLinearVelocity(0, jumpForce*deltaTime);
    } 
    if (self.grounded && Input.IsKeyPressed("left")) {
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