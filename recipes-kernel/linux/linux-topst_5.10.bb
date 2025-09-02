require linux-topst.inc

SRC_URI = "${TELECHIPS_TOPST_GIT}/kernel-5.10.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRC_URI:append = " \
	${@bb.utils.contains('IMAGE_FEATURES', 'uboot-fit', 'file://cmdline.cfg', '', d)} \
"
SRC_URI:append:tcc750x = " \
	${@bb.utils.contains('TOPST_FEATURES', 'pcie-host', 'file://pcie-host.cfg', '',d)} \
	${@bb.utils.contains('TOPST_FEATURES', 'display', 'file://display-vioc.cfg', '',d)} \
	${@bb.utils.contains('TOPST_FEATURES', 'camera', 'file://camera.cfg', '',d)} \
	file://wifi.cfg \
	file://expand-cma-512mb.cfg \
"
SRC_URI:append:tcc805x = " \
        file://pcie-host_d3.cfg \
        file://pcie-sata.cfg \
	file://pwm.cfg \
"

TOPST_BRANCH:tcc805x = "release/d3/2.0.0"
TOPST_BRANCH:tcc750x = "release/ai/1.0.0"
SRCREV:tcc805x = "10acd562a1a69f35ff904572642cde014fd832ec"
SRCREV:tcc750x = "7cd0622ce188fcd3204f5af4680edff442f0edd7"

LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

LINUX_VERSION = "5.10.205"
LINUX_VERSION:tcc750x = "5.10.223"
COMPATIBLE_MACHINE = "(tcc805x|tcc807x|tcc750x)"
KERNEL_EXTRA_ARGS:append:arm = " ARCH=arm"
KERNEL_EXTRA_ARGS:append:aarch64 = " ARCH=arm64"

KERNEL_OFFSET:arm = "0x8000"
KERNEL_OFFSET:aarch64 = "0x0"

kernel_do_install:append() {
	if [ -e "${D}${KERNEL_SRC_PATH}/tools/gator/daemon/escape" ]; then
		rm ${D}${KERNEL_SRC_PATH}/tools/gator/daemon/escape
	fi

	rm -rf ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu/vpu_hevc_enc_lib*
	rm -rf ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu/vpu_lib*
	rm -rf ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu/hevc_lib*
	rm -rf ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu/vpu_4k_d2_lib*
	rm -rf ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu/jpu_lib*
	rm -rf ${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/char/vpu/lib*
}
