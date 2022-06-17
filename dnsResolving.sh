#!/usr/bin/bash

make_dns_query(){
	i=$RANDOM;
	while [ $i -ge 0 ] ; do
 		curl -x socks5h://localhost:1080 google.com.ar &> /dev/null >&1;
 		let i=($i-1); 
 		sleep 1;  
	done
}


j=0; 
while [ $j -le $1 ] ; do 
	make_dns_query& 
	let j=($j+1);
done

sleep 10; 
pkill -P $$ --signal 9 &> /dev/null >&1;  
pkill curl 
