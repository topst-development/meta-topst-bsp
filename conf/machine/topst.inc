#@TYPE: Machine
#@NAME: Telechips TOPST SOC Family
#@DESCRIPTION: Machine configuration for the Telechips TOPST Board

# Ship all kernel modules by default
MACHINE_EXTRA_RRECOMMENDS = "kernel-modules"

DEFAULT_IMAGE_FSTYPE ?= "ext4"
SWITCH_ROOTFS_MOUNT_OPT ?= "-o ro,noatime,noload"
MKUBIFS_ARGS = "--min-io-size 4096 --leb-size 516096 --max-leb-cnt 8139"

TCC_ARCH_FAMILY ?= "${@d.getVar("SOC_FAMILY").split(":")[0]}"
TCC_MACHINE_FAMILY ?= "${TCC_ARCH_FAMILY}-${@d.getVar("SOC_FAMILY").split("-")[-1]}"

MACHINE_FEATURES = "usbgadget usbhost vfat alsa"
MACHINE_FEATURES += "${@oe.utils.conditional('BOOT_STORAGE', 'ufs', 'ufs', 'emmc', d)}"

# initramfs -> pivot root
# default rootfs is initramfs
# If you do not want to use the initramfs,
# you have to set 'INITRAMFS_IMAGE' and 'INITRAMFS_TASK' to NULL("") on your conf/local.conf

INITRAMFS_FSTYPES ?= "cpio"
INITRAMFS_IMAGE ?= "initramfs-topst-image"
INITRAMFS_IMAGE_BUNDLE = "1"

KERNEL_IMAGETYPES ?= "${@bb.utils.contains('IMAGE_FEATURES', 'uboot-fit', 'fitImage', '', d)}"
KERNEL_CLASSES ?= "${@bb.utils.contains('IMAGE_FEATURES', 'uboot-fit', 'kernel-fitimage', '', d)}"
UBOOT_ENTRYPOINT ?= "0x20080000"
UBOOT_DTB_LOADADDRESS ?= "0x23000000"

EXTRA_CMDLINE += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'fsck.repair=yes', '', d)}"
CMDLINE ?= "'quiet ${EXTRA_CMDLINE}'"

KCONFIG_MODE ?= "--alldefconfig"

KERNEL_MACH_PATH ?= "arch/arm/mach-${TCC_ARCH_FAMILY}"
TARGET_ARCH_ALIAS ?= "${TARGET_ARCH}"
TARGET_ARCH_ALIAS:aarch64 ?= "arm64"

require conf/machine/include/soc-family.inc

LINUX_VERSION ??= "5.10.205"
LINUXLIBCVERSION ?= "5.10"
LINUX_VERSION_EXTENSION = "-tcc"

TCC_BSP_FEATURES:tcc805x ??= "audio display graphics camera"
TCC_BSP_FEATURES:tcc807x ??= "audio display graphics camera"
TCC_BSP_FEATURES:tcc750x ??= ""
TOPST_FEATURES ??= ""
TCC_BSP_FEATURES:append = "${@bb.utils.contains('DISTRO_FEATURES', 'wayland', ' drm', '', d)}"
TCC_BSP_FEATURES:append = "${@bb.utils.contains('TOPST_FEATURES', 'optee', ' optee', '', d)}"
TCC_BSP_FEATURES:append = "${@bb.utils.contains('TOPST_FEATURES', 'with-subcore', ' with-subcore camera', '', d)}"
TCC_BSP_FEATURES:append = "${@bb.utils.contains('TOPST_FEATURES', 'gpu-vz', ' gpu-vz', '', d)}"
TCC_BSP_FEATURES:append = "${@bb.utils.contains('TOPST_FEATURES', 'network', ' network', '', d)}"
TCC_BSP_FEATURES:append:tcc805x = "${@bb.utils.contains('TOPST_FEATURES', 'multimedia', ' multimedia_vpu_v3', '', d)}"
TCC_BSP_FEATURES:append:tcc807x = "${@bb.utils.contains('TOPST_FEATURES', 'multimedia', ' multimedia_vpu_v3', '', d)}"

TCC_BSP_FEATURES:append:tcc750x = "${@bb.utils.contains('TOPST_FEATURES', 'display', ' display', '', d)}"
TCC_BSP_FEATURES:append:tcc750x = "${@bb.utils.contains('TOPST_FEATURES', 'camera', ' camera', '', d)}"
TCC_BSP_FEATURES:append:tcc750x = "${@bb.utils.contains('TOPST_FEATURES', 'multimedia', ' multimedia_vpu_v3', '', d)}"

FWDN_VERSION = "8"
DATA_PART_FSTYPE = "ext4"

STORAGE_SIZE ?= "${@oe.utils.conditional('BOOT_STORAGE', 'ufs', '${TCC_UFS_STORAGE_SIZE}', '${TCC_EMMC_STORAGE_SIZE}', d)}"
# Set system partition size. MB
SYSTEM_PARTITION_SIZE ?= "20480"
# Set maximum size of user rw parition. MB
MAX_DATA_SIZE ?= "1024"
# Set size of home rw parition. MB
HOME_DATA_SIZE ?= "500"

SPLASH_IMAGE ?= "${@oe.utils.conditional('BOOT_STORAGE', 'ufs', 'splash_1920x720x32_ufs.img', 'splash_1920x720x32.img', d)}"
SPLASH_IMAGE:tcc803x = "splash_1920x720x32_tcc803x.img"
