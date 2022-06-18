#!/usr/bin/bash

make_dns_query(){
 	nc localhost 1080 ;
}


j=0; 
while [ $j -le $1 ] ; do 
	make_dns_query $j & 
	let j=($j+1);
done

sleep 10; 
pkill -P $$ --signal 9 &> /dev/null >&1;  
pkill nc
