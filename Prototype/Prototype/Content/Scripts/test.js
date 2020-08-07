var obj2 = require("Content/scripts/test2.js").obj;

var obj = new TestObj(21);

console.log("Created Object: " + obj.id);
obj.id = 26;

obj.update = function (dt) {
    console.warn("Updating! :: " + dt); 
    return dt;
    };

obj.func = function(n) {
    console.debug("Func: " + n);
    return n;
}
var f = function () { console.log("we passed this function");};

createTest(f);

obj.linked = obj2;

obj;