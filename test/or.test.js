const {MPZ} = require('../');

test('or', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i | j).toString();
            const result = MPZ();

            expect(MPZ(i).or(j).toString()).toBe(ks);
            expect(MPZ(i).or(js).toString()).toBe(ks);
            expect(MPZ(i).or(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).or(BigInt(j)).toString()).toBe(ks);

            MPZ.or(result, i, j);
            expect(result.toString()).toBe(ks);
            MPZ.or(result, i, js);
            expect(result.toString()).toBe(ks);
            MPZ.or(result, MPZ(i), MPZ(j));
            expect(result.toString()).toBe(ks);
        }
    }
});

test('or exceptions', () => {
    expect(() => {
        MPZ.or(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.or(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.or(MPZ(1));
    }).toThrow();
});
