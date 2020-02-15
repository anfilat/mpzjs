const {MPZ} = require('../');

test('xor', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            const js = j.toString();
            const ks = (i ^ j).toString();

            expect(MPZ(i).xor(j).toString()).toBe(ks);
            expect(MPZ(i).xor(js).toString()).toBe(ks);
            expect(MPZ(i).xor(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).xor(BigInt(j)).toString()).toBe(ks);

            expect(MPZ.xor(i, j).toString()).toBe(ks);
        });
    });
});
