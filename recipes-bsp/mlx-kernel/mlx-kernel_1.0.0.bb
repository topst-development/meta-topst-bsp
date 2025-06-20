DESCRIPTION = "MLX Kernel for NN Application"
SECTION = "applications"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"

SRC_URI = "file://mlx_kernel.bin"

do_install:append() {
	install -d ${D}${base_libdir}/firmware
	install -m 0755 ${WORKDIR}/mlx_kernel.bin ${D}${base_libdir}/firmware/mlx_kernel.bin
}

FILES:${PN} += " \
	${base_libdir} \
	"
