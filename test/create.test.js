const GBI = require('../');

test('create', () => {
    expect(GBI(1337).toString()).toEqual('1337');
    expect(GBI('1337').toString()).toEqual('1337');
    expect(new GBI('100').toString()).toEqual('100');
    expect(new GBI('55555555555555555555555555').toString()).toEqual('55555555555555555555555555');

    expect(Number(GBI('1e+100').toString())).toEqual(1e+100);
    expect(Number(GBI('1.23e+45').toString())).toEqual(1.23e+45);
    for (let i = 0; i < 10; i++) {
        expect(GBI('1.23456e+' + i).toNumber()).toEqual(Math.floor(1.23456 * Math.pow(10, i)));
    }

    expect(GBI('1.23e-45').toString()).toEqual('0');

    /*
    expect(() => {
        GBI(undefined);
    }).toThrow();
    expect(() => {
        GBI(null);
    }).toThrow();
    */
});
