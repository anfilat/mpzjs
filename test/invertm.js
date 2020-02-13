const test = require('tap').test;
const GBI = require('../');

test('invertm', function (t) {
    // numbers from http://www.itl.nist.gov/fipspubs/fip186.htm appendix 5
    const q = GBI('b20db0b101df0c6624fc1392ba55f77d577481e5', 16);
    const k = GBI('79577ddcaafddc038b865b19f8eb1ada8a2838c6', 16);
    const kinv = k.invertm(q);
    t.same(kinv.toString(16), '2784e3d672d972a74e22c67f4f4f726ecc751efa');
    t.end();
});
