const {GmpBigInt, setJSConditioner} = require('bindings')('gmpbigint.node');

module.exports = GmpBigInt;

function conditionArgs(num, base) {
    if (typeof num !== 'string') {
        num = num.toString(base || 10);
    }

    if (num.match(/e\+/)) { // positive exponent
        if (!Number(num).toString().match(/e\+/)) {
            return {
                num: Math.floor(Number(num)).toString(),
                base: 10
            };
        } else {
            const pow = Math.ceil(Math.log(num) / Math.log(2));
            let n = (num / Math.pow(2, pow)).toString(2)
                .replace(/^0/,'');
            let i = n.length - n.indexOf('.');
            n = n.replace(/\./,'');

            for (; i <= pow; i++) {
                n += '0';
            }

            return {
                num : n,
                base : 2,
            };
        }
    } else if (num.match(/e-/)) { // negative exponent
        return {
            num : Math.floor(Number(num)).toString(),
            base : base || 10
        };
    } else {
        return {
            num : num,
            base : base || 10,
        };
    }
}

setJSConditioner(conditionArgs);

GmpBigInt.prototype.inspect = function () {
    return '<GmpBigInt ' + this.toString(10) + '>';
};

GmpBigInt.prototype.toNumber = function () {
    return parseInt(this.toString(), 10);
};

[ 'add', 'sub', 'mul', 'div', 'mod' ].forEach(function (op) {
    GmpBigInt.prototype[op] = function (num) {
        if (num instanceof GmpBigInt) {
            return this['b'+op](num);
        } else if (typeof num === 'number') {
            if (num >= 0) {
                return this['u'+op](num);
            } else if (op === 'add') {
                return this.usub(-num);
            } else if (op === 'sub') {
                return this.uadd(-num);
            } else {
                const x = GmpBigInt(num);
                return this['b'+op](x);
            }
        } else if (typeof num === 'string') {
            const x = GmpBigInt(num);
            return this['b'+op](x);
        } else {
            throw new TypeError('Unspecified operation for type '
                + (typeof num) + ' for ' + op);
        }
    };
});

GmpBigInt.prototype.abs = function () {
    return this.babs();
};

GmpBigInt.prototype.neg = function () {
    return this.bneg();
};

GmpBigInt.prototype.powm = function (num, mod) {
    let m;

    if ((typeof mod) === 'number' || (typeof mod) === 'string') {
        m = GmpBigInt(mod);
    } else if (mod instanceof GmpBigInt) {
        m = mod;
    }

    if ((typeof num) === 'number') {
        return this.upowm(num, m);
    } else if ((typeof num) === 'string') {
        const n = GmpBigInt(num);
        return this.bpowm(n, m);
    } else if (num instanceof GmpBigInt) {
        return this.bpowm(num, m);
    }
};

GmpBigInt.prototype.mod = function (num, mod) {
    let m;

    if ((typeof mod) === 'number' || (typeof mod) === 'string') {
        m = GmpBigInt(mod);
    } else if (mod instanceof GmpBigInt) {
        m = mod;
    }

    if ((typeof num) === 'number') {
        return this.umod(num, m);
    } else if ((typeof num) === 'string') {
        const n = GmpBigInt(num);
        return this.bmod(n, m);
    } else if (num instanceof GmpBigInt) {
        return this.bmod(num, m);
    }
};

GmpBigInt.prototype.pow = function (num) {
    if (typeof num === 'number') {
        if (num >= 0) {
            return this.upow(num);
        } else {
            return GmpBigInt.prototype.powm.call(this, num, this);
        }
    } else {
        const x = parseInt(num.toString(), 10);
        return GmpBigInt.prototype.pow.call(this, x);
    }
};

GmpBigInt.prototype.shiftLeft = function (num) {
    if (typeof num === 'number') {
        if (num >= 0) {
            return this.umul2exp(num);
        } else {
            return this.shiftRight(-num);
        }
    } else {
        const x = parseInt(num.toString(), 10);
        return GmpBigInt.prototype.shiftLeft.call(this, x);
    }
};

GmpBigInt.prototype.shiftRight = function (num) {
    if (typeof num === 'number') {
        if (num >= 0) {
            return this.udiv2exp(num);
        } else {
            return this.shiftLeft(-num);
        }
    } else {
        const x = parseInt(num.toString(), 10);
        return GmpBigInt.prototype.shiftRight.call(this, x);
    }
};

GmpBigInt.prototype.cmp = function (num) {
    if (num instanceof GmpBigInt) {
        return this.bcompare(num);
    } else if (typeof num === 'number') {
        if (num < 0) {
            return this.scompare(num);
        } else {
            return this.ucompare(num);
        }
    } else {
        const x = GmpBigInt(num);
        return this.bcompare(x);
    }
};

GmpBigInt.prototype.gt = function (num) {
    return this.cmp(num) > 0;
};

GmpBigInt.prototype.ge = function (num) {
    return this.cmp(num) >= 0;
};

GmpBigInt.prototype.eq = function (num) {
    return this.cmp(num) === 0;
};

GmpBigInt.prototype.ne = function (num) {
    return this.cmp(num) !== 0;
};

GmpBigInt.prototype.lt = function (num) {
    return this.cmp(num) < 0;
};

GmpBigInt.prototype.le = function (num) {
    return this.cmp(num) <= 0;
};

['and', 'or', 'xor'].forEach(function (name) {
    GmpBigInt.prototype[name] = function (num) {
        if (num instanceof GmpBigInt) {
            return this['b' + name](num);
        } else {
            const x = GmpBigInt(num);
            return this['b' + name](x);
        }
    };
});

GmpBigInt.prototype.sqrt = function() {
    return this.bsqrt();
};

GmpBigInt.prototype.root = function(num) {
    if (num instanceof GmpBigInt) {
        return this.broot(num);
    } else {
        return this.broot(num);
    }
};

GmpBigInt.prototype.rand = function (to) {
    if (to === undefined) {
        if (this.toString() === '1') {
            return GmpBigInt(0);
        } else {
            return this.brand0();
        }
    } else {
        const x = to instanceof GmpBigInt
            ? to.sub(this)
            : GmpBigInt(to).sub(this);
        return x.brand0().add(this);
    }
};

GmpBigInt.prototype.invertm = function (mod) {
    if (mod instanceof GmpBigInt) {
        return this.binvertm(mod);
    } else {
        const x = GmpBigInt(mod);
        return this.binvertm(x);
    }
};

GmpBigInt.prototype.probPrime = function (reps) {
    const n = this.probprime(reps || 10);
    return { 2: true, 1: 'maybe', 0: false }[n];
};

GmpBigInt.prototype.nextPrime = function () {
    return this.nextprime();
};

GmpBigInt.prototype.gcd = function (num) {
  return this.bgcd(num);
};

GmpBigInt.fromBuffer = function (buf, opts) {
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
            chunk.push(buf[
                i + (endian === 'big' ? j : (size - j - 1))
            ]);
        }

        hex.push(chunk
            .map(c => (c < 16 ? '0' : '') + c.toString(16))
            .join('')
        );
    }

    return GmpBigInt(hex.join(''), 16);
};

GmpBigInt.prototype.toBuffer = function (opts) {
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

        const ret = new Buffer(4 + len);
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
    if (hex.charAt(0) === '-') throw new Error(
        'converting negative numbers to Buffers not supported yet'
    );

    const len = Math.ceil(hex.length / (2 * size)) * size;
    const buf = new Buffer(len);

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

Object.keys(GmpBigInt.prototype).forEach(name => {
    if (name === 'inspect' || name === 'toString') {
        return;
    }

    GmpBigInt[name] = function (num) {
        const args = [].slice.call(arguments, 1);

        if (num instanceof GmpBigInt) {
            return num[name].apply(num, args);
        } else {
            const bigi = GmpBigInt(num);
            return bigi[name].apply(bigi, args);
        }
    };
});
