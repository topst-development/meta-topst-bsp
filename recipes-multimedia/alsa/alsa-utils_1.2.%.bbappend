DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}"
DEPENDS:remove = "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'udev', '', d)}"
PACKAGECONFIG = "udev"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
	file://init\
"

EXTRA_OECONF += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '--with-systemdsystemunitdir=${sysconfdir}/system', '', d)}"

do_configure:prepend() {
	sed -i 's%basic%multi-user%g' ${S}/alsactl/Makefile.am
}

do_install:append() {
	install -m 0644 ${WORKDIR}/init/* ${D}${datadir}/alsa/init/
}

FILES:alsa-utils-alsactl += "${sysconfdir}"
