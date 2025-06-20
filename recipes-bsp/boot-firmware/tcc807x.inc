SRC_URI:append = " file://micom.cfg"

SRCREV = "cdd28a25e77c90f08edb7fe7571565fbaf13e08f"

do_install() {
	install -d ${D}/boot-firmware
	install -d ${D}/boot-firmware/prebuilt

	if ${@bb.utils.contains_any('TCC_BSP_FEATURES', 'with-subcore', 'false', 'true', d)}; then
		sed -i 's%\(bootsel="\)dual%\1single%g'							${S}/tools/bconf_maker/tcc807x_bconf.xml
	fi
	python3 ${S}/tools/bconf_maker/bconf_maker_v01.py -i ${S}/tools/bconf_maker/tcc807x_bconf.xml -o ${D}/boot-firmware/prebuilt/bconf.bin

	install -m 0644 ${S}/prebuilt/dram_params.bin			${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/fwdn.rom					${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/hsm.bin					${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/mcert.bin					${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/optee.rom					${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/subcore_optee.rom 			${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/scfw.rom					${D}/boot-firmware/prebuilt

	if ${@oe.utils.conditional('BOOT_STORAGE', 'ufs', 'true', 'false', d)}; then
		install -m 0644 ${S}/prebuilt/ufs/ap0_bl1.rom		${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ufs/ap0_bl2.rom		${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ufs/ap0_lib.rom		${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ufs/ap1_bl1.rom		${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ufs/ap1_bl2.rom		${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ufs/ap1_lib.rom		${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/tcc807x_ufs_boot.json			${D}/boot-firmware/boot.json
		sed -i 's%\\\\ufs%%g'								${D}/boot-firmware/boot.json
	else
		install -m 0644 ${S}/prebuilt/ap0_bl1.rom			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ap0_bl2.rom			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ap0_lib.rom			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ap1_bl1.rom			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ap1_bl2.rom			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ap1_lib.rom			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/tcc807x_boot.json				${D}/boot-firmware/boot.json
	fi
	install -m 0644 ${S}/tcc807x_fwdn.json					${D}/boot-firmware/fwdn.json

	sed -i 's%SOURCE_DIR%${S}%g' ${WORKDIR}/micom.cfg
	${S}/tools/snor_mkimage/snor_mkimage -i ${WORKDIR}/micom.cfg -o ${D}/boot-firmware/prebuilt/${TCC_ARCH_FAMILY}_snor.rom
}
