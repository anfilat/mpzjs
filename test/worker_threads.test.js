const {Worker, isMainThread} = require('worker_threads');
const os = require('os');
const MPZ = require('../');

if (isMainThread) {
    test('worker threads', async () => {
        await expect(async () => {
            const threads = new Promise(resolve => {
                const size = os.cpus().length * 2;
                let wait = size;

                for (let i = 0; i < size; i++) {
                    const worker = new Worker(__filename);

                    worker.on('exit', () => {
                        wait--;
                        if (wait === 0) {
                            resolve();
                        }
                    });
                }
            });

            work();
            await threads;
        }).not.toThrow();
    });
} else {
    work();
}

function work() {
    for (let n = 0; n < 100; n++) {
        const p = MPZ(2).pow(n).sub(1);
        if (p.probPrime(50)) {
            p.mul(MPZ(2).pow(n - 1)).rand();
        }
    }
}
