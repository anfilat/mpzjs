const {MPZ} = require('../');

test('and', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            const js = j.toString();
            const ks = (i & j).toString();

            expect(MPZ(i).and(j).toString()).toBe(ks);
            expect(MPZ(i).and(js).toString()).toBe(ks);
            expect(MPZ(i).and(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).and(BigInt(j)).toString()).toBe(ks);

            expect(MPZ.and(i, j).toString()).toBe(ks);
        });
    });
});
