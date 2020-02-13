const test = require('tap').test;
const GBI = require('../');

test('gcd', function (t) {
    const b1 = GBI('234897235923342343242');
    const b2 = GBI('234790237101762305340234');
    const expected = GBI('6');

    t.equal(b1.gcd(b2).toString(), expected.toString());
    t.end();
});
