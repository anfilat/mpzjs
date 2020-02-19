// Generate two primes p and q to the Digital Signature Standard (DSS)
// http://www.itl.nist.gov/fipspubs/fip186.htm appendix 2.2

const {MPZ} = require('../');
const assert = require('assert');

const q = MPZ(2).pow(159).add(1).rand(MPZ(2).pow(160)).nextPrime();
const L = 512 + 64 * Math.floor(Math.random() * 8);

let p;
do {
    const X = MPZ(2).pow(L - 1).add(1).rand(MPZ(2).pow(L));
    const c = X.mod(q.mul(2));
    p = X.sub(c.sub(1)); // p is congruent to 1 % 2q somehow!
} while (p.lt(MPZ(2).pow(L - 1)) || p.probPrime(50) === false);

assert.ok(q.gt(MPZ(2).pow(159)), 'q > 2**159');
assert.ok(q.lt(MPZ(2).pow(160)), 'q < 2**160');
assert.ok(p.gt(MPZ(2).pow(L - 1)), 'p > 2**(L-1)');
assert.ok(q.lt(MPZ(2).pow(L)), 'p < 2**L');
assert.ok(q.mul(p.sub(1).div(q)).add(1).eq(p), 'q divides p - 1');

assert.ok(p.probPrime(50), 'p is not prime!');
assert.ok(q.probPrime(50), 'q is not prime!');

console.dir({ p : p.toString(), q : q.toString() });
