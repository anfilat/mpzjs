const GBI = require('../');

test('xor', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i ^ j).toString();
            expect(GBI(i).xor(j).toString()).toEqual(ks);
            expect(GBI(i).xor(js).toString()).toEqual(ks);
            expect(GBI(i).xor(GBI(j)).toString()).toEqual(ks);
            expect(GBI.xor(i, j).toString()).toEqual(ks);
        }
    }
});

test('and', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i & j).toString();
            expect(GBI(i).and(j).toString()).toEqual(ks);
            expect(GBI(i).and(js).toString()).toEqual(ks);
            expect(GBI(i).and(GBI(j)).toString()).toEqual(ks);
            expect(GBI.and(i, j).toString()).toEqual(ks);
        }
    }
});

test('or', () => {
    for (let i = -10; i < 10; i++) {
        for (let j = -10; j < 10; j++) {
            const js = j.toString();
            const ks = (i | j).toString();
            expect(GBI(i).or(j).toString()).toEqual(ks);
            expect(GBI(i).or(js).toString()).toEqual(ks);
            expect(GBI(i).or(GBI(j)).toString()).toEqual(ks);
            expect(GBI.or(i, j).toString()).toEqual(ks);
        }
    }
});
