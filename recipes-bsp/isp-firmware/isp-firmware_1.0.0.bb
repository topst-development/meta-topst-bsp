DESCRIPTION = "Telechips ISP Firmware"
SECTION = "BSP"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"

SRC_URI = " \
	file://topst_ov5647.bin \
	file://topst_isp_fw_ov5647.btset \
"

S = "${WORKDIR}"

do_install() {
	install -d ${D}${base_libdir}/firmware

	install -m 644 ${S}/topst_ov5647.bin			${D}${base_libdir}/firmware/
	install -m 644 ${S}/topst_isp_fw_ov5647.btset	${D}${base_libdir}/firmware/

	ln -s topst_ov5647.bin							${D}${base_libdir}/firmware/tcc-isp-fw
	ln -s topst_isp_fw_ov5647.btset					${D}${base_libdir}/firmware/tcc-isp-setting-0
}

FILES:${PN} += "${base_libdir}"
