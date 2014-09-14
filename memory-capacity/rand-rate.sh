shm-rm.sh
for i in `seq 0 7`; do
	tid=`expr $i + 1`
	core=`expr 2 \* $i`
	sudo taskset -c $core ./rand-rate $tid &
done
