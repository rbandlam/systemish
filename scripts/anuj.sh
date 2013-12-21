#Run as user akalia
sudo ifconfig ib0 10.0.0.50
for i in `seq 2 20`; do
	ip=`expr 50 + $i`
	ssh anuj$i.RDMA.fawn.susitna.pdl.cmu.local "sudo ifconfig ib0 10.0.0.$ip"
done
