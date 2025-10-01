SUMMARY = "Universal Boot Loader for embedded devices"
HOMEPAGE = "http://www.denx.de/wiki/U-Boot/WebHome"
SECTION = "bootloaders"
PROVIDES = "virtual/bootloader"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=5a7450c57ffe5ae63fd732446b988025"

SRC_URI = "${TELECHIPS_TOPST_GIT}/u-boot.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRC_URI += "${@bb.utils.contains('IMAGE_FEATURES', 'uboot-fit', 'file://fit.cfg', '', d)}"
SRC_URI += "${@bb.utils.contains('IMAGE_FEATURES', 'uboot-net', 'file://boot-net.cfg', '', d)}"

TOPST_BRANCH:tcc805x = "develop/d3"
TOPST_BRANCH:tcc750x = "develop/ai"
SRCREV:tcc805x = "${AUTOREV}"
SRCREV:tcc750x = "${AUTOREV}"

require recipes-bsp/u-boot/u-boot.inc

DEPENDS += "bison-native"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

inherit pkgconfig

UBOOT_BINARY = "${UBOOT_NAME}.${UBOOT_SUFFIX}"
UBOOT_ARCH_ARGS:arm = "ARCH=arm"
UBOOT_ARCH_ARGS:aarch64 = "ARCH=arm64 "

PATCHTOOL = "git"

do_compile:prepend() {
	export ${UBOOT_ARCH_ARGS} DEVICE_TREE=${UBOOT_DEVICE_TREE}
}

do_configure[cleandirs] = "${B}"

python __anonymous() {
    bsp_features = d.getVar('TCC_BSP_FEATURES', True)
    boot_storage = d.getVar('BOOT_STORAGE', True)
    src_uri = d.getVar('SRC_URI').split()

    if boot_storage == 'ufs':
        src_uri.append('file://boot-ufs.cfg')
    elif boot_storage == 'emmc':
        src_uri.append('file://boot-emmc.cfg')

    if 'display' in bsp_features:
        src_uri.append('file://display.cfg')

    if 'gpu-vz' in bsp_features:
        src_uri.append('file://gpu-vz.cfg')

    d.setVar('SRC_URI', ' '.join(src_uri))
}
