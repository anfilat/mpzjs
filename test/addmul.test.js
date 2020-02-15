const {MPZ} = require('../');

test('addMul', () => {
    [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(i => {
        [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(j => {
            [-100, -10, -5, -1, 0, 1, 5, 10, 100].forEach(k => {
                const value = MPZ();

                value.set(i);
                MPZ.addMul(value, j, k);
                expect(value.toNumber()).toBe(i + j * k);

                value.set(i);
                MPZ.addMul(value, j.toString(), k.toString());
                expect(value.toNumber()).toBe(i + j * k);

                value.set(i);
                MPZ.addMul(value, MPZ(j), MPZ(k));
                expect(value.toNumber()).toBe(i + j * k);
            });
        });
    });
});

test('addMul exceptions', () => {
    expect(() => {
        MPZ.addMul(1, 2);
    }).toThrow();

    expect(() => {
        MPZ.addMul(MPZ(1), MPZ(2));
    }).toThrow();

    expect(() => {
        MPZ.addMul(MPZ(1));
    }).toThrow();
});
