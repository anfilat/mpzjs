const {MPZ} = require('../');

test('or', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            const js = j.toString();
            const ks = (i | j).toString();

            expect(MPZ(i).or(j).toString()).toBe(ks);
            expect(MPZ(i).or(js).toString()).toBe(ks);
            expect(MPZ(i).or(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).or(BigInt(j)).toString()).toBe(ks);

            expect(MPZ.or(i, j).toString()).toBe(ks);
        });
    });
});
