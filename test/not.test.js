const MPZ = require('../');

test('not', () => {
    expect(MPZ(0).not().toNumber()).toBe(~0);
    expect(MPZ(1).not().toNumber()).toBe(~1);
    expect(MPZ('6456343333436').not().toBigInt()).toBe(~6456343333436n);

    const result = MPZ();

    MPZ.not(result, MPZ(0));
    expect(result.toNumber()).toBe(~0);
    MPZ.not(result, MPZ(1));
    expect(result.toNumber()).toBe(~1);
    MPZ.not(result, MPZ('1111', 2));
    expect(result.toNumber()).toBe(~0b1111);
    MPZ.not(result, 42);
    expect(result.toNumber()).toBe(~42);
});

test('not exceptions', () => {
    expect(() => {
        MPZ.not(1);
    }).toThrow();

    expect(() => {
        MPZ.not(MPZ(1));
    }).toThrow();
});
