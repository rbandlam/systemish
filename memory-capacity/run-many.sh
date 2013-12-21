shm-rm.sh
for i in `seq 0 0`; do
	taskset -c $i ./capacity-shm $i &
done
