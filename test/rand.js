var test = require('tap').test;
var bigint = require('../');

test('rand', function (t) {
    for (var i = 1; i < 1000; i++) {
        var x = bigint(i).rand().toNumber();
        t.ok(0 <= x  && x < i);
        
        var y = bigint(i).rand(i + 10).toNumber();
        t.ok(i <= y && y < i + 10);
        
        var z = bigint.rand(i, i + 10).toNumber();
        t.ok(i <= z && z < i + 10);
    }
    
    t.end();
});
