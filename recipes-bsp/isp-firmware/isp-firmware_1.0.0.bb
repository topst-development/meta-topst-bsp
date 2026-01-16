DESCRIPTION = "Telechips ISP Firmware"
SECTION = "BSP"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"

SRC_URI:tcc805x = " \
	file://topst_ov5647.bin \
	file://topst_isp_fw_d3_ov5647.btset \
	file://topst_isp_fw_d3_imx219.btset \
"

SRC_URI:tcc750x = " \
	file://topst_ov5647.bin \
	file://topst_isp_fw_ai_ov5647.btset \
	file://topst_isp_fw_ai_imx219.btset \
"

DEPENDS += " virtual/kernel"
S = "${WORKDIR}"

do_install:tcc805x() {
	install -d ${D}${base_libdir}/firmware

	install -m 644 ${S}/topst_ov5647.bin				${D}${base_libdir}/firmware/

	rm -f ${D}${base_libdir}/firmware/tcc-isp-setting-0

	if ${@bb.utils.contains('TOPST_CAM_MODULE', 'ov5647', 'true', 'false', d)}; then
		install -m 644 ${S}/topst_isp_fw_d3_ov5647.btset	${D}${base_libdir}/firmware/
		ln -sf topst_isp_fw_d3_ov5647.btset ${D}${base_libdir}/firmware/tcc-isp-setting-0
	fi
	if ${@bb.utils.contains('TOPST_CAM_MODULE', 'imx219', 'true', 'false', d)}; then
		install -m 644 ${S}/topst_isp_fw_d3_imx219.btset	${D}${base_libdir}/firmware/
		ln -sf topst_isp_fw_d3_imx219.btset ${D}${base_libdir}/firmware/tcc-isp-setting-0
	fi
}

do_install:tcc750x() {
	install -d ${D}${base_libdir}/firmware

	install -m 644 ${S}/topst_ov5647.bin				${D}${base_libdir}/firmware/

	rm -f ${D}${base_libdir}/firmware/tcc-isp-setting-0

	if ${@bb.utils.contains('TOPST_CAM_MODULE', 'ov5647', 'true', 'false', d)}; then
		install -m 644 ${S}/topst_isp_fw_ai_ov5647.btset	${D}${base_libdir}/firmware/
		ln -sf topst_isp_fw_ai_ov5647.btset ${D}${base_libdir}/firmware/tcc-isp-setting-0
	fi
	if ${@bb.utils.contains('TOPST_CAM_MODULE', 'imx219', 'true', 'false', d)}; then
		install -m 644 ${S}/topst_isp_fw_ai_imx219.btset	${D}${base_libdir}/firmware/
		ln -sf topst_isp_fw_ai_imx219.btset ${D}${base_libdir}/firmware/tcc-isp-setting-0
	fi
}

FILES:${PN} += "${base_libdir}"
