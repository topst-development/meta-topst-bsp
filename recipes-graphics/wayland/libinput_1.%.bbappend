#FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

#SRC_URI += " \
#	file://0001-tools-add-libinput-analyze-to-the-libinput-tool-help.patch \
#	file://0002-udev-don-t-use-IMPORT.patch \
#	file://0003-Fix-race-condition-causing-duplicate-devices-in-udev.patch \
#"

do_install:append() {
	rm -f ${D}${base_libdir}/udev/rules.d/80-libinput-device-groups.rules
	rm -f ${D}${base_libdir}/udev/rules.d/90-libinput-model-quirks.rules
}

