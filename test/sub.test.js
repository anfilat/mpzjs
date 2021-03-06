const MPZ = require('../');

test('sub', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            const js = j.toString();
            const ks = (i - j).toString();
            const result = MPZ();

            expect(MPZ(i).sub(j).toString()).toBe(ks);
            expect(MPZ(i).sub(js).toString()).toBe(ks);
            expect(MPZ(i).sub(MPZ(j)).toString()).toBe(ks);
            expect(MPZ(i).sub(BigInt(j)).toString()).toBe(ks);

            MPZ.sub(result, i, j);
            expect(result.toString()).toBe(ks);
            MPZ.sub(result, i, js);
            expect(result.toString()).toBe(ks);
            MPZ.sub(result, MPZ(i), MPZ(j));
            expect(result.toString()).toBe(ks);
        });
    });

    expect(
        MPZ(
            '635849762218952604062459342660379446997761295162166888134051068531'
            + '9813941775949841573516110003093332652267534768664621969514455380'
            + '8051168706779408804756208386011014197185296'
        ).sub(
            '757617343536280696839135295661092954931163607913400460585109207644'
            + '7966483882748233585856350085641718822741649072106343655764769889'
            + '6399869016678013515043471880323279258685478'
        ).toString()
    ).toBe(
        '-121767581317328092776675953000713507933402312751233572451058139112815'
        + '25421067983920123402400825483861704741143034417216862503145088348700'
        + '309898604710287263494312265061500182'
    );
});

test('sub exceptions', () => {
    expect(() => {
        MPZ.sub(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.sub(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.sub(MPZ(1));
    }).toThrow();
});
