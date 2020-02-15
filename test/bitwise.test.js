const {MPZ} = require('../');

test('xor', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i ^ j).toString();
            expect(MPZ(i).xor(j).toString()).toEqual(ks);
            expect(MPZ(i).xor(js).toString()).toEqual(ks);
            expect(MPZ(i).xor(MPZ(j)).toString()).toEqual(ks);
            expect(MPZ.xor(i, j).toString()).toEqual(ks);
        }
    }
});

test('and', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i & j).toString();
            expect(MPZ(i).and(j).toString()).toEqual(ks);
            expect(MPZ(i).and(js).toString()).toEqual(ks);
            expect(MPZ(i).and(MPZ(j)).toString()).toEqual(ks);
            expect(MPZ.and(i, j).toString()).toEqual(ks);
        }
    }
});

test('or', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i | j).toString();
            expect(MPZ(i).or(j).toString()).toEqual(ks);
            expect(MPZ(i).or(js).toString()).toEqual(ks);
            expect(MPZ(i).or(MPZ(j)).toString()).toEqual(ks);
            expect(MPZ.or(i, j).toString()).toEqual(ks);
        }
    }
});
