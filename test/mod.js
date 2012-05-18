var test = require('tap').test;
var bigint = require('../');

test('mod', function (t) {
    t.same(bigint(55555).mod(2).toString(), '1');
    t.same(
        bigint('1234567').mod(
            bigint('4321')
        ).toNumber(), 
        1234567 % 4321
    );
    t.end();
});
