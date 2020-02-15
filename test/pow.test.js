const {MPZ} = require('../');

test('powm', () => {
    const twos = [ 2, '2', MPZ(2), MPZ('2') ];
    const tens = [ 100000, '100000', MPZ(100000), MPZ(100000) ];
    twos.forEach(two => {
        tens.forEach(te => {
            expect(MPZ('111111111').powm(two, te).toString()).toEqual('54321');
        });
    });

    expect(MPZ('624387628734576238746587435')
            .powm(2732, '457676874367586')
            .toString()
    ).toEqual('335581885073251');
});

test('pow', () => {
    [ 2, '2', MPZ(2), MPZ('2') ].forEach(two => {
        expect(MPZ('111111111').pow(two).toString()).toEqual('12345678987654321');
    });

    expect(MPZ('3487438743234789234879').pow(22).toString())
        .toEqual('861281136448465709000943928980299119292959327175552412961995332536782980636409994680542395362634321718164701236369695670918217801815161694902810780084448291245512671429670376051205638247649202527956041058237646154753587769450973231275642223337064356190945030999709422512682440247294915605076918925272414789710234097768366414400280590151549041536921814066973515842848197905763447515344747881160891303219471850554054186959791307149715821010152303317328860351766337716947079041');
});
