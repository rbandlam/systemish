# Script to initialize DPDK on APT. Run as su
RTE_SDK=/users/akalia/dpdk-1.5.0r0/

# Create some hugepages
echo "Creating 1024 hugepages on both sockets"
echo 1024 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
echo 1024 > /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages
echo "Done creating hugepages. Status:"
cat /sys/devices/system/node/*/meminfo | grep Huge

# Bind eth2 and eth3 to igb_uio
echo "Bringing eth2 and eth3 down"
ifconfig eth2 down
ifconfig eth3 down
echo "Binding ixgbe to igb_uo"
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 03:00.0
$RTE_SDK/tools/pci_unbind.py --bind=igb_uio 03:00.1

