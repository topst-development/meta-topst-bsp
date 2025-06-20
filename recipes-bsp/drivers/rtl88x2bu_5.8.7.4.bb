DESCRIPTION = "Realtek WiFi Drivers"
SECTION = "kernel/modules"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=ab842b299d0a92fb908d6eb122cd6de9"

inherit module

SRC_URI = "file://rtl88x2bu-5.8.7.4.tar.gz"
SRC_URI[md5sum] = "5bf0496551d6cf761f99158f647bad41"
SRC_URI[sha256sum] = "894298cf2dbd218d5d1c90adeb652edb696df93c91d11ecd08773834b6dea8fe"

S = "${WORKDIR}/rtl88x2bu-5.8.7.4"

RTL_INSTALL_DIR = "/${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless"
EXTRA_OEMAKE += "KSRC='${STAGING_KERNEL_BUILDDIR}' MODDESTDIR='${RTL_INSTALL_DIR}' KVER='${KERNEL_VERSION}'"

do_install() {
	install -d ${D}${RTL_INSTALL_DIR}
	install -d ${D}${sysconfdir}/modules-load.d

	install -m 0644 ${WORKDIR}/rtl88x2bu-5.8.7.4/88x2bu.ko      ${D}${RTL_INSTALL_DIR}/
	install -m 0644 ${WORKDIR}/rtl88x2bu-5.8.7.4/88x2bu.conf    ${D}${sysconfdir}/modules-load.d/
}

RPROVIDES:${PN} += "kernel-module-88x2bu"
