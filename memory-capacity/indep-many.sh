for i in `seq 0 15`; do
	pid=`expr $i + 1`
	sudo ipcrm -M $pid 2>/dev/null
	sudo taskset -c $i ./indep-many $pid &
done
