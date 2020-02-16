/* Based on https://benchmarksgame-team.pages.debian.net/benchmarksgame/performance/pidigits.html
 *
 * The Computer Language Benchmarks Game
 * https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
 *
 * contributed by Denis Gribov
 *    a translation of the C program contributed by Mr Ledhug
 */

const assert = require('assert');
const {MPZ} = require('../');

const n = +process.argv[2] || 10000;

const char0 = '0'.charCodeAt(0);
const charT = '\t'.charCodeAt(0);
const charN = '\n'.charCodeAt(0);
const charC = ':'.charCodeAt(0);

let bufSize = (10 + 2 + n.toString().length + 1) * (n / 10);
for (let i = 10, ii = 10 ** (Math.log10(n) >>> 0); i < ii; i *= 10) {
    bufSize -= i - 1;
}

console.time('mpz');
const mpzResult = calcByMPZ(n, bufSize).toString();
console.timeEnd('mpz');

console.time('BigInt');
const bigIntResult = calcByBigInt(n, bufSize).toString();
console.timeEnd('BigInt');

assert.ok(mpzResult === bigIntResult);
console.log(mpzResult);

function calcByMPZ(n, bufSize) {
    const buf = Buffer.allocUnsafe(bufSize);
    let bufOffs = 0;

    let i = 0;
    let k = 0;
    let k2 = 1;
    const acc = MPZ(0);
    const den = MPZ(1);
    const num = MPZ(1);
    const tmp = MPZ();
    const d3 = MPZ();
    const d4 = MPZ();

    while (i < n) {
        k++;
        k2 += 2;

        MPZ.addMul(acc, num, 2);
        MPZ.mul(acc, acc, k2);
        MPZ.mul(den, den, k2);
        MPZ.mul(num, num, k);

        if (num.gt(acc)) {
            continue;
        }

        MPZ.mul(tmp, num, 3);
        MPZ.add(tmp, tmp, acc);
        MPZ.div(d3, tmp, den);

        MPZ.add(tmp, tmp, num);
        MPZ.div(d4, tmp, den);

        if (d3.ne(d4)) {
            continue;
        }

        const d = d3.toNumber();
        buf.writeInt8(d + char0, bufOffs++);
        i++;
        if (i % 10 === 0) {
            writeLineEnd(i);
        }

        MPZ.subMul(acc, den, d);
        MPZ.mul(acc, acc, 10);
        MPZ.mul(num, num, 10);
    }

    return buf;

    function writeLineEnd(i) {
        buf.writeInt8(charT, bufOffs++);
        buf.writeInt8(charC, bufOffs++);

        let str = i.toString();
        buf.write(str, bufOffs, bufOffs += str.length);

        buf.writeInt8(charN, bufOffs++);
    }
}

function calcByBigInt(n, bufSize) {
    const buf = Buffer.allocUnsafe(bufSize);
    let bufOffs = 0;

    let i = 0;
    let k = 0;
    let k2 = 1n;
    let acc = 0n;
    let den = 1n;
    let num = 1n;

    while (i < n) {
        k++;
        k2 += 2n;

        acc += num << 1n;
        acc = k2 * acc;
        den = k2 * den;
        num = BigInt(k) * num;

        if (num > acc) {
            continue;
        }

        let tmp = 3n * num + acc;
        const d3 = tmp / den;

        tmp = tmp + num;
        const d4 = tmp / den;

        if (d3 !== d4) {
            continue;
        }

        buf.writeInt8(Number(d3) + char0, bufOffs++);
        i++;
        if (i % 10 === 0) {
            writeLineEnd(i);
        }

        acc -= d3 * den;
        acc = 10n * acc;
        num = 10n * num;
    }

    return buf;

    function writeLineEnd(i) {
        buf.writeInt8(charT, bufOffs++);
        buf.writeInt8(charC, bufOffs++);

        let str = i.toString();
        buf.write(str, bufOffs, bufOffs += str.length);

        buf.writeInt8(charN, bufOffs++);
    }
}
