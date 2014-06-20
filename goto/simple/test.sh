for compute in `seq 1 5 50`; do
	shm-rm.sh 1>/dev/null 2>/dev/null

	echo "#define COMPUTE $compute" > param.h
	make

	echo "COMPUTE = $compute, nogoto:"
	nogoto_result=`sudo ./nogoto 2>/dev/null`
	nogoto_time=`echo $nogoto_result | cut -d' ' -f 3`

	echo "COMPUTE = $compute, goto:"
	goto_result=`sudo ./goto 2>/dev/null`
	goto_time=`echo $goto_result | cut -d' ' -f 3`

	ratio=`python -c "print $nogoto_time/$goto_time"`
	echo "COMPUTE = $compute, ratio = $ratio"
	
done
