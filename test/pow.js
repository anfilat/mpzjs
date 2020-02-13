const test = require('tap').test;
const bigint = require('../');

test('powm', function (t) {
    const twos = [ 2, '2', bigint(2), bigint('2') ];
    const tens = [ 100000, '100000', bigint(100000), bigint(100000) ];
    twos.forEach(function (two) {
        tens.forEach(function (te) {
            t.same(
                bigint('111111111').powm(two, te).toString(),
                '54321'
            );
        });
    });

    t.same(
        bigint('624387628734576238746587435')
            .powm(2732, '457676874367586')
            .toString()
        ,
        '335581885073251'
    );
    t.end();
});

test('pow', function (t) {
    [ 2, '2', bigint(2), bigint('2') ].forEach(function (two) {
        t.same(
            bigint('111111111').pow(two).toString(),
            '12345678987654321'
        );
    });

    t.same(
        bigint('3487438743234789234879').pow(22).toString(),
        '861281136448465709000943928980299119292959327175552412961995332536782980636409994680542395362634321718164701236369695670918217801815161694902810780084448291245512671429670376051205638247649202527956041058237646154753587769450973231275642223337064356190945030999709422512682440247294915605076918925272414789710234097768366414400280590151549041536921814066973515842848197905763447515344747881160891303219471850554054186959791307149715821010152303317328860351766337716947079041'
    );

    t.end();
});
