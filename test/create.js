var test = require('tap').test;
var bigint = require('../');

test('create', function (t) {
    t.same(bigint(1337).toString(), '1337');
    t.same(bigint('1337').toString(), '1337');
    t.same(new bigint('100').toString(), '100');
    t.same(
        new bigint('55555555555555555555555555').toString(),
        '55555555555555555555555555'
    );
    
    t.same(Number(bigint('1e+100').toString()), 1e+100);
    t.same(Number(bigint('1.23e+45').toString()), 1.23e+45);
    for (var i = 0; i < 10; i++) {
        t.same(
            bigint('1.23456e+' + i).toString(),
            Math.floor(1.23456 * Math.pow(10,i))
        );
    }
    
    t.same(bigint('1.23e-45').toString(), '0');

    t.throws(function() { bigint(undefined); });
    t.throws(function() { bigint(null); });
    t.end();
});
