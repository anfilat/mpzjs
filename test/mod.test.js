const GBI = require('../');

test('mod', () => {
    expect(GBI(55555).mod(2).toString()).toEqual('1');
    expect(GBI('1234567').mod(GBI('4321')).toNumber()).toEqual(1234567 % 4321);
});
