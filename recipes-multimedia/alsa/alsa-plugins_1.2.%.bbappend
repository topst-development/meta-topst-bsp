FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://50-pulseaudio.conf;subdir=${S}/pulse"
SRC_URI:append = " file://0001-add-set-pa-prop-media-role.patch"

do_install:append() {
	rm -f ${D}${datadir}/alsa/alsa.conf.d/99-pulseaudio-default.conf
	rm -f ${D}${sysconfdir}/alsa/conf.d/99-pulseaudio-default.conf
}
