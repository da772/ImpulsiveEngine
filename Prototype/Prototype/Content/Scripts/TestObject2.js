var _obj = require("Content/scripts/TestObject.js").obj;

var obj = CreateObject(_obj);

obj.id = 69;
obj.child = {hi:"yes", "Swag":221, "Apartment":"8888 swag st"};

exports.obj = obj;
