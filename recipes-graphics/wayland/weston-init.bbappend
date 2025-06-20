FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "${@bb.utils.contains('DISTRO_FEATURES','systemd', '${SYSTEMD_SERVICE_LIST}', '${INIT_SCRIPT_LIST}', d)}"

INIT_SCRIPT_LIST = " \
		file://init \
		file://wayland_env.sh \
		file://weston-start \
		file://weston-init \
"

SYSTEMD_SERVICE_LIST = " \
        file://weston.service \
        file://weston.socket \
        file://weston.env \
"

SYSTEMD_SERVICE:${PN} = "weston.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_install() {
	if ${@bb.utils.contains('DISTRO_FEATURES','systemd','true','false',d)}; then
		install -d ${D}${sysconfdir}/sysconfig
		install -d ${D}${systemd_unitdir}/system

		install -m 644 ${WORKDIR}/weston.service		${D}${systemd_system_unitdir}/weston.service
		install -m 644 ${WORKDIR}/weston.socket			${D}${systemd_system_unitdir}/weston.socket
		install -m 644 ${WORKDIR}/weston.env			${D}${sysconfdir}/sysconfig/weston
	else
        install -d ${D}${sysconfdir}/init.d
        install -m 755 ${WORKDIR}/init				${D}${sysconfdir}/init.d/weston

        install -d ${D}${sysconfdir}/profile.d
        install -m 0644 ${WORKDIR}/wayland_env.sh	${D}${sysconfdir}/profile.d/wayland_env.sh

		install -d ${D}${sysconfdir}/default
		install -m 0644 ${WORKDIR}/weston-init		${D}${sysconfdir}/default/weston

		install -d ${D}${bindir}
        install -m 755 ${WORKDIR}/weston-start		${D}${bindir}/weston-start

        #Touch calibration for ak4183
        if [ "${TOUCH_SCREEN_TYPE}" = "AK4183" ]; then
            install -d ${D}${sysconfdir}/udev/rules.d/
            install -m 0644 ${WORKDIR}/wl_calibrate.rules ${D}${sysconfdir}/udev/rules.d/wl_calibrate.rules
        fi

	fi
}

FILES:${PN} = " \
	${@bb.utils.contains('DISTRO_FEATURES','systemd', \
	'${systemd_system_unitdir}/weston.service ${systemd_system_unitdir}/weston.socket ${sysconfdir}/sysconfig/weston', \
	'${sysconfdir}/init.d/weston ${sysconfdir}/profile.d/wayland_env.sh ${sysconfdir}/default/weston ${bindir}/weston-start', d)} \
"
