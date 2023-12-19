MEMORY
{
    ram (xrw)     : ORIGIN = 0x20000000, LENGTH = 96K
    rom (rx)      : ORIGIN = 0x8000000, LENGTH = 512K
}

INCLUDE ../third_party/libopencm3/libopencm3/lib/cortex-m-generic.ld
