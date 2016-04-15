var test = require('tap').test
var bigint = require('../')

test('sqrt and root', function (t) {
  var r = bigint(32769)
  var sqrt = r.sqrt()
  var root = r.root(2)
  t.same(root.toString(), '181')
  t.same(sqrt.toString(), '181')
  t.end()
})
