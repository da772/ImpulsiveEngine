var self = new Component();

self.doesUpdate = true;


self.testString = "No";



self.OnBegin = function () {

    self.quadCollider = self.entity.GetComponents("characterCollider")[0];
    
    console.log(self.testString);

    if (self.quadCollider) {
        console.log("Sprite Collider Found - " + self.entity.GetComponents("characterCollider"));
    }

    self.quadCollider.SetOnCollideStartFunction(toObject(function (c) { 
        console.debug("Collision Start");    
    }));

    self.quadCollider.SetOnCollideEndFunction(toObject(function (c) { 
        console.debug("Collision End");

    } ));
}

self.OnUpdate = function(deltaTime) {

    var velocity = self.quadCollider.GetLinearVelocity();
    var maxSpeed = 2;
    var acceleration = 5;
    var jumpForce = 50;


    if (Input.IsKeyPressed("space")) {
        
        self.quadCollider.IncreaseLinearVelocity(0, jumpForce*deltaTime);
    } 
    if (Input.IsKeyPressed("left") ) {
        if (velocity.x + -acceleration*deltaTime >= -maxSpeed)
            self.quadCollider.IncreaseLinearVelocity(-acceleration*deltaTime, 0);
        else
            console.log("Max Speed")
    }
     if (Input.IsKeyPressed("right")) {
        if (velocity.x + acceleration*deltaTime <= maxSpeed)
            self.quadCollider.IncreaseLinearVelocity(acceleration*deltaTime, 0);
        else
            console.log("Max Speed")
    }
}

self.OnEnd = function () {
   self.quadCollider.RemoveOnCollideFunction();
   self.quadCollider.RemoveEndCollideFunction();
   console.log("Destroying Rigid self")
}

self;