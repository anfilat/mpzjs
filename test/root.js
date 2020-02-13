const test = require('tap').test;
const GBI = require('../');

test('sqrt and root', function (t) {
  const r = GBI(32769);
  const sqrt = r.sqrt();
  const root = r.root(2);
  t.same(root.toString(), '181');
  t.same(sqrt.toString(), '181');
  t.end();
});
