FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://50-topst-default.rules"

do_install:append() {
	install -m 0644 ${WORKDIR}/50-topst-default.rules	${D}${rootlibexecdir}/udev/rules.d/
}

FILES:${PN}-udev-rules += "${rootlibexecdir}/udev/rules.d/50-topst-default.rules"
