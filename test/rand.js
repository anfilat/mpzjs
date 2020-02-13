const test = require('tap').test;
const GBI = require('../');

test('rand', function (t) {
    for (let i = 1; i < 1000; i++) {
        const x = GBI(i).rand().toNumber();
        t.ok(0 <= x  && x < i);

        const y = GBI(i).rand(i + 10).toNumber();
        t.ok(i <= y && y < i + 10);

        const z = GBI.rand(i, i + 10).toNumber();
        t.ok(i <= z && z < i + 10);
    }

    t.end();
});
