#!/usr/bin/bash

i=$(curl http://pawserver.it.itba.edu.ar/paw-2022a-01/buyer/market | md5sum)

while true; do 
j=$(curl -x socks5://localhost:1080 http://pawserver.it.itba.edu.ar/paw-2022a-01/buyer/market | md5sum)
if [ "$j" !=  "$i" ];then
	echo "Error detected\n" 
	exit -1
fi
done; 
