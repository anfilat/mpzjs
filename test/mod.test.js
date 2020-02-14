const GBI = require('../');

test('mod', () => {
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            const js = j.toString();
            if (!isNaN(i % j)) {
                const ks = (i % j).toString();
                expect(GBI(i).mod(j).toString()).toEqual(ks);
                expect(GBI(i).mod(js).toString()).toEqual(ks);
                expect(GBI(i).mod(GBI(j)).toString()).toEqual(ks);
                expect(GBI.mod(i, j).toString()).toEqual(ks);
            }
        }
    }

    expect(GBI(55555).mod(2).toString()).toEqual('1');
    expect(GBI('1234567').mod(GBI('4321')).toNumber()).toEqual(1234567 % 4321);

    expect(
        GBI('486541542410442549118519277483401413')
            .mod('1802185856709793916115771381388554')
            .toString()
    ).toEqual('1753546955507985683376775889880387');
});
