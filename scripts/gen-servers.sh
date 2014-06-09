NUM_SERVERS=32
hi=`expr $NUM_SERVERS - 1`
echo $hi
for i in `seq 0 $hi`; do
	echo "node-1.RDMA.fawn.apt.emulab.net"
	echo `expr 5500 + $i`
done
