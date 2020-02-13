const test = require('tap').test;
const GBI = require('../');

test('create', function (t) {
    t.same(GBI(1337).toString(), '1337');
    t.same(GBI('1337').toString(), '1337');
    t.same(new GBI('100').toString(), '100');
    t.same(
        new GBI('55555555555555555555555555').toString(),
        '55555555555555555555555555'
    );

    t.same(Number(GBI('1e+100').toString()), 1e+100);
    t.same(Number(GBI('1.23e+45').toString()), 1.23e+45);
    for (let i = 0; i < 10; i++) {
        t.same(
            GBI('1.23456e+' + i).toString(),
            Math.floor(1.23456 * Math.pow(10,i))
        );
    }

    t.same(GBI('1.23e-45').toString(), '0');

    //t.throws(function() { GBI(undefined); });
    //t.throws(function() { GBI(null); });
    t.end();
});
