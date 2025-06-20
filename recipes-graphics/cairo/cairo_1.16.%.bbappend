PACKAGECONFIG ??= " \
	trace \
	${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11 xcb', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'glesv2', '', d)} \
"
