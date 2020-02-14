const GBI = require('../');

test('shift', () => {
    expect(GBI(37).shiftLeft(2).toString()).toEqual((37 << 2).toString()); // 148
    expect(GBI(37).shiftRight(2).toString()).toEqual((37 >> 2).toString()); // 9

    expect(GBI(2).pow(Math.pow(2, 10)).shiftRight(4).toString())
        .toEqual(GBI(2).pow(Math.pow(2, 10)).div(16).toString());
});
