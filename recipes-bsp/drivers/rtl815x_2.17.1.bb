DESCRIPTION = "DSM driver for realtek RTL8152/RTL8153/RTL8156 based USB Ethernet adapters"
SECTION = "kernel/modules"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "git://github.com/bb-qq/r8152.git;protocol=https;branch=master \
	file://0001-change-cross-compile-option-at-makefile-for-YP.patch \
"
SRCREV = "e3b377ad963b8ee88f4a9a14eb401f1be1eac6f2"

PATCHTOOL = "git"

inherit module

DEPENDS += "virtual/kernel"

S="${WORKDIR}/git"

EXTRA_OEMAKE += "KERNELDIR='${STAGING_KERNEL_DIR}'"
export INSTALL_MOD_DIR="kernel/drivers/net/usb"

PACKAGES =+ "${PN}-udev-rules"

do_install:append() {
	echo "r8152" > ${D}${sysconfdir}/modules-load.d/rtl815x.conf

	install -d ${D}${sysconfdir}/udev/rules.d
	install -m 0644 ${S}/50-usb-realtek-net.rules	${D}${sysconfdir}/udev/rules.d
	install -m 0644 ${S}/51-usb-r8152-net.rules		${D}${sysconfdir}/udev/rules.d
}

RDEPENDS:${PN} += "${PN}-udev-rules"

FILES:${PN}-udev-rules += " \
	${sysconfdir}/udev/rules.d \
"
FILES:${PN} += " \
	${sysconfdir}/modules-load.d \
"
