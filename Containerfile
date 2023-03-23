FROM registry.access.redhat.com/ubi8/ubi:latest AS builder

LABEL maintainer="Carlos Donato <cdonato@redhat.com>"

#Install dependencies

RUN yum -y update &&\
    yum -y install gcc make

RUN mkdir -p /root/spammer/

ADD . /root/spammer/

WORKDIR /root/spammer/

RUN make

FROM registry.access.redhat.com/ubi8/ubi-minimal:latest

RUN mkdir -p /root/spammer/

WORKDIR /root/spammer

COPY --from=builder /root/spammer/spammer ./spammer

ENTRYPOINT ./spammer
