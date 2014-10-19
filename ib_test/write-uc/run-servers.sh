sudo killall ib_write_bw
sudo killall ib_read_bw

for i in `seq 0 10`; do
	port=`expr 3185 + $i`
	ib_write_bw --connection=UC --port=$port &
done
