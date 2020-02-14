const GBI = require('../');

test('rand', () => {
    for (let i = 1; i < 1000; i++) {
        const x = GBI(i).rand().toNumber();
        expect(x).toBeGreaterThanOrEqual(0);
        expect(x).toBeLessThan(i);

        const y = GBI(i).rand(i + 10).toNumber();
        expect(y).toBeGreaterThanOrEqual(i);
        expect(y).toBeLessThan(i + 10);

        const z = GBI.rand(i, i + 10).toNumber();
        expect(z).toBeGreaterThanOrEqual(i);
        expect(z).toBeLessThan(i + 10);
    }
});
