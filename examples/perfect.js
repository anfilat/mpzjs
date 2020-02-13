// If 2**n-1 is prime, then (2**n-1) * 2**(n-1) is perfect.
const bigint = require('../');

for (let n = 0; n < 100; n++) {
    const p = bigint.pow(2, n).sub(1);
    if (p.probPrime(50)) {
        const perfect = p.mul(bigint.pow(2, n - 1));
        console.log(perfect.toString());
    }
}
