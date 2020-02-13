const test = require('tap').test;
const bigint = require('../');

test('gcd', function (t) {
    const b1 = bigint('234897235923342343242');
    const b2 = bigint('234790237101762305340234');
    const expected = bigint('6');

    t.equal(b1.gcd(b2).toString(), expected.toString());
    t.end();
});
