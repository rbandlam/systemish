if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
	echo "Usage: ./run-machine.sh <machine-id>"
	exit
fi

sudo killall ib_write_bw
sudo killall ib_read_bw

port=`expr $1 + 3185`
options="--size=16 --post_list=16 --run_infinitely --duration=2 --connection=RC"

# Server names

# APT
server_name="node-1.RDMA.fawn.apt.emulab.net"

# Nome
# server_name="node-1.RDMA.fawn.nome.nx"

taskset -c 0 ib_read_bw -F --port=$port $options $server_name
