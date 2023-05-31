FROM registry.access.redhat.com/ubi8/ubi:latest AS builder

LABEL maintainer="Carlos Donato <cdonato@redhat.com>"

#Install dependencies
#
RUN mkdir -p /mnt/rootfs

RUN yum -y update &&\
    yum -y install gcc make && \
    yum install --installroot /mnt/rootfs nmap-ncat --releasever 8 --setopt install_weak_deps=false --nodocs -y; yum --installroot /mnt/rootfs clean all

RUN mkdir -p /root/spammer/

ADD . /root/spammer/

WORKDIR /root/spammer/

RUN make

FROM registry.access.redhat.com/ubi8/ubi-micro:latest

RUN mkdir -p /root/spammer/

WORKDIR /root/spammer

COPY --from=builder /root/spammer/spammer ./spammer
COPY --from=builder /mnt/rootfs/ /
COPY src/spammer.sh ./spammer.sh
COPY src/http.response ./http.response

ENV MAX_MEM=500
ENV PER_MEM=100
ENV NUM_THREADS=4
ENV LOG_LEVEL=2
ENV PER_CPU=1
ENV LISTEN_PORT=8080
ENV LISTEN=0

EXPOSE $LISTEN_PORT

ENTRYPOINT ./spammer.sh
