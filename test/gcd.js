var test = require('tap').test;
var bigint = require('../');

test('gcd', function (t) {
    var b1 = bigint('234897235923342343242');
    var b2 = bigint('234790237101762305340234');
    var expected = bigint('6');
    
    t.equal(b1.gcd(b2).toString(), expected.toString());
    t.end();
});
