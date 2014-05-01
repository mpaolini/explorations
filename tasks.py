from invoke import run, task
import subprocess
import re


@task
def compile():
    run('cd httpserver && go build -o server_go.out server.go')
    run('cd httpserver && ghc -o server_haskell.out server.hs')


@task
def perf():
    for impl in ['server.py', 'server_haskell.out', 'server_go.out']:
        server = subprocess.Popen(['httpserver/{}'.format(impl)])
        perf = subprocess.check_output(['httperf', '--server=localhost',
                                        '--port=8000', '--uri=/',
                                        '--num-conns=5000', '--hog'],
                                       stderr=subprocess.DEVNULL)
        server.terminate()
        mo = re.search(r'^Request rate: ([0-9\.]+) .*', perf.decode(),
                       re.MULTILINE)
        print('{server} {out}'.format(server=impl, out=mo.group(1)))

