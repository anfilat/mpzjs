const MPZ = require('../');

test('sqrt', () => {
  const r = MPZ(32769);

  expect(r.sqrt().toNumber()).toBe(181);

  const result = MPZ();

  MPZ.sqrt(result, r);
  expect(result.toNumber()).toBe(181);
  MPZ.sqrt(result, r.toString());
  expect(result.toNumber()).toBe(181);
});

test('sqrt exceptions', () => {
  expect(() => {
    MPZ.sqrt(1);
  }).toThrow();

  expect(() => {
    MPZ.sqrt(MPZ(1));
  }).toThrow();
});

test('root', () => {
  const r = MPZ(32769);

  expect(r.root(2).toNumber()).toBe(181);
  expect(r.root('2').toNumber()).toBe(181);

  const result = MPZ();

  MPZ.root(result, r, 2);
  expect(result.toNumber()).toBe(181);
  MPZ.root(result, r, '2');
  expect(result.toNumber()).toBe(181);
  MPZ.root(result, 32769, '2');
  expect(result.toNumber()).toBe(181);
  MPZ.root(result, r, 3);
  expect(result.toNumber()).toBe(32);
});

test('root exceptions', () => {
  expect(() => {
    MPZ(10).root(-2);
  }).toThrow();

  expect(() => {
    MPZ.root(1, 2);
  }).toThrow();

  expect(() => {
    MPZ.root(MPZ(1), MPZ(2));
  }).toThrow();

  expect(() => {
    MPZ.root(MPZ(1));
  }).toThrow();

  expect(() => {
    MPZ.root(MPZ(1), MPZ(10), -2);
  }).toThrow();
});
