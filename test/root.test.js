const {MPZ} = require('../');

test('sqrt and root', () => {
  const r = MPZ(32769);
  const sqrt = r.sqrt();
  const root = r.root(2);
  expect(root.toString()).toEqual('181');
  expect(sqrt.toString()).toEqual('181');
});
