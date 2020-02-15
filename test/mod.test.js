const {MPZ} = require('../');

test('mod', () => {
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            const js = j.toString();
            if (!isNaN(i % j)) {
                const ks = (i % j).toString();
                expect(MPZ(i).mod(j).toString()).toEqual(ks);
                expect(MPZ(i).mod(js).toString()).toEqual(ks);
                expect(MPZ(i).mod(MPZ(j)).toString()).toEqual(ks);
                expect(MPZ.mod(i, j).toString()).toEqual(ks);
            }
        }
    }

    expect(MPZ(55555).mod(2).toString()).toEqual('1');
    expect(MPZ('1234567').mod(MPZ('4321')).toNumber()).toEqual(1234567 % 4321);

    expect(
        MPZ('486541542410442549118519277483401413')
            .mod('1802185856709793916115771381388554')
            .toString()
    ).toEqual('1753546955507985683376775889880387');
});
