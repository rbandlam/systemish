# Script to initialize DPDK on APT. Run as root.

ESC_SEQ="\x1b["
COL_RED=$ESC_SEQ"31;01m"
COL_RESET=$ESC_SEQ"39;49;00m"

RTE_SDK=/home/akalia/dpdk-1.5.0r0/
RTE_TARGET=x86_64-default-linuxapp-gcc

# Increase shmmax and shmall parameters
echo -e "$COL_RED Increasing shmmax and shmall $COL_RESET"
sudo sysctl -w kernel.shmmax=2147483648     # Bytes
sudo sysctl -w kernel.shmall=2147483648     # Pages
sudo sysctl -p /etc/sysctl.conf

# Create hugepage mount
echo -e "$COL_RED Creating /mnt/huge $COL_RESET"
umount /mnt/huge
rm -rf /mnt/huge
mkdir /mnt/huge
mount -t hugetlbfs nodev /mnt/huge

# Install uio modules
echo -e "$COL_RED Installing uio and igb_uio modules $COL_RESET"
modprobe uio
rmmod igb_uio
insmod $RTE_SDK/$RTE_TARGET/kmod/igb_uio.ko

# Create some hugepages
echo -e "$COL_RED Creating 2048 hugepages on both sockets $COL_RESET"
echo 2048 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
echo 2048 > /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages
echo -e "$COL_RED Done creating hugepages. Status: $COL_RESET"
cat /sys/devices/system/node/*/meminfo | grep Huge

# Bind 10 GbE ports to igb_uio
echo -e "$COL_RED Bringing xge0 through xge3 down: $COL_RESET"
for i in `seq 0 3`; do
	ifconfig xge$i down
done

echo -e "$COL_RED Binding ports to igb_uio: $COL_RESET"
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 02:00.0
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 02:00.1
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 08:00.0
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 08:00.1
echo -e "$COL_RED Done binding ixgbe to igb_uio. Status: $COL_RESET"

$RTE_SDK/tools/pci_unbind.py --status

