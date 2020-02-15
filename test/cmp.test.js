const {MPZ} = require('../');

test('cmp', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        const bi = MPZ(i);

        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            [ j, j.toString(), MPZ(j), BigInt(j) ].forEach(jj => {
                expect(bi.lt(jj)).toBe(i < j);
                expect(bi.le(jj)).toBe(i <= j);
                expect(bi.eq(jj)).toBe(i === j);
                expect(bi.ne(jj)).toBe(i !== j);
                expect(bi.gt(jj)).toBe(i > j);
                expect(bi.ge(jj)).toBe(i >= j);
            });
        });
    });
});
