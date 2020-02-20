const MPZ = require('../');

test('shift left', () => {
    expect(MPZ(37).shiftLeft(2).toNumber()).toBe(37 << 2);
    expect(MPZ(37).shiftLeft('2').toNumber()).toBe(37 << 2);

    expect(MPZ(2).pow(Math.pow(2, 10)).shiftLeft(4).toString())
        .toBe(MPZ(2).pow(Math.pow(2, 10)).mul(16).toString());

    const result = MPZ();
    MPZ.shiftLeft(result, 37, 2);
    expect(result.toNumber()).toBe(37 << 2);
    MPZ.shiftLeft(result, 37, '2');
    expect(result.toNumber()).toBe(37 << 2);
});

test('shift right', () => {
    expect(MPZ(37).shiftRight(2).toNumber()).toBe(37 >> 2);
    expect(MPZ(37).shiftRight('2').toNumber()).toBe(37 >> 2);

    expect(MPZ(2).pow(Math.pow(2, 10)).shiftRight(4).toString())
        .toBe(MPZ(2).pow(Math.pow(2, 10)).div(16).toString());

    const result = MPZ();
    MPZ.shiftRight(result, 37, 2);
    expect(result.toNumber()).toBe(37 >> 2);
    MPZ.shiftRight(result, 37, '2');
    expect(result.toNumber()).toBe(37 >> 2);
});

test('shift left exceptions', () => {
    expect(() => {
        MPZ(1).shiftLeft(-1);
    }).toThrow();

    expect(() => {
        MPZ.shiftLeft(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.shiftLeft(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.shiftLeft(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.shiftLeft(MPZ(1), MPZ(2), -1);
    }).toThrow();
});

test('shift right exceptions', () => {
    expect(() => {
        MPZ(1).shiftRight(-1);
    }).toThrow();

    expect(() => {
        MPZ.shiftRight(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.shiftRight(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.shiftRight(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.shiftRight(MPZ(1), MPZ(2), -1);
    }).toThrow();
});
