extern "C"
{
// #include "l2dkit-hostd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <linux/limits.h>
#include <linux/unistd.h>
#include <linux/virtio_gpu.h>
#include <linux/virtio_ring.h>
#include <linux/virtio_config.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_mmio.h>
#include <linux/virtio_pci.h>
}

