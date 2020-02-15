const {MPZ} = require('../');

test('invertm', () => {
    // numbers from http://www.itl.nist.gov/fipspubs/fip186.htm appendix 5
    const q = MPZ('b20db0b101df0c6624fc1392ba55f77d577481e5', 16);
    const k = MPZ('79577ddcaafddc038b865b19f8eb1ada8a2838c6', 16);
    const kinv = k.invertm(q);
    expect(kinv.toString(16)).toEqual('2784e3d672d972a74e22c67f4f4f726ecc751efa');
});
