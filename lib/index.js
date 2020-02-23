const inspect = require('util').inspect;
const {MPZInternal} = require('node-gyp-build')(__dirname + '/..');

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

const exponentRE = /e[-+]?\d/i;
const parseRE = /(.*)e(.*)/i;

function prepareArgs(num, base) {
    if (typeof num !== 'string') {
        num = num.toString(base);
    }

    if (base === 10 && exponentRE.test(num)) {
        const parsedNum = parseRE.exec(num);
        const coefficient = parsedNum[1];
        let pow = Number(parsedNum[2]);

        const dotPos = coefficient.indexOf('.');
        if (dotPos !== -1) {
            if (pow < 0) {
                pow -= coefficient.length - dotPos;
            } else {
                pow -= coefficient.length - dotPos - 1;
            }
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
        throw RangeError('base must be : 2<= base <= 62');
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

MPZ.prototype.toJSON = function () {
    return this.toString();
};

MPZ.prototype.valueOf = function () {
    return this.toBigInt();
};

MPZ.prototype.set = function (value) {
    if (value instanceof MPZ) {
        this._value.set(value._value);
    }
    if (typeof value === 'number') {
        this._value.set(value);
    }
    this._value.set(new MPZ(value)._value);
    return this;
};

MPZ.set = function (result, value) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }
    result.set(value);
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

MPZ.addMul = function (result, value1, value2) {
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
    result._value.assignAddMul(num1, num2);
};

MPZ.subMul = function (result, value1, value2) {
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
    result._value.assignSubMul(num1, num2);
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

MPZ.prototype.not = function () {
    return new MPZ(this._value.not());
};

MPZ.not = function (result, value) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }

    const num = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    result._value.assignNot(num);
};

MPZ.prototype.shiftLeft = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num < 0) {
        throw RangeError('Shift must be positive');
    }
    return new MPZ(this._value.shiftLeft(num));
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
    if (num2 < 0) {
        throw RangeError('Shift must be positive');
    }
    result._value.assignShiftLeft(num1, num2);
};

MPZ.prototype.shiftRight = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num < 0) {
        throw RangeError('Shift must be positive');
    }
    return new MPZ(this._value.shiftRight(num));
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
    if (num2 < 0) {
        throw RangeError('Shift must be positive');
    }
    result._value.assignShiftRight(num1, num2);
};

MPZ.prototype.abs = function () {
    return new MPZ(this._value.abs());
};

MPZ.abs = function (result, value) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }

    const num = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    result._value.assignAbs(num);
};

MPZ.prototype.neg = function () {
    return new MPZ(this._value.neg());
};

MPZ.neg = function (result, value) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }

    const num = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    result._value.assignNeg(num);
};

MPZ.prototype.sqrt = function() {
    return new MPZ(this._value.sqrt());
};

MPZ.sqrt = function (result, value) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }

    const num = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    result._value.assignSqrt(num);
};

MPZ.prototype.root = function(num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num < 0) {
        throw RangeError('Root degree must be positive');
    }
    return new MPZ(this._value.root(num));
};

MPZ.root = function (result, value, num) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (value == null) {
        throw new TypeError('Second operand is missed');
    }
    if (num == null) {
        throw new TypeError('Third operand is missed');
    }

    const num1 = value instanceof MPZ
        ? value._value
        : new MPZ(value)._value;
    const num2 = typeof num === 'number'
        ? num
        : parseInt(num.toString(), 10);
    if (num2 < 0) {
        throw RangeError('Root degree must be positive');
    }
    result._value.assignRoot(num1, num2);
};

MPZ.prototype.powm = function (exp, mod) {
    const normMod = mod instanceof MPZ
        ? mod._value
        : new MPZ(mod)._value;

    if (exp instanceof MPZ) {
        return new MPZ(this._value.powm(exp._value, normMod));
    }
    if (typeof exp === 'number' && exp >= 0) {
        return new MPZ(this._value.powm(exp, normMod));
    }
    return new MPZ(this._value.powm(new MPZ(exp)._value, normMod));
};

MPZ.powm = function (result, base, exp, mod) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (base == null) {
        throw new TypeError('base operand is missed');
    }
    if (exp == null) {
        throw new TypeError('exp operand is missed');
    }
    if (mod == null) {
        throw new TypeError('mod operand is missed');
    }

    const normBase = base instanceof MPZ
        ? base._value
        : new MPZ(base)._value;
    const normExp = exp instanceof MPZ
        ? exp._value
        : typeof exp === 'number' && exp >= 0
            ? exp
            : new MPZ(exp)._value;
    const normMod = mod instanceof MPZ
        ? mod._value
        : new MPZ(mod)._value;
    result._value.assignPowm(normBase, normExp, normMod);
};

MPZ.prototype.pow = function (exp) {
    if (typeof exp !== 'number') {
        exp = parseInt(exp.toString(), 10);
    }

    if (exp < 0) {
        throw RangeError('Exponent must be positive');
    }
    return new MPZ(this._value.pow(exp));
};

MPZ.pow = function (result, base, exp) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (base == null) {
        throw new TypeError('base operand is missed');
    }
    if (exp == null) {
        throw new TypeError('exp operand is missed');
    }

    const normBase = base instanceof MPZ
        ? base._value
        : new MPZ(base)._value;
    const normExp = typeof exp === 'number'
        ? exp
        : parseInt(exp.toString(), 10);
    if (normExp < 0) {
        throw RangeError('Exponent must be positive');
    }
    result._value.assignPow(normBase, normExp);
};

MPZ.prototype.cmp = function (num) {
    if (num instanceof MPZ) {
        return this._value.compare(num._value);
    }
    if (typeof num === 'number') {
        return this._value.compare(num);
    }
    return this._value.compare(new MPZ(num)._value);
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

MPZ.prototype.rand = function (to) {
    if (to == null) {
        if (this.toNumber() === 1) {
            return new MPZ(0);
        }
        return new MPZ(this._value.rand());
    }

    const n = to instanceof MPZ
        ? to.sub(this)
        : new MPZ(to).sub(this);
    return new MPZ(n._value.rand()).add(this);
};

MPZ.rand = function (result, from, to) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (from == null) {
        throw new TypeError('Second operand is missed');
    }

    if (to == null) {
        const normTo = from instanceof MPZ
            ? from._value
            : new MPZ(from)._value;
        result._value.assignRand(normTo);
        return;
    }

    const normFrom = from instanceof MPZ
        ? from._value
        : new MPZ(from)._value;
    const normTo = to instanceof MPZ
        ? to._value
        : new MPZ(to)._value;
    result._value.assignRand(normTo.sub(normFrom));
    result._value.assignAdd(result._value, normFrom);
};

MPZ.prototype.probPrime = function (reps) {
    const n = this._value.probPrime(reps || 10);
    return { 0: false, 1: 'maybe', 2: true }[n];
};

MPZ.prototype.nextPrime = function () {
    return new MPZ(this._value.nextPrime());
};

MPZ.nextPrime = function (result, num) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (num == null) {
        throw new TypeError('Second operand is missed');
    }

    const normNum = num instanceof MPZ
        ? num._value
        : new MPZ(num)._value;
    result._value.assignNextPrime(normNum);
};

MPZ.prototype.invert = function (mod) {
    const normNum = mod instanceof MPZ
        ? mod._value
        : new MPZ(mod)._value;
    return new MPZ(this._value.invert(normNum));
};

MPZ.invert = function (result, num, mod) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (num == null) {
        throw new TypeError('Second operand is missed');
    }
    if (mod == null) {
        throw new TypeError('Third operand is missed');
    }

    const normNum = num instanceof MPZ
        ? num._value
        : new MPZ(num)._value;
    const normMod = mod instanceof MPZ
        ? mod._value
        : new MPZ(mod)._value;
    result._value.assignInvert(normNum, normMod);
};

MPZ.prototype.gcd = function (num) {
    const normNum = num instanceof MPZ
        ? num._value
        : new MPZ(num)._value;
    return new MPZ(this._value.gcd(normNum));
};

MPZ.gcd = function (result, num1, num2) {
    if (!(result instanceof MPZ)) {
        throw new TypeError('First operand must be MPZ');
    }
    if (num1 == null) {
        throw new TypeError('Second operand is missed');
    }
    if (num2 == null) {
        throw new TypeError('Third operand is missed');
    }

    const normNum1 = num1 instanceof MPZ
        ? num1._value
        : new MPZ(num1)._value;
    const normNum2 = num2 instanceof MPZ
        ? num2._value
        : new MPZ(num2)._value;
    result._value.assignGcd(normNum1, normNum2);
};

MPZ.prototype.bitLength = function () {
    return this._value.bitLength();
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
            throw new TypeError('Unsupported Buffer representation');
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
            ret[ret.length - 1]++;
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
