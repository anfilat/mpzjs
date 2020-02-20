const {MPZ} = require('../');

test('create', () => {
    expect(MPZ(1337).toNumber()).toBe(1337);
    expect(MPZ(-1337).toNumber()).toBe(-1337);

    expect(MPZ('1337').toNumber()).toBe(1337);
    expect(MPZ('-1337').toNumber()).toBe(-1337);

    expect(new MPZ('100').toNumber()).toBe(100);
    expect(new MPZ('55555555555555555555555555').toString()).toBe('55555555555555555555555555');

    expect(MPZ('65756167457571234567.23456e+0').toString()).toBe('65756167457571234567');
    expect(MPZ('65756167457571234567.23456e+2').toString()).toBe('6575616745757123456723');
    expect(MPZ('65756167457571234567.23456e+20').toString()).toBe('6575616745757123456723456000000000000000');
    expect(MPZ('-65756167457571234567.23456e+2').toString()).toBe('-6575616745757123456723');

    expect(MPZ('1e100').toString())
        .toBe('10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000');
    expect(MPZ('1E100').toString())
        .toBe('10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000');
    expect(MPZ('1e+100').toString())
        .toBe('10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000');
    expect(MPZ('1.23456789e100').toString())
        .toBe('12345678900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000');
    expect(MPZ('-1.23456789e100').toString())
        .toBe('-12345678900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000');
    expect(MPZ('1.23e+45').toString())
        .toBe('1230000000000000000000000000000000000000000000');
    expect(MPZ('0.1e+45').toString())
        .toBe('100000000000000000000000000000000000000000000');
    expect(MPZ('.1e+45').toString())
        .toBe('100000000000000000000000000000000000000000000');

    expect(MPZ(1.23e-45).toNumber()).toBe(0);
    expect(MPZ('1.23e-45').toNumber()).toBe(0);
    expect(MPZ(-1.23e-45).toNumber()).toBe(0);
    expect(MPZ('-1.23e-45').toNumber()).toBe(0);
    expect(MPZ(-0.6).toNumber()).toBe(0);
    expect(MPZ('-6e-1').toNumber()).toBe(0);

    expect(MPZ('65756167457571234567.23456e-2').toString()).toBe('657561674575712345');
    expect(MPZ('-65756167457571234567.23456e-2').toString()).toBe('-657561674575712345');
    expect(MPZ('6575616745757123456723456e-2').toString()).toBe('65756167457571234567234');
    expect(MPZ('65756167457571234567.23456e-19').toNumber()).toBe(6);
    expect(MPZ('-65756167457571234567.23456e-19').toNumber()).toBe(-6);
    expect(MPZ('65756167457571234567.23456e-20').toNumber()).toBe(0);
    expect(MPZ('-65756167457571234567.23456e-20').toNumber()).toBe(0);
    expect(MPZ('65756167457571234567.23456e-22').toNumber()).toBe(0);

    expect(MPZ('43856439285743298057234').valueOf()).toBe(43856439285743298057234n);
    expect(MPZ(43856439285743298057234n).valueOf()).toBe(43856439285743298057234n);
    expect(MPZ(MPZ('43856439285743298057234')).valueOf()).toBe(43856439285743298057234n);

    for (let i = 0; i < 10; i++) {
        expect(MPZ('1.23456e+' + i).toNumber()).toBe(Math.floor(1.23456 * Math.pow(10, i)));
    }

    expect(MPZ().toNumber()).toBe(0);
    expect(MPZ(undefined).toNumber()).toBe(0);
    expect(MPZ(null).toNumber()).toBe(0);
});
