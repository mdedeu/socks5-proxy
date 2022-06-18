#!/usr/bin/bash


let i=$(pgrep socks5d);
let j=$1
while [ $j -ge 0  ] ; do
	ps aux --sort=%mem | egrep $i	
	sleep 1;
	let j=$j-1;  
done 
