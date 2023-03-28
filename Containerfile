FROM registry.access.redhat.com/ubi8/ubi:latest AS builder

LABEL maintainer="Carlos Donato <cdonato@redhat.com>"

#Install dependencies

RUN yum -y update &&\
    yum -y install gcc make

RUN mkdir -p /root/spammer/

ADD . /root/spammer/

WORKDIR /root/spammer/

RUN make

FROM registry.access.redhat.com/ubi8/ubi-micro:latest

RUN mkdir -p /root/spammer/

WORKDIR /root/spammer

COPY --from=builder /root/spammer/spammer ./spammer

ENV MAX_MEM=500
ENV PER_MEM=100
ENV NUM_THREADS=4
ENV LOG_LEVEL=2

ENTRYPOINT ./spammer $MAX_MEM $PER_MEM $NUM_THREADS $LOG_LEVEL
