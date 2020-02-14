const GBI = require('../');

test('sub', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i - j).toString();
            const result = GBI();

            expect(GBI(i).sub(j).toString()).toEqual(ks);
            expect(GBI(i).sub(js).toString()).toEqual(ks);
            expect(GBI(i).sub(GBI(j)).toString()).toEqual(ks);

            GBI.sub(result, i, j);
            expect(result.toString()).toEqual(ks);
            GBI.sub(result, i, js);
            expect(result.toString()).toEqual(ks);
            GBI.sub(result, GBI(i), GBI(j));
            expect(result.toString()).toEqual(ks);
        }
    }

    expect(() => {
        GBI.sub(1, 2);
    }).toThrow();

    expect(() => {
        GBI.sub(GBI(1), GBI(2));
    }).toThrow();

    expect(() => {
        GBI.sub(GBI(1));
    }).toThrow();

    expect(
        GBI(
            '635849762218952604062459342660379446997761295162166888134051068531'
            + '9813941775949841573516110003093332652267534768664621969514455380'
            + '8051168706779408804756208386011014197185296'
        ).sub(
            '757617343536280696839135295661092954931163607913400460585109207644'
            + '7966483882748233585856350085641718822741649072106343655764769889'
            + '6399869016678013515043471880323279258685478'
        ).toString()
    ).toEqual(
        '-121767581317328092776675953000713507933402312751233572451058139112815'
        + '25421067983920123402400825483861704741143034417216862503145088348700'
        + '309898604710287263494312265061500182'
    );
});
