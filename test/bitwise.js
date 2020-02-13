const test = require('tap').test;
const bigint = require('../');

test('xor', function (t) {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const is = i.toString();
            const js = j.toString();
            const ks = (i ^ j).toString();
            t.same(bigint(i).xor(j).toString(), ks);
            t.same(bigint(i).xor(js).toString(), ks);
            t.same(bigint(i).xor(bigint(j)).toString(), ks);
            t.same(bigint.xor(i, j).toString(), ks);
        }
    }
    t.end();
});

test('and', function (t) {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const is = i.toString();
            const js = j.toString();
            const ks = (i & j).toString();
            t.same(bigint(i).and(j).toString(), ks);
            t.same(bigint(i).and(js).toString(), ks);
            t.same(bigint(i).and(bigint(j)).toString(), ks);
            t.same(bigint.and(i, j).toString(), ks);
        }
    }
    t.end();
});

test('or', function (t) {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const is = i.toString();
            const js = j.toString();
            const ks = (i | j).toString();
            t.same(bigint(i).or(j).toString(), ks);
            t.same(bigint(i).or(js).toString(), ks);
            t.same(bigint(i).or(bigint(j)).toString(), ks);
            t.same(bigint.or(i, j).toString(), ks);
        }
    }
    t.end();
});
