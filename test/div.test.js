const GBI = require('../');

test('div', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = Math.floor(i / j).toString();
            if (ks.match(/^-?\d+$/)) { // ignore exceptions
                const result = GBI();

                expect(GBI(i).div(j).toString()).toEqual(ks);
                expect(GBI(i).div(js).toString()).toEqual(ks);
                expect(GBI(i).div(GBI(j)).toString()).toEqual(ks);

                GBI.div(result, i, j);
                expect(result.toString()).toEqual(ks);
                GBI.div(result, i, js);
                expect(result.toString()).toEqual(ks);
                GBI.div(result, GBI(i), GBI(j));
                expect(result.toString()).toEqual(ks);
            }
        }
    }

    expect(() => {
        GBI.div(1, 2);
    }).toThrow();

    expect(() => {
        GBI.div(GBI(1), GBI(2));
    }).toThrow();

    expect(() => {
        GBI.div(GBI(1));
    }).toThrow();

    expect(
        GBI(
            '433593290010590489671135819286259593426549306666324008679782084292'
            + '2446494189019075159822930571858728009485237489829138626896756141'
            + '8738958337632249177044975686477011571044266'
        ).div(
            '127790264841901718791915669264129510947625523373763053776083279450'
            + '3886212911067061184379695097643279217271150419129022856601771338'
            + '794256383410400076210073482253089544155377'
        ).toString()
    ).toEqual(
        '33'
    );
});
