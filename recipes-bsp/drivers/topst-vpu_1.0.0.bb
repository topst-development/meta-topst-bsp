DESCRIPTION = "Telechips VPU Drivers"

SECTION = "kernel/modules"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"

inherit module cmake

SRC_URI = "${TELECHIPS_TOPST_GIT}/vpu-kernel-library.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRCREV = "0d0ad44d16ac526d979a85af5f5234ae6ce99ceb"

SRC_URI += " file://00.vpu-lib.conf "

PATCHTOOL = "git"
LINKER_HASH_STYLE = "sysv"

S="${WORKDIR}/git"

PACKAGES += "kernel-modules-vpu"

VPU_INSTALL_DIR = "${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu_v3/lib"

do_configure:prepend() {
	export KERNEL_BUILD_DIR=${STAGING_KERNEL_BUILDDIR}
	export CHIPSET=${TCC_ARCH_FAMILY}
}

do_configure:prepend() {
	export KERNEL_BUILD_DIR=${STAGING_KERNEL_BUILDDIR}
	export CHIPSET=${TCC_ARCH_FAMILY}
}

do_install() {
    install -d ${VPU_INSTALL_DIR}
    install -d ${D}${sysconfdir}/modules-load.d
    install -d ${D}${nonarch_base_libdir}/firmware

	if ${@bb.utils.contains('TCC_ARCH_FAMILY', 'tcc805x', 'true', 'false', d)}; then
		install -m 0644 ${WORKDIR}/build/vpu_c7/vpu_lib.ko				${VPU_INSTALL_DIR}/vpu_c7_lib.ko
		install -m 0644 ${WORKDIR}/build/vpu_4k_d2/vpu_4k_d2_lib.ko		${VPU_INSTALL_DIR}/vpu_4k_d2_lib.ko
		install -m 0644 ${S}/firmware/vpu_c7.bin						${D}${nonarch_base_libdir}/firmware/
		install -m 0644 ${S}/firmware/vpu4k_d2.bin						${D}${nonarch_base_libdir}/firmware/
	fi

	install -m 0644 ${WORKDIR}/build/vpu_hevc_enc/vpu_hevc_enc_lib.ko	${VPU_INSTALL_DIR}/vpu_hevc_enc_lib.ko
	install -m 0644 ${S}/firmware/hevc_e3.bin							${D}${nonarch_base_libdir}/firmware/
	install -m 0644 ${WORKDIR}/00.vpu-lib.conf							${D}${sysconfdir}/modules-load.d/
}

FILES:${PN} += " \
	${sysconfdir}/modules-load.d/00.vpu-lib.conf \
	${nonarch_base_libdir}/firmware \
"

RPROVIDES:${PN} += "kernel-modules-vpu"
OECMAKE_GENERATOR = "Unix Makefiles"
