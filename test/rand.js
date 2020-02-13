const test = require('tap').test;
const bigint = require('../');

test('rand', function (t) {
    for (let i = 1; i < 1000; i++) {
        const x = bigint(i).rand().toNumber();
        t.ok(0 <= x  && x < i);

        const y = bigint(i).rand(i + 10).toNumber();
        t.ok(i <= y && y < i + 10);

        const z = bigint.rand(i, i + 10).toNumber();
        t.ok(i <= z && z < i + 10);
    }

    t.end();
});
