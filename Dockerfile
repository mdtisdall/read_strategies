FROM mdtisdall/devenv_cpp

WORKDIR /opt/read_strategies

ADD . /opt/read_strategies/

RUN mkdir bin && \
    make

