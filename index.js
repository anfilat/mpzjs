const {GmpBigInt} = require('bindings')('gmpbigint.node');

function GBI(value, base) {
    if (!new.target) {
        return new GBI(value, base);
    }

    if (value == null) {
        this._value = new GmpBigInt();
        return;
    }

    if (value instanceof GmpBigInt) {
        this._value = value;
        return;
    }

    if (typeof value === 'number') {
        this._value = new GmpBigInt(value);
        return;
    }

    const args = prepareArgs(value, base || 10);
    this._value = new GmpBigInt(args.num, args.base);
}

function prepareArgs(num, base) {
    if (typeof num !== 'string') {
        num = num.toString(base);
    }

    if (num.match(/e\+/)) { // positive exponent
        if (!Number(num).toString().match(/e\+/)) {
            return {
                num: Math.floor(Number(num)).toString(),
                base: 10
            };
        }

        const pow = Math.ceil(Math.log2(num));
        let n = (num / Math.pow(2, pow))
            .toString(2)
            .replace(/^0/, '')
            .replace(/\./, '')
            .padEnd(pow, '0');

        return {
            num: n,
            base: 2,
        };
    }
    if (num.match(/e-/)) { // negative exponent
        return {
            num: Math.trunc(Number(num)).toString(),
            base: 10
        };
    }
    return {
        num,
        base,
    };
}

module.exports = GBI;

GBI.prototype.toString = function (base) {
    if (base == null || base === 10) {
        return this._value.toString();
    }

    base = Number(base);
    if (base < 2 || base > 62) {
        throw RangeError('base should be : 2<= base <= 62');
    }
    return this._value.toString(base);
};

GBI.prototype.inspect = function () {
    return '<GBI ' + this.toString() + '>';
};

GBI.prototype.toNumber = function () {
    return this._value.toNumber();
};

GBI.prototype.add = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.badd(value._value));
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            return new GBI(this._value.uadd(value));
        }
        return new GBI(this._value.usub(-value));
    }
    if (type === 'string') {
        return new GBI(this._value.badd(new GBI(value)._value));
    }
    throw new TypeError(`Unspecified add for type ${type}`);
};

GBI.add = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.add(value2);
    }
    return new GBI(value1).add(value2);
};

GBI.prototype.assignAdd = function (value) {
    if (value instanceof GBI) {
        this._value.bassignAdd(value._value);
        return this;
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            this._value.uassignAdd(value);
            return this;
        }
        this._value.uassignSub(-value);
        return this;
    }
    if (type === 'string') {
        this._value.bassignAdd(new GBI(value)._value);
        return this;
    }
    throw new TypeError(`Unspecified add for type ${type}`);
};

GBI.assignAdd = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.assignAdd(value2);
    }
    return new GBI(value1).assignAdd(value2);
};

GBI.prototype.sub = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.bsub(value._value));
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            return new GBI(this._value.usub(value));
        }
        return new GBI(this._value.uadd(-value));
    }
    if (type === 'string') {
        return new GBI(this._value.bsub(new GBI(value)._value));
    }
    throw new TypeError(`Unspecified sub for type ${type}`);
};

GBI.sub = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.sub(value2);
    }
    return new GBI(value1).sub(value2);
};

GBI.prototype.assignSub = function (value) {
    if (value instanceof GBI) {
        this._value.bassignSub(value._value);
        return this;
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            this._value.uassignSub(value);
            return this;
        }
        this._value.uassignAdd(-value);
        return this;
    }
    if (type === 'string') {
        this._value.bassignSub(new GBI(value)._value);
        return this;
    }
    throw new TypeError(`Unspecified sub for type ${type}`);
};

GBI.assignSub = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.assignSub(value2);
    }
    return new GBI(value1).assignSub(value2);
};

GBI.prototype.mul = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.bmul(value._value));
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            return new GBI(this._value.umul(value));
        }
        return new GBI(this._value.bmul(new GBI(value)._value));
    }
    if (type === 'string') {
        return new GBI(this._value.bmul(new GBI(value)._value));
    }
    throw new TypeError(`Unspecified mul for type ${type}`);
};

GBI.mul = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.mul(value2);
    }
    return new GBI(value1).mul(value2);
};

GBI.prototype.assignMul = function (value) {
    if (value instanceof GBI) {
        this._value.bassignMul(value._value);
        return this;
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            this._value.uassignMul(value);
            return this;
        }
        this._value.bassignMul(new GBI(value)._value);
        return this;
    }
    if (type === 'string') {
        this._value.bassignMul(new GBI(value)._value);
        return this;
    }
    throw new TypeError(`Unspecified mul for type ${type}`);
};

GBI.assignMul = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.assignMul(value2);
    }
    return new GBI(value1).assignMul(value2);
};

GBI.prototype.div = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.bdiv(value._value));
    }
    const type = typeof value;
    if (type === 'number') {
        if (value >= 0) {
            return new GBI(this._value.udiv(value));
        }
        return new GBI(this._value.bdiv(new GBI(value)._value));
    }
    if (type === 'string') {
        return new GBI(this._value.bdiv(new GBI(value)._value));
    }
    throw new TypeError(`Unspecified div for type ${type}`);
};

GBI.div = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.div(value2);
    }
    return new GBI(value1).div(value2);
};

GBI.prototype.mod = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.bmod(value._value));
    }
    const type = typeof value;
    if (type === 'number') {
        return new GBI(this._value.umod(value));
    }
    if (type === 'string') {
        return new GBI(this._value.bmod(new GBI(value)._value));
    }
    throw new TypeError(`Unspecified mod for type ${type}`);
};

GBI.mod = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.mod(value2);
    }
    return new GBI(value1).mod(value2);
};

GBI.prototype.and = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.band(value._value));
    }
    return new GBI(this._value.band(new GBI(value)._value));
};

GBI.and = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.and(value2);
    }
    return new GBI(value1).and(value2);
};

GBI.prototype.or = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.bor(value._value));
    }
    return new GBI(this._value.bor(new GBI(value)._value));
};

GBI.or = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.or(value2);
    }
    return new GBI(value1).or(value2);
};

GBI.prototype.xor = function (value) {
    if (value instanceof GBI) {
        return new GBI(this._value.bxor(value._value));
    }
    return new GBI(this._value.bxor(new GBI(value)._value));
};

GBI.xor = function (value1, value2) {
    if (value1 instanceof GBI) {
        return value1.xor(value2);
    }
    return new GBI(value1).xor(value2);
};

GBI.prototype.abs = function () {
    return new GBI(this._value.babs());
};

GBI.abs = function (value) {
    if (value instanceof GBI) {
        return value.abs();
    }
    return new GBI(value).abs();
};

GBI.prototype.neg = function () {
    return new GBI(this._value.bneg());
};

GBI.neg = function (value) {
    if (value instanceof GBI) {
        return value.neg();
    }
    return new GBI(value).neg();
};

GBI.prototype.bitLength = function () {
    return this._value.bitLength();
};

GBI.bitLength = function (value) {
    if (value instanceof GBI) {
        return value.bitLength();
    }
    return new GBI(value).bitLength();
};

GBI.prototype.powm = function (num, mod) {
    const m = normalizeValue(mod)._value;

    if (num instanceof GBI) {
        return new GBI(this._value.bpowm(num._value, m));
    }
    const type = typeof num;
    if (type === 'number') {
        return new GBI(this._value.upowm(num, m));
    }
    if (type === 'string') {
        return new GBI(this._value.bpowm(new GBI(num)._value, m));
    }
    throw new TypeError(`Unspecified powm for type ${type} for m`);
};

function normalizeValue(value) {
    if (value instanceof GBI) {
        return value;
    }
    const type = typeof value;
    if (type === 'number' || type === 'string') {
        return new GBI(value);
    }
    throw new TypeError(`Unspecified operation for type ${type} for n`);
}

GBI.powm = function (value, num, mod) {
    if (value instanceof GBI) {
        return value.powm(num, mod);
    }
    return new GBI(value).powm(num, mod);
};

GBI.prototype.pow = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num >= 0) {
        return new GBI(this._value.upow(num));
    }
    return this.powm(num, this);
};

GBI.pow = function (value, num) {
    if (value instanceof GBI) {
        return value.pow(num);
    }
    return new GBI(value).pow(num);
};

GBI.prototype.shiftLeft = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num >= 0) {
        return new GBI(this._value.umul2exp(num));
    }
    return this.shiftRight(-num);
};

GBI.shiftLeft = function (value, num) {
    if (value instanceof GBI) {
        return value.shiftLeft(num);
    }
    return new GBI(value).shiftLeft(num);
};

GBI.prototype.shiftRight = function (num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    if (num >= 0) {
        return new GBI(this._value.udiv2exp(num));
    }
    return this.shiftLeft(-num);
};

GBI.shiftRight = function (value, num) {
    if (value instanceof GBI) {
        return value.shiftRight(num);
    }
    return new GBI(value).shiftRight(num);
};

GBI.prototype.cmp = function (num) {
    if (num instanceof GBI) {
        return this._value.bcompare(num._value);
    }
    if (typeof num === 'number') {
        if (num < 0) {
            return this._value.scompare(num);
        }
        return this._value.ucompare(num);
    }
    return this._value.bcompare(new GBI(num)._value);
};

GBI.cmp = function (value, num) {
    if (value instanceof GBI) {
        return value.cmp(num);
    }
    return new GBI(value).cmp(num);
};

GBI.prototype.gt = function (num) {
    return this.cmp(num) > 0;
};

GBI.prototype.ge = function (num) {
    return this.cmp(num) >= 0;
};

GBI.prototype.eq = function (num) {
    return this.cmp(num) === 0;
};

GBI.prototype.ne = function (num) {
    return this.cmp(num) !== 0;
};

GBI.prototype.lt = function (num) {
    return this.cmp(num) < 0;
};

GBI.prototype.le = function (num) {
    return this.cmp(num) <= 0;
};

GBI.prototype.sqrt = function() {
    return new GBI(this._value.bsqrt());
};

GBI.sqrt = function (value) {
    if (value instanceof GBI) {
        return value.sqrt();
    }
    return new GBI(value).sqrt();
};

GBI.prototype.root = function(num) {
    if (typeof num !== 'number') {
        num = parseInt(num.toString(), 10);
    }

    return new GBI(this._value.broot(num));
};

GBI.root = function (value, num) {
    if (value instanceof GBI) {
        return value.root(num);
    }
    return new GBI(value).root(num);
};

GBI.prototype.rand = function (to) {
    if (to == null) {
        if (this.toNumber() === 1) {
            return new GBI(0);
        }
        return new GBI(this._value.brand0());
    }

    const x = to instanceof GBI
        ? to.sub(this)
        : new GBI(to).sub(this);
    return new GBI(x._value.brand0()).add(this);
};

GBI.rand = function (value, to) {
    if (value instanceof GBI) {
        return value.rand(to);
    }
    return new GBI(value).rand(to);
};

GBI.prototype.invertm = function (mod) {
    if (mod instanceof GBI) {
        return new GBI(this._value.binvertm(mod._value));
    }
    return new GBI(this._value.binvertm(new GBI(mod)._value));
};

GBI.invertm = function (value, mod) {
    if (value instanceof GBI) {
        return value.invertm(mod);
    }
    return new GBI(value).invertm(mod);
};

GBI.prototype.probPrime = function (reps) {
    const n = this._value.probprime(reps || 10);
    return { 0: false, 1: 'maybe', 2: true }[n];
};

GBI.probPrime = function (value, reps) {
    if (value instanceof GBI) {
        return value.probPrime(reps);
    }
    return new GBI(value).probPrime(reps);
};

GBI.prototype.nextPrime = function () {
    return new GBI(this._value.nextprime());
};

GBI.nextPrime = function (value) {
    if (value instanceof GBI) {
        return value.nextPrime();
    }
    return new GBI(value).nextPrime();
};

GBI.prototype.gcd = function (num) {
    return new GBI(this._value.bgcd(num._value));
};

GBI.gcd = function (value, num) {
    if (value instanceof GBI) {
        return value.gcd(num);
    }
    return new GBI(value).gcd(num);
};

GBI.fromBuffer = function (buf, opts) {
    if (!opts) {
        opts = {};
    }

    const endian = { 1: 'big', '-1': 'little' }[opts.endian]
        || opts.endian || 'big';

    const size = opts.size || 1;

    if (buf.length % size !== 0) {
        throw new RangeError('Buffer length (' + buf.length + ')'
            + ' must be a multiple of size (' + size + ')'
        );
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

    return new GBI(hex.join(''), 16);
};

GBI.prototype.toBuffer = function (opts) {
    if (typeof opts === 'string') {
        if (opts !== 'mpint') {
            return 'Unsupported Buffer representation';
        }

        const abs = this.abs();
        const buf = abs.toBuffer({ size: 1, endian: 'big' });
        let len = buf.length === 1 && buf[0] === 0 ? 0 : buf.length;
        if (buf[0] & 0x80) {
            len ++;
        }

        const ret = Buffer.alloc(4 + len);
        if (len > 0) {
            buf.copy(ret, 4 + (buf[0] & 0x80 ? 1 : 0));
        }
        if (buf[0] & 0x80) {
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
        .filter(function (s) { return s.length > 0 });

    hx.forEach((chunk, i) => {
        for (let j = 0; j < size; j++) {
            const ix = i * size + (endian === 'big' ? j : size - j - 1);
            buf[ix] = parseInt(chunk.slice(j * 2, j * 2 + 2), 16);
        }
    });

    return buf;
};
