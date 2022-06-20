#!/usr/bin/bash

TIMEFORMAT=%R


k=0
while [ $k -le $1 ]; do
j=$(time curl http://pawserver.it.itba.edu.ar/paw-2022a-01/buyer/market &>/dev/null >&1)
let k=$k+1;
done



