const test = require('tap').test;
const GBI = require('../');

test('xor', function (t) {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const is = i.toString();
            const js = j.toString();
            const ks = (i ^ j).toString();
            t.same(GBI(i).xor(j).toString(), ks);
            t.same(GBI(i).xor(js).toString(), ks);
            t.same(GBI(i).xor(GBI(j)).toString(), ks);
            t.same(GBI.xor(i, j).toString(), ks);
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
            t.same(GBI(i).and(j).toString(), ks);
            t.same(GBI(i).and(js).toString(), ks);
            t.same(GBI(i).and(GBI(j)).toString(), ks);
            t.same(GBI.and(i, j).toString(), ks);
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
            t.same(GBI(i).or(j).toString(), ks);
            t.same(GBI(i).or(js).toString(), ks);
            t.same(GBI(i).or(GBI(j)).toString(), ks);
            t.same(GBI.or(i, j).toString(), ks);
        }
    }
    t.end();
});
