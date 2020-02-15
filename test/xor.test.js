const {MPZ} = require('../');

test('xor', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i ^ j).toString();
            const result = MPZ();

            expect(MPZ(i).xor(j).toString()).toBe(ks);
            expect(MPZ(i).xor(js).toString()).toBe(ks);
            expect(MPZ(i).xor(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).xor(BigInt(j)).toString()).toBe(ks);

            MPZ.xor(result, i, j);
            expect(result.toString()).toBe(ks);
            MPZ.xor(result, i, js);
            expect(result.toString()).toBe(ks);
            MPZ.xor(result, MPZ(i), MPZ(j));
            expect(result.toString()).toBe(ks);
        }
    }
});

test('xor exceptions', () => {
    expect(() => {
        MPZ.xor(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.xor(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.xor(MPZ(1));
    }).toThrow();
});
