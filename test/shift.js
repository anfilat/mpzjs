var test = require('tap').test;
var bigint = require('../');

test('shift', function (t) {
    t.same(bigint(37).shiftLeft(2).toString(), (37 << 2).toString()); // 148
    t.same(bigint(37).shiftRight(2).toString(), (37 >> 2).toString()); // 9
    
    t.equal(
        bigint(2).pow(Math.pow(2,10)).shiftRight(4).toString(),
        bigint(2).pow(Math.pow(2,10)).div(16).toString()
    );
    t.end();
});
