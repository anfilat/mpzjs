const {MPZ} = require('../');

test('mod', () => {
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            const js = j.toString();
            if (!isNaN(i % j)) {
                const ks = (i % j).toString();
                const result = MPZ();

                expect(MPZ(i).mod(j).toString()).toBe(ks);
                expect(MPZ(i).mod(js).toString()).toBe(ks);
                expect(MPZ(i).mod(MPZ(j)).toString()).toBe(ks);
                expect(MPZ(i).mod(BigInt(j)).toString()).toBe(ks);

                MPZ.mod(result, i, j);
                expect(result.toString()).toBe(ks);
                MPZ.mod(result, i, js);
                expect(result.toString()).toBe(ks);
                MPZ.mod(result, MPZ(i), MPZ(j));
                expect(result.toString()).toBe(ks);
            }
        }
    }

    expect(MPZ(55555).mod(2).toNumber()).toBe(1);
    expect(MPZ('1234567').mod(MPZ('4321')).toNumber()).toBe(1234567 % 4321);

    expect(
        MPZ('486541542410442549118519277483401413')
            .mod('1802185856709793916115771381388554')
            .toString()
    ).toBe('1753546955507985683376775889880387');
});

test('mod exceptions', () => {
    expect(() => {
        MPZ.mod(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.mod(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.mod(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.mod(MPZ(1), 42, 0);
    }).toThrow();

    expect(() => {
        MPZ.mod(MPZ(1), 42, MPZ(0));
    }).toThrow();

    expect(() => {
        MPZ(42).mod(0);
    }).toThrow();

    expect(() => {
        MPZ(42).mod(MPZ(0));
    }).toThrow();
});
