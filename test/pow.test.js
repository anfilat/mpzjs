const MPZ = require('../');

test('powm', () => {
    [ 2, '2', MPZ(2), 2n ].forEach(two => {
        [ 100000, '100000', MPZ(100000), 100000n ].forEach(ten => {
            expect(MPZ('111111111').powm(two, ten).toBigInt()).toBe(111111111n ** 2n % 100000n);

            const result = MPZ();
            MPZ.powm(result, MPZ('111111111'), two, ten);
            expect(result.toBigInt()).toBe(111111111n ** 2n % 100000n);
            MPZ.powm(result, 111111111, two, ten);
            expect(result.toBigInt()).toBe(111111111n ** 2n % 100000n);
        });
    });

    expect(MPZ('624387628734576238746587435').powm(2732, '457676874367586').toBigInt())
        .toBe(624387628734576238746587435n ** 2732n % 457676874367586n);
});

test('powm exceptions', () => {
    expect(() => {
        MPZ.powm(1, 2, 3, 4);
    }).toThrow();

    expect(() => {
        MPZ.powm(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.powm(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.powm(MPZ(1), MPZ(2), MPZ(3));
    }).toThrow();

    expect(() => {
        MPZ.powm(MPZ(1), 42, 17, 0);
    }).toThrow();

    expect(() => {
        MPZ(42).powm(17, 0);
    }).toThrow();
});

test('pow', () => {
    [ 2, '2', MPZ(2), 2n ].forEach(two => {
        expect(MPZ('111111111').pow(two).toBigInt()).toBe(111111111n ** 2n);

        const result = MPZ();
        MPZ.pow(result, MPZ('111111111'), two);
        expect(result.toBigInt()).toBe(111111111n ** 2n);
        MPZ.pow(result, 111111111, two);
        expect(result.toBigInt()).toBe(111111111n ** 2n);
    });

    expect(MPZ('3487438743234789234879').pow(22).toBigInt())
        .toBe(3487438743234789234879n ** 22n);
});

test('pow exceptions', () => {
    expect(() => {
        MPZ(10).pow(-2);
    }).toThrow();

    expect(() => {
        MPZ.pow(MPZ(), MPZ(10), -2);
    }).toThrow();

    expect(() => {
        MPZ.pow(1, 2, 3);
    }).toThrow();

    expect(() => {
        MPZ.pow(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.pow(MPZ(1), MPZ(2));
    }).toThrow();
});
