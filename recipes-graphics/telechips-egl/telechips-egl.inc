SUMMARY = "Telechips Graphic library"
DESCRIPTION = "Graphic libraries for telechips SoCs for a family of Mali GPU"
HOMEPAGE = "http://www.telechips.com"
SECTION = "libs"
LICENSE = "ARM"
LIC_FILES_CHKSUM = "file://${THISDIR}/../../licenses/ARM;md5=75b66ff7a76b0f648ed1db4b275073a3"

DEPENDS = "${@bb.utils.contains('TCC_BSP_FEATURES', 'drm', 'libdrm', '', d)}"

PROVIDES = "virtual/egl virtual/libgles1 virtual/libgles2 virtual/mesa"
PROVIDES:append:tcc803x = " virtual/libgles3"
PROVIDES:append:tcc807x = " virtual/libgles3 opencl-headers virtual/opencl-icd"
PROVIDES:append = "${@bb.utils.contains('DISTRO_FEATURES', 'wayland', ' virtual/libgbm virtual/libwayland-egl', ' ', d)}"

inherit gettext features_check

# check mandatory features
REQUIRED_DISTRO_FEATURES = "opengl"

PACKAGECONFIG ??= "egl gles \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'wayland', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'vulkan', 'vulkan', '', d)} \
	${@bb.utils.contains('TCC_BSP_FEATURES', 'opencl', 'opencl', '', d)} \
"

PACKAGECONFIG[egl] = ",,"
PACKAGECONFIG[gles] = ",,"
PACKAGECONFIG[wayland] = ",,wayland"
PACKAGECONFIG[opencl] = ",,"
PACKAGECONFIG[vulkan] = ",,"

S = "${WORKDIR}/git"

PACKAGES = " \
	libmali-telechips \
	libegl-telechips libegl-telechips-dev \
	libgles1-telechips libgles1-telechips-dev \
	libgles2-telechips libgles2-telechips-dev \
	libgles3-telechips-dev \
	libwayland-egl libwayland-egl-dev \
	libgbm libgbm-dev \
	libopencl-telechips libopencl-telechips-dev \
	libvulkan-telechips \
"

TA = ""
GA:tcc897x = "utgard-400"
GA:tcc803x = "bifrost-g51"
GA:tcc807x = "valhall-g78ae"
MALI_VER:tcc897x = "r9"
WS = "${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'wayland', 'fbdev', d)}"
OCL = "${@bb.utils.contains('TCC_BSP_FEATURES', 'opencl', '-ocl', '', d)}"
FS = "${@bb.utils.contains('MACHINE', 'tcc8030-sub', '-full-strip', '', d)}"

MALI_SUPPORT_GLES3:tcc803x = "1"
MALI_SUPPORT_GLES3:tcc807x = "1"

python __anonymous() {
    tunes = d.getVar("TUNE_FEATURES", True)
    if not tunes:
        return

    if bb.utils.contains('TUNE_FEATURES', 'aarch64', True, False, d):
        d.setVar("TA", "aarch64-none-linux-gnu")
    else:
        d.setVar("TA", "arm-none-linux-gnueabihf")
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

do_preconfigure[noexec] = "1"
do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
	# Copy header files
	install -d ${D}${includedir}
	cp -ap ${S}/include/EGL				${D}${includedir}
	cp -ap ${S}/include/GLES			${D}${includedir}
	cp -ap ${S}/include/GLES2			${D}${includedir}
	if [ "${MALI_SUPPORT_GLES3}" = "1" ]; then
		cp -ap ${S}/include/GLES3		${D}${includedir}
	fi
	cp -ap ${S}/include/KHR				${D}${includedir}
	install -m 0644 ${S}/include/GBM/gbm.h			${D}${includedir}

	# Create MALI manifest
	install -d ${D}${libdir}
	install -m 0755 ${S}/lib/${TA}/libmali-${GA}-${MALI_VER}p0-${WS}${OCL}${FS}.so ${D}${libdir}/libmali.so.0

	ln -sf libmali.so.0 ${D}${libdir}/libEGL.so
	ln -sf libmali.so.0 ${D}${libdir}/libEGL.so.1
	ln -sf libmali.so.0 ${D}${libdir}/libGLESv1_CM.so
	ln -sf libmali.so.0 ${D}${libdir}/libGLESv1_CM.so.1
	ln -sf libmali.so.0 ${D}${libdir}/libGLESv2.so
	ln -sf libmali.so.0 ${D}${libdir}/libGLESv2.so.2
	ln -sf libmali.so.0 ${D}${libdir}/libgbm.so
	ln -sf libmali.so.0 ${D}${libdir}/libgbm.so.1

	if [ "${WS}" = "wayland" ]; then
		ln -sf libmali.so.0 ${D}${libdir}/libwayland-egl.so.1
		ln -sf libmali.so.0 ${D}${libdir}/libwayland-egl.so
	fi

	if ${@bb.utils.contains('TCC_BSP_FEATURES', 'opencl', 'true', 'false', d)}; then
		# Install OpenCL header files
		cp -ap ${S}/include/CL					${D}${includedir}
		# Install ARM ICDs for OpenCL
		install -d ${D}${sysconfdir}/OpenCL/vendors/
		install -m 0644 ${S}/etc/OpenCL/vendors/ARM.icd		${D}${sysconfdir}/OpenCL/vendors/ARM.icd
		ln -sf libmali.so.0 ${D}${libdir}/libOpenCL.so
		ln -sf libmali.so.0 ${D}${libdir}/libOpenCL.so.2
	fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'vulkan', 'true', 'false', d)}; then
		# Install vulkan icd and library
		install -m 755 -d ${D}${datadir}/vulkan
		install -m 0644 ${S}/lib/${TA}/vulkan/mali_icd.json ${D}${datadir}/vulkan/mali_icd.json

		install -m 755 -d ${D}${datadir}/vulkan/implicit_layer.d
		install -m 0644 ${S}/lib/${TA}/vulkan/implicit_layer.d/libVkLayer_window_system_integration.so ${D}${datadir}/vulkan/implicit_layer.d/
		install -m 0644 ${S}/lib/${TA}/vulkan/implicit_layer.d/VkLayer_window_system_integration.json ${D}${datadir}/vulkan/implicit_layer.d/
	fi

	# Copy pkgconfig files
	install -d ${D}${libdir}/pkgconfig
	install -m 0644 ${S}/pkgconfig/*.pc			${D}/${libdir}/pkgconfig

	if [ "${WS}" = "wayland" ]; then
		cd ${D}${libdir}/pkgconfig/
		sed -i "s/^Libs:/Libs:-lwayland-client -lwayland-server /" wayland-egl.pc
	fi
}

# For the packages that make up the OpenGL interfaces, inject variables so that
# they don't get Debian-renamed (which would remove the -telechips suffix), and
# RPROVIDEs/RCONFLICTs on the generic libgl name.
python __anonymous() {
    pkgconfig = (d.getVar('PACKAGECONFIG', True) or "").split()
    for p in (("egl", "libegl", "libegl1"),
              ("dri", "libgl", "libgl1"),
              ("gles", "libgles1", "libglesv1-cm1"),
              ("gles", "libgles2", "libglesv2-2"),
              ("gles", "libgles3",)):
        if not p[0] in pkgconfig:
            continue
        fullp = p[1] + "-telechips"
        pkgs = " ".join(p[1:])
        d.setVar("DEBIAN_NOAUTONAME:" + fullp, "1")
        d.appendVar("RREPLACES:" + fullp, pkgs)
        d.appendVar("RPROVIDES:" + fullp, pkgs)
        d.appendVar("RCONFLICTS:" + fullp, pkgs)

        # For -dev, the first element is both the Debian and original name
        fullp += "-dev"
        pkgs = p[1] + "-dev"
        d.setVar("DEBIAN_NOAUTONAME:" + fullp, "1")
        d.appendVar("RREPLACES:" + fullp, pkgs)
        d.appendVar("RPROVIDES:" + fullp, pkgs)
        d.appendVar("RCONFLICTS:" + fullp, pkgs)
}

RDEPENDS:libmali-telechips += " \
	libgles2-telechips \
	libgles1-telechips \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'libwayland-egl libgbm', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'vulkan', ' vulkan-headers vulkan-loader', '', d)} \
	${@bb.utils.contains('TCC_BSP_FEATURES', 'opencl', ' libopencl-telechips', '', d)} \
"
RDEPENDS:libgles1-telechips += " \
	libegl-telechips \
"
RDEPENDS:libgles2-telechips += " \
	libegl-telechips \
"
# as long as there is no libgles3-telechips: ship libgles3-telechips-dev along with
# libgles2-telechips-dev - otherwise GLES3 headers have to be added manually
RDEPENDS:libgles2-telechips-dev += "libgles3-telechips-dev"

LINKER_HASH_STYLE = "sysv"

FILES:libmali-telechips = "${libdir}/libmali${SOLIBS}"
FILES:libegl-telechips = "${libdir}/libEGL${SOLIBS}"
FILES:libgbm = "${libdir}/libgbm${SOLIBS}"
FILES:libgles1-telechips = "${libdir}/libGLESv1*${SOLIBS}"
FILES:libgles2-telechips = "${libdir}/libGLESv2${SOLIBS}"
FILES:libwayland-egl = "${libdir}/libwayland-egl${SOLIBS}"
FILES:libopencl-telechips = "${libdir}/libOpenCL${SOLIBS} ${sysconfdir}/OpenCL/vendors/ARM.icd"
FILES:libvulkan-telechips = "${datadir}/vulkan/mali_icd.json ${datadir}/vulkan/implicit_layer.d/*"

FILES:libegl-telechips-dev = "${libdir}/libEGL${SOLIBSDEV} ${includedir}/EGL ${includedir}/KHR ${libdir}/pkgconfig/egl.pc ${libdir}/pkgconfig/mali.pc"
FILES:libgbm-dev = "${libdir}/libgbm${SOLIBSDEV} ${includedir}/gbm.h ${libdir}/pkgconfig/gbm.pc"
FILES:libgles1-telechips-dev = "${libdir}/libGLESv1*${SOLIBSDEV} ${includedir}/GLES ${libdir}/pkgconfig/glesv1*.pc"
FILES:libgles2-telechips-dev = "${libdir}/libGLESv2${SOLIBSDEV} ${includedir}/GLES2 ${libdir}/pkgconfig/glesv2.pc"
FILES:libgles3-telechips-dev = "${includedir}/GLES3"
FILES:libwayland-egl-dev = "${libdir}/libwayland-egl${SOLIBSDEV} ${libdir}/pkgconfig/wayland-egl*.pc"
FILES:libopencl-telechips-dev = "${libdir}/libOpenCL${SOLIBSDEV} ${includedir}/CL"

INSANE_SKIP:libmali-telechips = "already-stripped ldflags dev-so"
INSANE_SKIP:libvulkan-telechips = "libdir"
