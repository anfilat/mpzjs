const MPZ = require('../');

test('and', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i & j).toString();
            const result = MPZ();

            expect(MPZ(i).and(j).toString()).toBe(ks);
            expect(MPZ(i).and(js).toString()).toBe(ks);
            expect(MPZ(i).and(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).and(BigInt(j)).toString()).toBe(ks);

            MPZ.and(result, i, j);
            expect(result.toString()).toBe(ks);
            MPZ.and(result, i, js);
            expect(result.toString()).toBe(ks);
            MPZ.and(result, MPZ(i), MPZ(j));
            expect(result.toString()).toBe(ks);
        }
    }
});

test('and exceptions', () => {
    expect(() => {
        MPZ.and(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.and(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.and(MPZ(1));
    }).toThrow();
});
