const GBI = require('../');

test('create', () => {
    expect(GBI(1337).toNumber()).toBe(1337);
    expect(GBI(-1337).toNumber()).toBe(-1337);

    expect(GBI('1337').toNumber()).toBe(1337);
    expect(GBI('-1337').toNumber()).toBe(-1337);

    expect(new GBI('100').toNumber()).toBe(100);
    expect(new GBI('55555555555555555555555555').toString()).toBe('55555555555555555555555555');

    expect(GBI('1e+100').toNumber()).toBe(1e+100);
    expect(GBI('1.23e+45').toNumber()).toBe(1.23e+45);

    expect(GBI(1.23e-45).toNumber()).toEqual(0);
    expect(GBI('1.23e-45').toNumber()).toEqual(0);
    expect(GBI(-1.23e-45).toNumber()).toEqual(0);
    expect(GBI('-1.23e-45').toNumber()).toEqual(0);
    expect(GBI(-0.6).toNumber()).toEqual(0);
    expect(GBI('-6e-1').toNumber()).toEqual(0);

    expect(GBI(GBI('43856439285743298057234')).toString()).toBe('43856439285743298057234');
    expect(GBI(BigInt('43856439285743298057234')).toString()).toBe('43856439285743298057234');

    for (let i = 0; i < 10; i++) {
        expect(GBI('1.23456e+' + i).toNumber()).toEqual(Math.floor(1.23456 * Math.pow(10, i)));
    }

    expect(() => {
        GBI(undefined);
    }).toThrow();
    expect(() => {
        GBI(null);
    }).toThrow();
});
