const test = require('tap').test;
const GBI = require('../');

test('mod', function (t) {
    t.same(GBI(55555).mod(2).toString(), '1');
    t.same(
        GBI('1234567').mod(
            GBI('4321')
        ).toNumber(),
        1234567 % 4321
    );
    t.end();
});
