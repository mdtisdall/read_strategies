FROM mdtisdall/devenv_cpp


ADD ./test /opt/read_strategies/test
WORKDIR /opt/read_strategies/test
RUN ./make_test_input.sh 

WORKDIR /opt/read_strategies
ADD ./Makefile /opt/read_strategies/Makefile
ADD ./src /opt/read_strategies/src

RUN mkdir bin && \
    make

