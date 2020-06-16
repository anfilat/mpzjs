mpzjs 0.7.1
===========

Arbitrary precision integral arithmetic for node.js.
Based on [node-bigint](https://github.com/substack/node-bigint)

![build & test CI](https://github.com/anfilat/mpzjs/workflows/build%20&%20test%20CI/badge.svg)

This library wraps around [libgmp](https://gmplib.org)'s
[integer functions](https://gmplib.org/manual/Integer-Functions.html#Integer-Functions)
to perform infinite-precision arithmetic.
It can be used with worker threads.

mpzjs is several times faster than BigInt.

Install
=======

You'll need the libgmp to work this package. Under Debian-based systems,

    sudo apt-get install libgmp-dev

On a Mac with [Homebrew](https://github.com/mxcl/homebrew/),

    brew install gmp

And then install with [npm](http://npmjs.org):

    npm install mpzjs

Example
=======

simple.js
---------

    const MPZ = require('mpzjs');
    
    const n = MPZ('782910138827292261791972728324982');
    MPZ.sub(n, n, '182373273283402171237474774728373');
    MPZ.div(n, n, 8);
    
    console.log(n);
    
    const b = MPZ('782910138827292261791972728324982')
        .sub('182373273283402171237474774728373')
        .div(8);
    
    console.log(b);

***
    $ node simple.js
    <MPZ 75067108192986261319312244199576>
    <MPZ 75067108192986261319312244199576>

perfect.js
----------

Generate the perfect numbers:

    // If 2**n-1 is prime, then (2**n-1) * 2**(n-1) is perfect.
    const MPZ = require('mpzjs');

    for (let n = 0; n < 100; n++) {
        const p = MPZ(2).pow(n).sub(1);
        if (p.probPrime(50)) {
            const perfect = p.mul(MPZ(2).pow(n - 1));
            console.log(perfect.toString());
        }
    }

***

    6
    28
    496
    8128
    33550336
    8589869056
    137438691328
    2305843008139952128
    2658455991569831744654692615953842176
    191561942608236107294793378084303638130997321548169216

Limitations
===========

It doesn't work in Windows now.

API
===

There are two sets of methods

Instance methods that create new `MPZ`.
```
const num = value.method(operand);
```

for example
```
const value = MPZ(7);
const result = value.mul(6);
```

And static methods that save the result to the specified variable.
```
MPZ.method(result, value, operand);
```

for example
```
const result = MPZ();
MPZ.mul(result, 7, 6);
```
Static methods are noticeably faster.

MPZ(num, base=10)
-----------------

Create a new `MPZ` from `num` and a base. `num` can be a string, number, BigInt, empty or another `MPZ`.

If you pass in a string you can set the base that string is encoded in.

value.toString(base=10)
-----------------------

Print out the `MPZ` instance in the requested base as a string.

value.toNumber()
----------------

Turn a `MPZ` into a `Number`. If the `MPZ` is too big you'll lose precision or you'll get ±`Infinity`.

value.toBigInt(), value.toJSON()
--------------------------------

Convert `MPZ` to the specified format

value.valueOf()
---------------

Convert `MPZ` to BigInt

MPZ.fromBuffer(buf, opts)
-------------------------

Create a new `MPZ` from a `Buffer`.

The default options are:
```
    {
        order : 'forward', // low-to-high indexed word ordering
        endian : 'big',
        size : 1, // number of bytes in each word
    }
```
Note that endian doesn't matter when size = 1.

value.toBuffer(opts)
--------------------

Return a new `Buffer` with the data from the `MPZ`.

The default options are:
```
    {
        order : 'forward', // low-to-high indexed word ordering
        endian : 'big',
        size : 1, // number of bytes in each word
    }
```
Note that endian doesn't matter when size = 1.

value.set(num), MPZ.set(value, num)
-----------------------------------

Assigns `num` to `value`.

result = value.add(num), MPZ.add(result, value, num)
----------------------------------------------------

Set `result` to `value` plus `num`.

result = value.sub(num), MPZ.sub(result, value, num)
----------------------------------------------------

Set `result` to `value` minus `num`.

result = value.mul(num), MPZ.mul(result, value, num)
----------------------------------------------------

Set `result` to `value` multiplied by `num`.

result = value.div(num), MPZ.div(result, value, num)
----------------------------------------------------

Set `result` to `value` integrally divided by `num`.

result = value.mod(num), MPZ.mod(result, value, num)
----------------------------------------------------

Set `result` to `value` modulo `num`.

MPZ.addMul(result, value1, value2)
----------------------------------

Set `result` to `result` plus `value1` times `value2`.

MPZ.subMul(result, value1, value2)
----------------------------------

Set `result` to `result` minus `value1` times `value2`.

result = value.and(num), MPZ.and(result, value, num)
----------------------------------------------------

Set `result` to `value` bitwise AND (&)-ed with `num`.

result = value.or(num), MPZ.or(result, value, num)
--------------------------------------------------

Set `result` to `value` bitwise inclusive-OR (|)-ed with `num`.

result = value.xor(num), MPZ.xor(result, value, num)
----------------------------------------------------

Set `result` to `value` bitwise exclusive-OR (^)-ed with `num`.

result = value.not(), MPZ.not(result, value)
--------------------------------------------

Set `result` to `value` bitwise NOT (~)ed.

result = value.shiftLeft(num), MPZ.shiftLeft(result, value, num)
----------------------------------------------------------------

Set `result` to `value` multiplied by `2^num`. Equivalent of the `<<` operator.

result = value.shiftRight(num), MPZ.shiftRight(result, value, num)
------------------------------------------------------------------

Set `result` to `value` integrally divided by `2^num`. Equivalent of the `>>` operator.

result = value.abs(), MPZ.abs(result, value)
--------------------------------------------

Set `result` to the absolute value of `value`.

result = value.neg(), MPZ.neg(result, value)
--------------------------------------------

Set `result` to the negative of `value`.

result = value.sqrt(), MPZ.sqrt(result, value)
----------------------------------------------

Set `result` to square root of `value`. This truncates.

result = value.root(nth), MPZ.root(result, value, nth)
------------------------------------------------------

Set `result` to `nth` root of `value`. This truncates.

result = value.pow(exp), MPZ.pow(result, value, exp)
----------------------------------------------------

Set `result` to `value` raised to the `exp` power.

result = value.powm(exp, mod), MPZ.powm(result, value, exp, mod)
----------------------------------------------------------------

Set `result` to `value` raised to the `exp` power modulo `mod`.

value.cmp(num)
--------------

Compare the instance value to `num`. Return a positive integer if `> num`,
a negative integer if `< num`, and 0 if `=== num`.

value.gt(num)
-------------

Return a boolean: whether the instance value is greater than num (`> num`).

value.ge(num)
-------------

Return a boolean: whether the instance value is greater than or equal to num (`>= num`).

value.eq(num)
-------------

Return a boolean: whether the instance value is equal to num (`== num`).

value.lt(num)
-------------

Return a boolean: whether the instance value is less than num (`< num`).

value.le(num)
-------------

Return a boolean: whether the instance value is less than or equal to num (`<= num`).

result = value.rand(upperBound), MPZ.rand(result, lowerBound, upperBound), MPZ.rand(result, upperBound)
-------------------------------------------------------------------------------------------------------

If `upperBound` is supplied, set `result`to a random `MPZ` between the `value` (`lowerBound`)
and `upperBound - 1`, inclusive.

Otherwise, set `result`to a random `MPZ` between 0 and the `value` - 1, inclusive.

value.probPrime()
-----------------

Return whether the `value` is:

* certainly prime (true)
* probably prime ('maybe')
* certainly composite (false)

using [mpz_probab_prime](https://gmplib.org/manual/Number-Theoretic-Functions.html).

result = value.nextPrime(), MPZ.nextPrime(result, value)
--------------------------------------------------------

Set `result` to the next prime greater than `value` using
[mpz_nextprime](https://gmplib.org/manual/Number-Theoretic-Functions.html).

result = value.invert(mod), MPZ.invert(result, value, mod)
----------------------------------------------------------

Compute the multiplicative inverse modulo `mod`.

result = value.gcd(num), MPZ.gcd(result, value, num)
----------------------------------------------------

Set `result` to the greatest common divisor of the `value` with `num`.

value.bitLength()
-----------------

Return the number of bits used to represent the current `MPZ` as a javascript Number.

License
=======

MIT or LGPL-3 license.
