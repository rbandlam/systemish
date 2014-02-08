shm-rm.sh
for i in `seq 0 7`; do
	taskset -c $i ./capacity-noshm $i &
done
