# Check the Telechips license
LICENSE = "Telechips"
SECTION = "hsm"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/Telechips;md5=bf748a8e7a397a71f48f21715741f8a1"

# Download hsm_test code through git url
SRC_URI = "${TELECHIPS_TOPST_GIT}/hsm.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRCREV = "${AUTOREV}"

do_compile[noexec] = "1"

INSANE_SKIP:${PN}-dev = "ldflags already-stripped dev-elf dev-deps"
INSANE_SKIP:${PN} = "ldflags dev-so dev-deps already-stripped"

S = "${WORKDIR}/git"
B = "${WORKDIR}"

HSM_BIT:arm = "32"
HSM_BIT:aarch64 = "64"

HSM_CHIP = "${@bb.utils.contains_any('TCC_ARCH_FAMILY', 'tcc803x tcc803xp', 'M4', 'SC000', d)}"

do_install() {
	install -d ${D}${bindir}
	if ${@bb.utils.contains_any('TCC_ARCH_FAMILY', "tcc803x tcc803xp tcc805x tcc807x tcc750x", 'true', 'false', d)}; then
		make -C ${S}/hsm_test/${HSM_CHIP} TARGET_BIT=${HSM_BIT} CHIP=${HSM_CHIP} YOCTO_BUILD=y
		install -m 0755 ${S}/hsm_test/${HSM_CHIP}/out/hsm_test_${HSM_BIT}_${HSM_CHIP} ${D}${bindir}/
	fi
}
