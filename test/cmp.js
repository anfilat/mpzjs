var test = require('tap').test;
var bigint = require('../');

test('cmp', function (t) {
    for (var i = -10; i <= 10; i++) {
        var bi = bigint(i);
        
        for (var j = -10; j <= 10; j++) {
            [ j, bigint(j) ].forEach(function (jj) {
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
