const MPZ = require('mpzjs');

const n = MPZ('782910138827292261791972728324982');
MPZ.sub(n, n, '182373273283402171237474774728373');
MPZ.div(n, n, 8);

console.log(n);

const b = MPZ('782910138827292261791972728324982')
    .sub('182373273283402171237474774728373')
    .div(8);

console.log(b);
