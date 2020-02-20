const MPZ = require('../');

test('set', () => {
    const result = MPZ(0);

    expect(result.set(42).toNumber()).toBe(42);
    expect(result.set('7').toNumber()).toBe(7);
    expect(result.set(MPZ('7645666454375432058423572733466')).toString()).toBe('7645666454375432058423572733466');
    expect(result.set(74320985432097549328892097589432583n).toString()).toBe('74320985432097549328892097589432583');

    MPZ.set(result, 42);
    expect(result.toString()).toBe('42');
    MPZ.set(result, '-4353425435243652');
    expect(result.toString()).toBe('-4353425435243652');
    MPZ.set(result, MPZ('435354389573249785425435243652'));
    expect(result.toString()).toBe('435354389573249785425435243652');
    MPZ.set(result, 5463872957243905720394n);
    expect(result.toString()).toBe('5463872957243905720394');
});

test('set exceptions', () => {
    expect(() => {
        MPZ.set(1);
    }).toThrow();

    expect(() => {
        MPZ.set(MPZ(1));
    }).toThrow();
});
