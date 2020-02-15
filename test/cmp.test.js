const GBI = require('../');

test('cmp', () => {
    for (let i = -10; i <= 10; i++) {
        const bi = GBI(i);

        for (let j = -10; j <= 10; j++) {
            [ j, GBI(j) ].forEach(jj => {
                expect(bi.lt(jj)).toEqual(i < j);
                expect(bi.le(jj)).toEqual(i <= j);
                expect(bi.eq(jj)).toEqual(i === j);
                expect(bi.ne(jj)).toEqual(i !== j);
                expect(bi.gt(jj)).toEqual(i > j);
                expect(bi.ge(jj)).toEqual(i >= j);
            });
        }
    }
});