#!/bin/bash
gcc -o consumer consumer.c
gcc -o producer producer.c

echo "PRODUCER 				 CONSUMER1 				CONSUMER2"

./producer $1 &

for ((i = 0; i < $1; i++))
do
	echo "Start consumer $i"
	./consumer $1 $i & 
done