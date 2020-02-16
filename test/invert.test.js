const {MPZ} = require('../');

test('invert', () => {
    // numbers from http://www.itl.nist.gov/fipspubs/fip186.htm appendix 5
    const q = MPZ('b20db0b101df0c6624fc1392ba55f77d577481e5', 16);
    const k = MPZ('79577ddcaafddc038b865b19f8eb1ada8a2838c6', 16);

    expect(k.invert(q).toString(16))
        .toBe('2784e3d672d972a74e22c67f4f4f726ecc751efa');
    expect(k.invert(1016505658889014629900729618210002584918553821669n).toString(16))
        .toBe('2784e3d672d972a74e22c67f4f4f726ecc751efa');

    const result = MPZ();

    MPZ.invert(result, k, q);
    expect(result.toString(16))
        .toBe('2784e3d672d972a74e22c67f4f4f726ecc751efa');
    MPZ.invert(result, 692739012227105812082439949966529529336373459142n, 1016505658889014629900729618210002584918553821669n);
    expect(result.toString(16))
        .toBe('2784e3d672d972a74e22c67f4f4f726ecc751efa');
});

test('invert exceptions', () => {
    expect(() => {
        MPZ.invert(1, 2, 3);
    }).toThrow();

    expect(() => {
        MPZ.invert(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.invert(MPZ(1), 2);
    }).toThrow();
});
