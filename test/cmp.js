const test = require('tap').test;
const GBI = require('../');

test('cmp', function (t) {
    for (let i = -10; i <= 10; i++) {
        const bi = GBI(i);

        for (let j = -10; j <= 10; j++) {
            [ j, GBI(j) ].forEach(function (jj) {
                t.same(bi.lt(jj), i < j);
                t.same(bi.le(jj), i <= j);
                t.same(bi.eq(jj), i === j);
                t.same(bi.ne(jj), i !== j);
                t.same(bi.gt(jj), i > j);
                t.same(bi.ge(jj), i >= j);
            });
        }
    }
    t.end();
});
