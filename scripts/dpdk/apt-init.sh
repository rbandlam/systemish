# Script to initialize DPDK on APT. Run as root.

ESC_SEQ="\x1b["
COL_RED=$ESC_SEQ"31;01m"
COL_RESET=$ESC_SEQ"39;49;00m"

RTE_SDK=/users/akalia/dpdk-1.5.0r0/
RTE_TARGET=x86_64-default-linuxapp-gcc

# Install uio modules
modprobe uio
insmod $RTE_SDK/$RTE_TARGET/kmod/igb_uio.ko

# Create some hugepages
echo -e "$COL_RED Creating 1024 hugepages on both sockets $COL_RESET"
echo 1024 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
echo 1024 > /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages
echo -e "$COL_RED Done creating hugepages. Status: $COL_RESET"
cat /sys/devices/system/node/*/meminfo | grep Huge

# Bind eth2 and eth3 to igb_uio
echo -e "$COL_RED Bringing eth2 and eth3 down: $COL_RESET"
ifconfig eth2 down
ifconfig eth3 down
echo -e "$COL_RED Binding ixgbe to igb_uio: $COL_RESET"
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 03:00.0
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 03:00.1
echo -e "$COL_RED Done binding ixgbe to igb_uio. Status: $COL_RESET"
echo "Done binding ixgbe to igb_uio. Status:"
$RTE_SDK/tools/pci_unbind.py --status

