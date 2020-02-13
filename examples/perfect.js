// If 2**n-1 is prime, then (2**n-1) * 2**(n-1) is perfect.
const GBI = require('../');

for (let n = 0; n < 100; n++) {
    const p = GBI.pow(2, n).sub(1);
    if (p.probPrime(50)) {
        const perfect = p.mul(GBI.pow(2, n - 1));
        console.log(perfect.toString());
    }
}
