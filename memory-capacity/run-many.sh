for i in `seq 0 15`; do
	taskset -c $i ./capacity &
done
