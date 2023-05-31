#!/bin/bash

BASEDIR=`dirname $0`

$BASEDIR/spammer $MAX_MEM $PER_MEM $NUM_THREADS $LOG_LEVEL $PER_CPU &

if [[ $LISTEN > 0 ]]
then
  echo "Listening on port $LISTEN_PORT"

  while true
  do
    /usr/bin/ncat -l $LISTEN_PORT < $BASEDIR/http.response
  done
else
  sleep infinity
fi
