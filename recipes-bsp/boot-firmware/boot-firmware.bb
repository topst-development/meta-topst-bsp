DESCRIPTION = "Telechips prebuilt boot firmwares"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"
SECTION = "bsp"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}/${CHIP_PATH}:"

require ${TCC_ARCH_FAMILY}.inc

inherit deploy

CHIP_PATH = "${@d.getVar("MACHINE").split("-")[0]}"
PACKAGE_ARCH = "${MACHINE_ARCH}"

SRC_URI = "${TELECHIPS_TOPST_GIT}/boot-firmware.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
TOPST_BRANCH:tcc805x ??= "release/d3/1.0.0"
TOPST_BRANCH:tcc750x ??= "release/ai/1.0.0"

S = "${WORKDIR}/git"
PATCHTOOL = "git"

do_deploy() {
	install -d ${DEPLOYDIR}
	cp -ap ${D}/boot-firmware ${DEPLOYDIR}
}
addtask deploy after do_install

do_configure[noexec] = "1"
do_compile[noexec] = "1"

FILES:${PN} = "boot-firmware"
