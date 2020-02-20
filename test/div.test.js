const MPZ = require('../');

test('div', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            const js = j.toString();
            const ks = Math.floor(i / j).toString();
            if (ks.match(/^-?\d+$/)) { // ignore exceptions
                const result = MPZ();

                expect(MPZ(i).div(j).toString()).toBe(ks);
                expect(MPZ(i).div(js).toString()).toBe(ks);
                expect(MPZ(i).div(MPZ(j)).toString()).toBe(ks);
                expect(MPZ(i).div(BigInt(j)).toString()).toBe(ks);

                MPZ.div(result, i, j);
                expect(result.toString()).toBe(ks);
                MPZ.div(result, i, js);
                expect(result.toString()).toBe(ks);
                MPZ.div(result, MPZ(i), MPZ(j));
                expect(result.toString()).toBe(ks);
            }
        });
    });

    expect(
        MPZ(
            '433593290010590489671135819286259593426549306666324008679782084292'
            + '2446494189019075159822930571858728009485237489829138626896756141'
            + '8738958337632249177044975686477011571044266'
        ).div(
            '127790264841901718791915669264129510947625523373763053776083279450'
            + '3886212911067061184379695097643279217271150419129022856601771338'
            + '794256383410400076210073482253089544155377'
        ).toString()
    ).toBe(
        '33'
    );
});

test('div exceptions', () => {
    expect(() => {
        MPZ.div(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.div(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.div(MPZ(1));
    }).toThrow();

    expect(() => {
        MPZ.div(MPZ(1), 42, 0);
    }).toThrow();

    expect(() => {
        MPZ.div(MPZ(1), 42, MPZ(0));
    }).toThrow();

    expect(() => {
        MPZ(42).div(0);
    }).toThrow();

    expect(() => {
        MPZ(42).div(MPZ(0));
    }).toThrow();
});
