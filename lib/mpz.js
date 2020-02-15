const inspect = require('util').inspect;
const {MPZInternal} = require('bindings')('gmpjs.node');

function MPZ(value, base) {
    if (!new.target) {
        return new MPZ(value, base);
    }

    if (value == null) {
        this._value = new MPZInternal();
        return;
    }

    if (value instanceof MPZInternal) {
        this._value = value;
        return;
    }

    if (typeof value === 'number') {
        this._value = new MPZInternal(value);
        return;
    }

    const args = prepareArgs(value, base || 10);
    this._value = new MPZInternal(args.num, args.base);
}

const positiveExponentRE = /e\+?\d/i;
const parseRE = /(.*)e\+?(.*)/i;
const negativeExponentRE = /e-\d/i;

function prepareArgs(num, base) {
    if (typeof num !== 'string') {
        num = num.toString(base);
    }

    if (base === 10) {
        if (positiveExponentRE.test(num)) {
            const parsedNum = parseRE.exec(num);
            const coefficient = parsedNum[1];
            let pow = Number(parsedNum[2]);

            const dotPos = coefficient.indexOf('.');
            if (dotPos !== -1) {
                pow -= coefficient.length - dotPos - 1;
            }

            if (pow < 0) {
                return {
                    num: coefficient.substring(0, coefficient.length + pow).replace('.', ''),
                    base: 10,
                };
            }
            return {
                num: coefficient.replace('.', '') + ''.padEnd(pow, '0'),
                base: 10,
            };
        }
        if (negativeExponentRE.test(num)) {
            return {
                num: Math.trunc(Number(num)).toString(),
                base: 10
            };
        }
    }
    return {
        num,
        base,
    };
}

module.exports = MPZ;

MPZ.prototype.toString = function (base) {
    if (base == null || base === 10) {
        return this._value.toString();
    }

    base = Number(base);
    if (base < 2 || base > 62) {
        throw RangeError('base should be : 2<= base <= 62');
    }
    return this._value.toString(base);
};

MPZ.prototype[inspect.custom] = function () {
    return `<MPZ ${this.toString()}>`;
};

MPZ.prototype.toNumber = function () {
    return this._value.toNumber();
};

MPZ.prototype.toBigInt = function () {
    return BigInt(this._value.toString());
};

MPZ.prototype.valueOf = function () {
    return this.toBigInt();
};

MPZ.prototype.add = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.add(value._value));
    }
    if (typeof value === 'number') {
        return new MPZ(this._value.add(value));
    }
    return new MPZ(this._value.add(new MPZ(value)._value));
};

MPZ.add = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : typeof value2 === 'number'
            ? value2
            : new MPZ(value2)._value;
    result._value.assignAdd(num1, num2);
};

MPZ.prototype.sub = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.sub(value._value));
    }
    if (typeof value === 'number') {
        return new MPZ(this._value.sub(value));
    }
    return new MPZ(this._value.sub(new MPZ(value)._value));
};

MPZ.sub = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : typeof value2 === 'number'
            ? value2
            : new MPZ(value2)._value;
    result._value.assignSub(num1, num2);
};

MPZ.prototype.mul = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.mul(value._value));
    }
    if (typeof value === 'number' && value >= 0) {
        return new MPZ(this._value.mul(value));
    }
    return new MPZ(this._value.mul(new MPZ(value)._value));
};

MPZ.mul = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : typeof value2 === 'number' && value2 >= 0
            ? value2
            : new MPZ(value2)._value;
    result._value.assignMul(num1, num2);
};

MPZ.prototype.div = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.div(value._value));
    }
    if (typeof value === 'number' && value >= 0) {
        return new MPZ(this._value.div(value));
    }
    return new MPZ(this._value.div(new MPZ(value)._value));
};

MPZ.div = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : typeof value2 === 'number' && value2 >= 0
            ? value2
            : new MPZ(value2)._value;
    result._value.assignDiv(num1, num2);
};

MPZ.prototype.mod = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.mod(value._value));
    }
    if (typeof value === 'number' && value >= 0) {
        return new MPZ(this._value.mod(value));
    }
    return new MPZ(this._value.mod(new MPZ(value)._value));
};

MPZ.mod = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : typeof value2 === 'number' && value2 >= 0
            ? value2
            : new MPZ(value2)._value;
    result._value.assignMod(num1, num2);
};

MPZ.prototype.and = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.and(value._value));
    }
    return new MPZ(this._value.and(new MPZ(value)._value));
};

MPZ.and = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : new MPZ(value2)._value;
    result._value.assignAnd(num1, num2);
};

MPZ.prototype.or = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.or(value._value));
    }
    return new MPZ(this._value.or(new MPZ(value)._value));
};

MPZ.or = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : new MPZ(value2)._value;
    result._value.assignOr(num1, num2);
};

MPZ.prototype.xor = function (value) {
    if (value instanceof MPZ) {
        return new MPZ(this._value.xor(value._value));
    }
    return new MPZ(this._value.xor(new MPZ(value)._value));
};

MPZ.xor = function (result, value1, value2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (value2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value1 instanceof MPZ
        ? value1._value
        : new MPZ(value1)._value;
    const num2 = value2 instanceof MPZ
        ? value2._value
        : new MPZ(value2)._value;
    result._value.assignXor(num1, num2);
};

MPZ.prototype.shiftLeft = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num >= 0) {
        return new MPZ(this._value.shiftLeft(num));
    }
    throw RangeError('shift should be > 0');
};

MPZ.shiftLeft = function (result, value, shift) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }
    if (shift == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    const num2 = typeof shift === 'number'
        ? shift
        : parseInt(shift.toString(), 10);
    if (num2 >= 0) {
        return new MPZ(result._value.assignShiftLeft(num1, num2));
    }
    throw RangeError('shift should be > 0');
};

MPZ.prototype.shiftRight = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num >= 0) {
        return new MPZ(this._value.shiftRight(num));
    }
    throw RangeError('shift should be > 0');
};

MPZ.shiftRight = function (result, value, shift) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }
    if (shift == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    const num2 = typeof shift === 'number'
        ? shift
        : parseInt(shift.toString(), 10);
    if (num2 >= 0) {
        return new MPZ(result._value.assignShiftRight(num1, num2));
    }
    throw RangeError('shift should be > 0');
};

MPZ.prototype.abs = function () {
    return new MPZ(this._value.babs());
};

MPZ.abs = function (value) {
    if (value instanceof MPZ) {
        return value.abs();
    }
    return new MPZ(value).abs();
};

MPZ.prototype.neg = function () {
    return new MPZ(this._value.bneg());
};

MPZ.neg = function (value) {
    if (value instanceof MPZ) {
        return value.neg();
    }
    return new MPZ(value).neg();
};

MPZ.prototype.bitLength = function () {
    return this._value.bitLength();
};

MPZ.bitLength = function (value) {
    if (value instanceof MPZ) {
        return value.bitLength();
    }
    return new MPZ(value).bitLength();
};

MPZ.prototype.powm = function (num, mod) {
    const m = normalizeValue(mod)._value;

    if (num instanceof MPZ) {
        return new MPZ(this._value.bpowm(num._value, m));
    }
    const type = typeof num;
    if (type === 'number') {
        return new MPZ(this._value.upowm(num, m));
    }
    if (type === 'string') {
        return new MPZ(this._value.bpowm(new MPZ(num)._value, m));
    }
    throw new TypeError(`Unspecified powm for type ${type} for m`);
};

function normalizeValue(value) {
    if (value instanceof MPZ) {
        return value;
    }
    const type = typeof value;
    if (type === 'number' || type === 'string') {
        return new MPZ(value);
    }
    throw new TypeError(`Unspecified operation for type ${type} for n`);
}

MPZ.powm = function (value, num, mod) {
    if (value instanceof MPZ) {
        return value.powm(num, mod);
    }
    return new MPZ(value).powm(num, mod);
};

MPZ.prototype.pow = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num >= 0) {
        return new MPZ(this._value.upow(num));
    }
    return this.powm(num, this);
};

MPZ.pow = function (value, num) {
    if (value instanceof MPZ) {
        return value.pow(num);
    }
    return new MPZ(value).pow(num);
};

MPZ.prototype.cmp = function (num) {
    if (num instanceof MPZ) {
        return this._value.bcompare(num._value);
    }
    if (typeof num === 'number') {
        if (num < 0) {
            return this._value.scompare(num);
        }
        return this._value.ucompare(num);
    }
    return this._value.bcompare(new MPZ(num)._value);
};

MPZ.cmp = function (value, num) {
    if (value instanceof MPZ) {
        return value.cmp(num);
    }
    return new MPZ(value).cmp(num);
};

MPZ.prototype.gt = function (num) {
    return this.cmp(num) > 0;
};

MPZ.prototype.ge = function (num) {
    return this.cmp(num) >= 0;
};

MPZ.prototype.eq = function (num) {
    return this.cmp(num) === 0;
};

MPZ.prototype.ne = function (num) {
    return this.cmp(num) !== 0;
};

MPZ.prototype.lt = function (num) {
    return this.cmp(num) < 0;
};

MPZ.prototype.le = function (num) {
    return this.cmp(num) <= 0;
};

MPZ.prototype.sqrt = function() {
    return new MPZ(this._value.bsqrt());
};

MPZ.sqrt = function (value) {
    if (value instanceof MPZ) {
        return value.sqrt();
    }
    return new MPZ(value).sqrt();
};

MPZ.prototype.root = function(num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    return new MPZ(this._value.broot(num));
};

MPZ.root = function (value, num) {
    if (value instanceof MPZ) {
        return value.root(num);
    }
    return new MPZ(value).root(num);
};

MPZ.prototype.rand = function (to) {
    if (to == null) {
        if (this.toNumber() === 1) {
            return new MPZ(0);
        }
        return new MPZ(this._value.brand0());
    }

    const x = to instanceof MPZ
        ? to.sub(this)
        : new MPZ(to).sub(this);
    return new MPZ(x._value.brand0()).add(this);
};

MPZ.rand = function (value, to) {
    if (value instanceof MPZ) {
        return value.rand(to);
    }
    return new MPZ(value).rand(to);
};

MPZ.prototype.invertm = function (mod) {
    if (mod instanceof MPZ) {
        return new MPZ(this._value.binvertm(mod._value));
    }
    return new MPZ(this._value.binvertm(new MPZ(mod)._value));
};

MPZ.invertm = function (value, mod) {
    if (value instanceof MPZ) {
        return value.invertm(mod);
    }
    return new MPZ(value).invertm(mod);
};

MPZ.prototype.probPrime = function (reps) {
    const n = this._value.probprime(reps || 10);
    return { 0: false, 1: 'maybe', 2: true }[n];
};

MPZ.probPrime = function (value, reps) {
    if (value instanceof MPZ) {
        return value.probPrime(reps);
    }
    return new MPZ(value).probPrime(reps);
};

MPZ.prototype.nextPrime = function () {
    return new MPZ(this._value.nextprime());
};

MPZ.nextPrime = function (value) {
    if (value instanceof MPZ) {
        return value.nextPrime();
    }
    return new MPZ(value).nextPrime();
};

MPZ.prototype.gcd = function (num) {
    return new MPZ(this._value.bgcd(num._value));
};

MPZ.gcd = function (value, num) {
    if (value instanceof MPZ) {
        return value.gcd(num);
    }
    return new MPZ(value).gcd(num);
};

MPZ.fromBuffer = function (buf, opts) {
    if (!opts) {
        opts = {};
    }

    const endian = { 1: 'big', '-1': 'little' }[opts.endian]
        || opts.endian || 'big';

    const size = opts.size || 1;

    if (buf.length % size !== 0) {
        throw new RangeError(`Buffer length (${buf.length}) must be a multiple of size (${size})`);
    }

    const hex = [];
    for (let i = 0; i < buf.length; i += size) {
        const chunk = [];
        for (let j = 0; j < size; j++) {
            chunk.push(buf[i + (endian === 'big' ? j : (size - j - 1))]);
        }

        hex.push(chunk
            .map(c => (c < 16 ? '0' : '') + c.toString(16))
            .join('')
        );
    }

    return new MPZ(hex.join(''), 16);
};

MPZ.prototype.toBuffer = function (opts) {
    if (typeof opts === 'string') {
        if (opts !== 'mpint') {
            return 'Unsupported Buffer representation';
        }

        const abs = this.abs();
        const buf = abs.toBuffer({ size: 1, endian: 'big' });
        let len = buf.length === 1 && buf[0] === 0 ? 0 : buf.length;
        if ((buf[0] & 0x80) === 0x80) {
            len ++;
        }

        const ret = Buffer.alloc(4 + len);
        if (len > 0) {
            buf.copy(ret, 4 + ((buf[0] & 0x80) === 0x80 ? 1 : 0));
        }
        if ((buf[0] & 0x80) === 0x80) {
            ret[4] = 0;
        }

        ret[0] = len & (0xff << 24);
        ret[1] = len & (0xff << 16);
        ret[2] = len & (0xff << 8);
        ret[3] = len & (0xff << 0);

        // two's compliment for negative integers:
        const isNeg = this.lt(0);
        if (isNeg) {
            for (let i = 4; i < ret.length; i++) {
                ret[i] = 0xff - ret[i];
            }
        }
        ret[4] = (ret[4] & 0x7f) | (isNeg ? 0x80 : 0);
        if (isNeg) {
            ret[ret.length - 1] ++;
        }

        return ret;
    }

    if (!opts) {
        opts = {};
    }

    const endian = { 1: 'big', '-1': 'little' }[opts.endian]
        || opts.endian || 'big';
    const size = opts.size || 1;

    let hex = this.toString(16);
    if (hex.charAt(0) === '-') {
        throw new Error('converting negative numbers to Buffers not supported yet');
    }

    const len = Math.ceil(hex.length / (2 * size)) * size;
    const buf = Buffer.alloc(len);

    // zero-pad the hex string so the chunks are all `size` long
    while (hex.length < 2 * len) {
        hex = '0' + hex;
    }

    const hx = hex
        .split(new RegExp('(.{' + (2 * size) + '})'))
        .filter(s => s.length > 0);

    hx.forEach((chunk, i) => {
        for (let j = 0; j < size; j++) {
            const ix = i * size + (endian === 'big' ? j : size - j - 1);
            buf[ix] = parseInt(chunk.slice(j * 2, j * 2 + 2), 16);
        }
    });

    return buf;
};
