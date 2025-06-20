FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
	file://${TCC_ARCH_FAMILY}/01-telechips.conf \
	${@bb.utils.contains('DISTRO_FEATURES', 'bluetooth', 'file://02-bluez.conf', '', d)} \
"
do_install:append() {
	install -d ${D}${sysconfdir}/alsa/conf.d
	install -m 0644 ${WORKDIR}/${TCC_ARCH_FAMILY}/01-telechips.conf ${D}${sysconfdir}/alsa/conf.d/

	if ${@bb.utils.contains('DISTRO_FEATURES', 'bluetooth', 'true', 'false', d)}; then
		install -d ${D}${sysconfdir}
		install -m 0644 ${WORKDIR}/02-bluez.conf ${D}${sysconfdir}
	fi
}

