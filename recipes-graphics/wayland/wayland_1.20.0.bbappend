FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001-Add-setGeometry-protocol.patch"

do_install:append() {
	if ${@bb.utils.contains_any('TCC_ARCH_FAMILY', "tcc803x tcc807x", 'true', 'false', d)}; then
        rm -rf ${D}/${libdir}/libwayland-egl*
        rm -rf ${D}/${libdir}/pkgconfig/wayland-egl*
	fi
}
