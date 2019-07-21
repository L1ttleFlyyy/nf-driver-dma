# nf-driver-dma
DMA driver and user api for NetFPGA

## Memory map and dma
[Linux Device Drivers, Third Edition](https://lwn.net/Kernel/LDD3/) chapter 15

## Mapping physical memory to user space
[How to access physical addresses from user space in Linux?](https://stackoverflow.com/questions/12827948/mapping-a-physical-device-to-a-pointer-in-user-space)

## Usage
`make`

memmap will read physical address 0x2000

writemap will write (0,1,2...9)/(a ,9 ,8->1) to physical address 0x2000

Meanwhile, somehow trigger the driver to read from/write to 
that 0x2000 memory address instead of pci_single_map(skb).
