const {MPZ} = require('../');

test('gcd', () => {
    const b1 = MPZ('234897235923342343242');
    const b2 = MPZ('234790237101762305340234');

    expect(b1.gcd(b2).toBigInt()).toBe(6n);
    expect(b1.gcd('234790237101762305340234').toBigInt()).toBe(6n);

    const result = MPZ();

    MPZ.gcd(result, b1, b2);
    expect(result.toNumber()).toBe(6);
    MPZ.gcd(result, 234897235923342343242n, 234790237101762305340234n);
    expect(result.toNumber()).toBe(6);
});

test('gcd exceptions', () => {
    expect(() => {
        MPZ.gcd(1, 2, 3);
    }).toThrow();

    expect(() => {
        MPZ.gcd(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.gcd(MPZ(1), 2);
    }).toThrow();
});
