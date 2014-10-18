if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
	echo "Usage: ./run-machine.sh <machine-id>"
	exit
fi

sudo killall ib_write_bw

port=`expr $1 + 3185`
options="--size=16 --inline_size=16 --post_list=16 --run_infinitely --duration=10 --connection=UC"
taskset -c 0 ib_write_bw -F --port=$port $options node-1.RDMA.fawn.nome.nx
