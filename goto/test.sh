for compute in `seq 2 2 50`; do
	echo "#define COMPUTE $compute" > param.h
	make

	echo "COMPUTE = $compute, nogoto:"
	nogoto_result=`./nogoto 2>/dev/null`
	nogoto_time=`echo $nogoto_result | cut -d' ' -f 3`

	echo "COMPUTE = $compute, goto:"
	goto_result=`./goto 2>/dev/null`
	goto_time=`echo $goto_result | cut -d' ' -f 3`

	ratio=`python -c "print $nogoto_time/$goto_time"`
	echo "COMPUTE = $compute, ratio = $ratio"
	
done
