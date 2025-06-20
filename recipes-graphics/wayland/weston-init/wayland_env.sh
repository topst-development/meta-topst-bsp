#!/bin/sh

### Wayland Environment Variables ###

export XDG_CONFIG_HOME=/usr/share/weston
export XDG_RUNTIME_DIR=/run/user/root
export WESTON_DISABLE_GBM_MODIFIERS=0
export WS_CALUDEV_FILE=/home/root/.telechips/wayland/wl_calibrate.rules
export WS_CALUDEV_TMPDIR=/etc/udev/rules.d

if ! test -d "${XDG_RUNTIME_DIR}"; then
	mkdir -p "${XDG_RUNTIME_DIR}"
	chmod 0700 "${XDG_RUNTIME_DIR}"
fi
