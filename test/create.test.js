const {MPZ} = require('../');

test('create', () => {
    expect(MPZ(1337).toNumber()).toBe(1337);
    expect(MPZ(-1337).toNumber()).toBe(-1337);

    expect(MPZ('1337').toNumber()).toBe(1337);
    expect(MPZ('-1337').toNumber()).toBe(-1337);

    expect(new MPZ('100').toNumber()).toBe(100);
    expect(new MPZ('55555555555555555555555555').toString()).toBe('55555555555555555555555555');

    expect(MPZ('1e+100').toNumber()).toBe(1e+100);
    expect(MPZ('1.23e+45').toNumber()).toBe(1.23e+45);

    expect(MPZ(1.23e-45).toNumber()).toEqual(0);
    expect(MPZ('1.23e-45').toNumber()).toEqual(0);
    expect(MPZ(-1.23e-45).toNumber()).toEqual(0);
    expect(MPZ('-1.23e-45').toNumber()).toEqual(0);
    expect(MPZ(-0.6).toNumber()).toEqual(0);
    expect(MPZ('-6e-1').toNumber()).toEqual(0);

    expect(MPZ(MPZ('43856439285743298057234')).toString()).toBe('43856439285743298057234');
    expect(MPZ(BigInt('43856439285743298057234')).toString()).toBe('43856439285743298057234');

    for (let i = 0; i < 10; i++) {
        expect(MPZ('1.23456e+' + i).toNumber()).toEqual(Math.floor(1.23456 * Math.pow(10, i)));
    }

    expect(MPZ().toNumber()).toBe(0);
    expect(MPZ(undefined).toNumber()).toBe(0);
    expect(MPZ(null).toNumber()).toBe(0);
});
