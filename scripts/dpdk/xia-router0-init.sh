# Script to initialize DPDK on xia-router0. Run as root.

function blue() {
	es=`tput setaf 4`
	ee=`tput sgr0`
	echo "${es}$1${ee}"
}

ESC_SEQ="\x1b["
COL_RED=$ESC_SEQ"31;01m"
COL_RESET=$ESC_SEQ"39;49;00m"

RTE_SDK=/home/akalia/dpdk-1.5.0r0/
RTE_TARGET=x86_64-default-linuxapp-gcc

# Increase shmmax and shmall parameters
blue "Increasing shmmax and shmall"
kernel.shmmax = 9223372036854775807
kernel.shmall = 1152921504606846720
sudo sysctl -p /etc/sysctl.conf

# Create hugepage mount
blue "Creating /mnt/huge"
umount /mnt/huge
rm -rf /mnt/huge
mkdir /mnt/huge
mount -t hugetlbfs nodev /mnt/huge

# Install uio modules
blue "Installing uio and igb_uio modules"
modprobe uio
insmod $RTE_SDK/$RTE_TARGET/kmod/igb_uio.ko

# Create some hugepages
blue "Creating 2048 hugepages on both sockets"
echo 2048 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
echo 2048 > /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages
blue "Done creating hugepages. Status:"
cat /sys/devices/system/node/*/meminfo | grep Huge

# Bind 10 GbE ports to igb_uio
blue "Bringing xge0 through xge3 down:"
for i in `seq 0 3`; do
	ifconfig xge$i down
done

blue "Binding ports to igb_uio:"
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 02:00.0
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 02:00.1
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 08:00.0
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 08:00.1
blue "Done binding ixgbe to igb_uio. Status:"

$RTE_SDK/tools/pci_unbind.py --status

