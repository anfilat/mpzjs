const {MPZ} = require('../');

test('gcd', () => {
    const b1 = MPZ('234897235923342343242');
    const b2 = MPZ('234790237101762305340234');
    const expected = MPZ('6');

    expect(b1.gcd(b2).toString()).toEqual(expected.toString());
});
