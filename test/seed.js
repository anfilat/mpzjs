const test = require('tap').test;
const exec = require('child_process').exec;

test('rand', function (t) {
    t.plan(9);

    const cmd = process.execPath + ' -e \'console.log(require('
        + JSON.stringify(__dirname + '/../')
        + ').rand(1000).toString())\'';
    exec(cmd, function (err1, r1) {
        exec(cmd, function (err2, r2) {
            t.ok(!err1);
            t.ok(!err2);

            t.ok(
                r1.match(/^\d+\n/),
                JSON.stringify(r1) + ' is not an integer'
            );
            t.ok(
                r2.match(/^\d+\n/),
                JSON.stringify(r2) + ' is not an integer'
            );

            const n1 = parseInt(r1.split('\n')[0], 10);
            const n2 = parseInt(r2.split('\n')[0], 10);

            t.ok(n1 >= 0, 'n1 >= 0');
            t.ok(n2 >= 0, 'n2 >= 0');
            t.ok(n1 < 1000, 'n1 < 1000');
            t.ok(n2 < 1000, 'n2 < 1000');

            t.ok(n1 != n2, 'n1 != n2');
        })
    });
});
