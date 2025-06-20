FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
	file://weston.desktop.ini \
	${@bb.utils.contains("DISTRO_FEATURES", 'x11', 'file://weston.xwayland.ini', '', d)} \
	file://background.png \
	file://0001-Define-K_OFF-if-it-isn-t-defined-already.patch \
	file://0002-backend-drm-disable-bo-geometry-out-of-bounds-messag.patch \
	file://0003-meson-fix-failure-to-find-libudev-when-linking-the-c.patch \
	file://0004-Add-setGeometry-handling-function.patch \
	file://0005-gl-renderer-update-logic-for-dmabuf.patch \
	file://0006-weston-enable-background-alpha-blending-for-tcc.patch \
	file://0007-feat-gl-renderer-support-yuv-color-space-and-range.patch \
"

EXTRA_OEMESON += "-Ddeprecated-wl-shell=true"

SIMPLECLIENTS = "damage,im,egl,shm,touch,dmabuf-v4l,dmabuf-egl"

do_install:append() {
	install -d ${D}/${datadir}
	install -d ${D}/${datadir}/weston

	if ${@bb.utils.contains("DISTRO_FEATURES", 'x11', 'true', 'false', d)}; then
		install -m 0644 ${WORKDIR}/weston.xwayland.ini ${D}${datadir}/weston/weston.ini
	else
		install -m 0644 ${WORKDIR}/weston.desktop.ini ${D}${datadir}/weston/weston.ini
		install -m 0644 ${WORKDIR}/background.png ${D}${datadir}/weston
	fi
}

FILES_${PN} += "${datadir} ${sysconfdir}"
FILES_${PN}-dbg += "${libdir}/libweston-1/.debug/*.so"
