const MPZ = require('../');

test('rand', () => {
    for (let i = 1; i < 100000; i += 2111) {
        const x = MPZ(i).rand().toNumber();
        expect(x).toBeGreaterThanOrEqual(0);
        expect(x).toBeLessThan(i);

        const y = MPZ(i).rand(i + 10).toNumber();
        expect(y).toBeGreaterThanOrEqual(i);
        expect(y).toBeLessThan(i + 10);

        const z = MPZ(i).rand(MPZ(i + 10)).toNumber();
        expect(z).toBeGreaterThanOrEqual(i);
        expect(z).toBeLessThan(i + 10);

        const result = MPZ();

        MPZ.rand(result, i);
        expect(result.toNumber()).toBeGreaterThanOrEqual(0);
        expect(result.toNumber()).toBeLessThan(i);

        MPZ.rand(result, MPZ(i));
        expect(result.toNumber()).toBeGreaterThanOrEqual(0);
        expect(result.toNumber()).toBeLessThan(i);

        MPZ.rand(result, i, i + 10);
        expect(result.toNumber()).toBeGreaterThanOrEqual(i);
        expect(result.toNumber()).toBeLessThan(i + 10);

        MPZ.rand(result, MPZ(i), MPZ(i + 10));
        expect(result.toNumber()).toBeGreaterThanOrEqual(i);
        expect(result.toNumber()).toBeLessThan(i + 10);
    }
});

test('rand exceptions', () => {
    expect(() => {
        MPZ.rand(1, 2, 3);
    }).toThrow();

    expect(() => {
        MPZ.rand(MPZ(1));
    }).toThrow();
});
