const {Worker, isMainThread, workerData} = require('worker_threads');
const os = require('os');
const MPZ = require('../');

if (isMainThread) {
    (async function main() {
        const threads = new Promise(resolve => {
            const size = os.cpus().length * 2;
            let wait = size;

            for (let i = 0; i < size; i++) {
                const worker = new Worker(__filename, {workerData: i});

                worker.on('exit', () => {
                    wait--;
                    if (wait === 0) {
                        resolve();
                    }
                });
            }
        });

        work('main');
        await threads;
    })();
} else {
    work(workerData);
}

function work(id) {
    for (let n = 0; n < 100; n++) {
        const p = MPZ(2).pow(n).sub(1);
        if (p.probPrime(50)) {
            const perfect = p.mul(MPZ(2).pow(n - 1));
            console.log(id, perfect.toString());
        }
    }
}
