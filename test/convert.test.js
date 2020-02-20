const inspect = require('util').inspect;
const MPZ = require('../');

test('convert', () => {
  expect(MPZ(42).toString()).toBe('42');
  expect(MPZ(42).toString(10)).toBe('42');
  expect(MPZ(42).toString(2)).toBe('101010');
  expect(MPZ(42).toString(16)).toBe('2a');

  expect(MPZ(42).toNumber()).toBe(42);
  expect(MPZ(42).toBigInt()).toBe(42n);
  expect(MPZ(42).valueOf()).toBe(42n);

  expect(JSON.stringify(MPZ(42))).toBe('"42"');
  expect(JSON.stringify({value: MPZ(42)})).toBe('{"value":"42"}');

  expect(MPZ(42) + ' == 6 * 7').toBe('42 == 6 * 7');
  expect(MPZ(42) + 42n).toBe(84n);

  expect(inspect(MPZ(42))).toBe('<MPZ 42>');

  expect(() => {
    MPZ(42).toString(1);
  }).toThrow();

  expect(() => {
    MPZ(42).toString(63);
  }).toThrow();
});
