const MPZ = require('../');

test('bit length', () => {
  const bl = MPZ(
    '433593290010590489671135819286259593426549306666324008679782084292'
      + '2446494189019075159822930571858728009485237489829138626896756141'
      + '873895833763224917704497568647701157104426'
  ).bitLength();

  expect(bl).toBe(571);

  expect(MPZ(0).bitLength()).toBe(1);
  expect(MPZ(1).bitLength()).toBe(1);
  expect(MPZ(2).bitLength()).toBe(2);
  expect(MPZ(4).bitLength()).toBe(3);
  expect(MPZ(256).bitLength()).toBe(9);
  expect(MPZ(0xFF).bitLength()).toBe(8);
  expect(MPZ(0xFFFF).bitLength()).toBe(16);
  expect(MPZ(0xFFFFFFFF).bitLength()).toBe(32);
  expect(MPZ('FFFFFFFFFFFFFFFF', 16).bitLength()).toBe(64);
});
