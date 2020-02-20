const MPZ = require('../');

test('add', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            const js = j.toString();
            const ks = (i + j).toString();
            const result = MPZ();

            expect(MPZ(i).add(j).toString()).toBe(ks);
            expect(MPZ(i).add(js).toString()).toBe(ks);
            expect(MPZ(i).add(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).add(BigInt(j)).toString()).toBe(ks);

            MPZ.add(result, i, j);
            expect(result.toString()).toBe(ks);
            MPZ.add(result, i, js);
            expect(result.toString()).toBe(ks);
            MPZ.add(result, MPZ(i), MPZ(j));
            expect(result.toString()).toBe(ks);
        });
    });

    expect(
        MPZ(
            '201781752444966478956292456789265633588628356858680927185287861892'
            + '9889675589272409635031813235465496971529430565627918846694860512'
            + '1492948268400884893722767401972695174353441'
        ).add(
            '939769862972759638577945343130228368606420083646071622223953046277'
            + '3784500359975110887672142614667937014937371109558223563373329424'
            + '0624814097369771481147215472578762824607080'
        ).toString()
    ).toBe(
        '1141551615417726117534237799919494002195048440504752549409240908170367'
        + '41759492475205227039558501334339864668016751861424100681899362117762'
        + '365770656374869982874551457998960521'
    );
});

test('add exceptions', () => {
    expect(() => {
        MPZ.add(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.add(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.add(MPZ(1));
    }).toThrow();
});
