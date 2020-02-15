const {MPZ} = require('../');

test('shift', () => {
    expect(MPZ(37).shiftLeft(2).toString()).toEqual((37 << 2).toString()); // 148
    expect(MPZ(37).shiftRight(2).toString()).toEqual((37 >> 2).toString()); // 9

    expect(MPZ(2).pow(Math.pow(2, 10)).shiftRight(4).toString())
        .toEqual(MPZ(2).pow(Math.pow(2, 10)).div(16).toString());
});
