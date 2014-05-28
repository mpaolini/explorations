from invoke import run, task
import subprocess
import re
import os


@task
def compile():
    run('cd httpserver && go build -o server_go.out server.go')
    run('cd httpserver && ghc -o server_haskell.out server.hs')
    run('cd httpserver/c_libev && gcc -O2 server.c -l ev')
    run('cd httpserver/cpp_boost && g++ -O2 server.c -l boost_system')
    # XXX does not work
    run('cd httpserver/clojure && lein install')  


@task
def perf():
    os.chdir('httpserver')
    # TODO: add clojure
    for impl in [['python', 'server.py'], ['server_haskell.out'], ['server_go.out'], ['node', 'server.js'], ['c_libev/a.out'], ['cpp_boost/a.out']]:
        server = subprocess.Popen(impl)
        perf = subprocess.check_output(['httperf', '--server=localhost',
                                        '--port=8000', '--uri=/',
                                        '--num-conns=5000', '--hog'],
                                       stderr=subprocess.DEVNULL)
        server.terminate()
        mo = re.search(r'^Request rate: ([0-9\.]+) .*', perf.decode(),
                       re.MULTILINE)
        print('{server} {out}'.format(server=impl, out=mo.group(1)))

