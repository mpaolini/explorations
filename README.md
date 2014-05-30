# My Haskell experiments

## Setup developer environment

### Common

sudo apt-get install httperf 


### Python

Install python 3.4 (use pyenv)

Install required packages:

    virtualenv virtual
    source virtual/bin/activate
    pip install requirements.txt


### Haskell

Install ghc

    cabal install Network


### GO

Install go


### Node

Install node


### Clojure

Install clojure and leiningen. Finally:

    cd httpserver && lein install


### C (libev)

Install cc and libev-dev and libboost-dev

### Scala

    Download and untar Typesafe mega-package >= 1.1.3
        https://typesafe.com/platform/getstarted
    $ export PATH=/path/to/activator/bin
    $ cd httpserver/scala
    $ activator
    [hello-play] $ start 8000

## HTTP server performance

    invoke compile
    invoke perf
