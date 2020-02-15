const GBI = require('../');

test('add', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i + j).toString();
            const result = GBI();

            expect(GBI(i).add(j).toString()).toEqual(ks);
            expect(GBI(i).add(js).toString()).toEqual(ks);
            expect(GBI(i).add(GBI(j)).toString()).toEqual(ks);

            GBI.add(result, i, j);
            expect(result.toString()).toEqual(ks);
            GBI.add(result, i, js);
            expect(result.toString()).toEqual(ks);
            GBI.add(result, GBI(i), GBI(j));
            expect(result.toString()).toEqual(ks);
        }
    }

    expect(() => {
        GBI.add(1, 2);
    }).toThrow();

    expect(() => {
        GBI.add(GBI(1), GBI(2));
    }).toThrow();

    expect(() => {
        GBI.add(GBI(1));
    }).toThrow();

    expect(
        GBI(
            '201781752444966478956292456789265633588628356858680927185287861892'
            + '9889675589272409635031813235465496971529430565627918846694860512'
            + '1492948268400884893722767401972695174353441'
        ).add(
            '939769862972759638577945343130228368606420083646071622223953046277'
            + '3784500359975110887672142614667937014937371109558223563373329424'
            + '0624814097369771481147215472578762824607080'
        ).toString()
    ).toEqual(
        '1141551615417726117534237799919494002195048440504752549409240908170367'
        + '41759492475205227039558501334339864668016751861424100681899362117762'
        + '365770656374869982874551457998960521'
    );
});