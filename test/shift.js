const test = require('tap').test;
const GBI = require('../');

test('shift', function (t) {
    t.same(GBI(37).shiftLeft(2).toString(), (37 << 2).toString()); // 148
    t.same(GBI(37).shiftRight(2).toString(), (37 >> 2).toString()); // 9

    t.equal(
        GBI(2).pow(Math.pow(2,10)).shiftRight(4).toString(),
        GBI(2).pow(Math.pow(2,10)).div(16).toString()
    );
    t.end();
});
