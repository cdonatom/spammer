#!/bin/bash

BASEDIR=`dirname $0`

$BASEDIR/spammer $MAX_MEM $PER_MEM $NUM_THREADS $LOG_LEVEL &

while true
do
  /usr/bin/ncat -l $LISTEN_PORT < $BASEDIR/http.response
done
