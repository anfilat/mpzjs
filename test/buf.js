const test = require('tap').test;
const GBI = require('../');
const put = require('put');

test('buf be', function (t) {
    const buf1 = Buffer.from([1, 2, 3, 4]);
    const num = GBI.fromBuffer(buf1, { size : 4 }).toNumber();
    t.same(
        num,
          1 * Math.pow(256, 3)
        + 2 * Math.pow(256, 2)
        + 3 * 256
        + 4
    );

    const buf2 = put().word32be(num).buffer();
    t.same(buf1, buf2,
        '[ ' + [].slice.call(buf1) + ' ] != [ ' + [].slice.call(buf2) + ' ]'
    );
    t.end();
});

test('buf le', function (t) {
    const buf1 = Buffer.from([1, 2, 3, 4]);
    const num = GBI
        .fromBuffer(buf1, { size : 4, endian : 'little' })
        .toNumber()
    ;
    const buf2 = put().word32le(num).buffer();
    t.same(buf1, buf2,
        '[ ' + [].join.call(buf1, ',') + ' ] != [ ' + [].join.call(buf2, ',') + ' ]'
    );
    t.end();
});

test('buf be le', function (t) {
    const buf_be = Buffer.from([1, 2, 3, 4, 5, 6, 7, 8]);
    const buf_le = Buffer.from([4, 3, 2, 1, 8, 7, 6, 5]);

    const num_be = GBI
        .fromBuffer(buf_be, { size : 4, endian : 'big' })
        .toString();
    const num_le = GBI
        .fromBuffer(buf_le, { size : 4, endian : 'little' })
        .toString();

    t.same(num_be, num_le);
    t.end();
});

test('buf high bits', function (t) {
    const buf_be = Buffer.from([
        201, 202, 203, 204,
        205, 206, 207, 208
    ]);
    const buf_le = Buffer.from([
        204, 203, 202, 201,
        208, 207, 206, 205
    ]);

    const num_be = GBI
        .fromBuffer(buf_be, { size : 4, endian : 'big' })
        .toString();
    const num_le = GBI
        .fromBuffer(buf_le, { size : 4, endian : 'little' })
        .toString();

    t.same(num_be, num_le);
    t.end();
});

test('buf to from', function (t) {
    const nums = [
        0, 1, 10, 15, 3, 16,
        7238, 1337, 31337, 505050,
        '172389721984375328763297498273498732984324',
        '32848432742',
        '12988282841231897498217398217398127983721983719283721',
        '718293798217398217312387213972198321'
    ];

    nums.forEach(function (num) {
        const b = GBI(num);
        const u = b.toBuffer();

        t.ok(u);
        t.same(
            GBI.fromBuffer(u).toString(),
            b.toString()
        );
    });

    t.throws(function () {
        GBI(-1).toBuffer(); // can't pack negative numbers yet
    });
    t.end();
});

test('toBuf', function (t) {
    const buf = Buffer.from([ 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f ]);
    const b = GBI(
          0x0a * 256*256*256*256*256
        + 0x0b * 256*256*256*256
        + 0x0c * 256*256*256
        + 0x0d * 256*256
        + 0x0e * 256
        + 0x0f
    );

    t.same(b.toString(16), 'a0b0c0d0e0f');

    t.same(
        [].slice.call(b.toBuffer({ endian : 'big', size : 2 })),
        [ 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f ]
    );

    t.same(
        [].slice.call(b.toBuffer({ endian : 'little', size : 2 })),
        [ 0x0b, 0x0a, 0x0d, 0x0c, 0x0f, 0x0e ]
    );

    t.same(
        GBI.fromBuffer(buf).toString(16),
        b.toString(16)
    );

    t.same(
        [].slice.call(GBI(43135012110).toBuffer({
            endian : 'little', size : 4
        })),
        [ 0x0a, 0x00, 0x00, 0x00, 0x0e, 0x0d, 0x0c, 0x0b ]
    );

    t.same(
        [].slice.call(GBI(43135012110).toBuffer({
            endian : 'big', size : 4
        })),
        [ 0x00, 0x00, 0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e ]
    );
    t.end();
});

test('zero pad', function (t) {
    const b = GBI(0x123456);

    t.same(
        [].slice.call(b.toBuffer({ endian : 'big', size:4 })),
        [ 0x00, 0x12, 0x34, 0x56 ]
    );

    t.same(
        [].slice.call(b.toBuffer({ endian : 'little', size:4 })),
        [ 0x56, 0x34, 0x12, 0x00 ]
    );
    t.end();
});

test('to mpint', function (t) {
    // test values taken directly out of
    // http://tools.ietf.org/html/rfc4251#page-10

    const refs = {
        '0': Buffer.from([ 0x00, 0x00, 0x00, 0x00 ]),
        '9a378f9b2e332a7': Buffer.from([
            0x00, 0x00, 0x00, 0x08,
            0x09, 0xa3, 0x78, 0xf9,
            0xb2, 0xe3, 0x32, 0xa7,
        ]),
        '80': Buffer.from([ 0x00, 0x00, 0x00, 0x02, 0x00, 0x80 ]),
        '-1234': Buffer.from([ 0x00, 0x00, 0x00, 0x02, 0xed, 0xcc ]),
        '-deadbeef': Buffer.from([
            0x00, 0x00, 0x00, 0x05, 0xff, 0x21, 0x52, 0x41, 0x11
        ]),
    };

    Object.keys(refs).forEach(function (key) {
        const buf0 = GBI(key, 16).toBuffer('mpint');
        const buf1 = refs[key];

        t.same(
            buf0, buf1,
            buf0.inspect() + ' != ' + buf1.inspect()
            + ' for GBI(' + key + ')'
        );
    });
    t.end();
});
