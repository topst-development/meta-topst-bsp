DESCRIPTION = "Telechips Make Image tools"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"
SECTION = "bsp"

inherit native

SRC_URI = "${TELECHIPS_TOPST_GIT}/mktcimg.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRCREV = "0fbbb075788b54f1f94ca7ff3e701f2a0e716700"

S = "${WORKDIR}/git"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/mktcimg		${D}${bindir}
}

do_configure[noexec] = "1"
do_compile[noexec] = "1"

BBCLASSEXTEND = "native"
