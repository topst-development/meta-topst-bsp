SRC_URI:append = " file://micom.cfg"

SRCREV = "01e25d35b39684235328df3222d9edc5d05c1bd5"

do_install() {
	install -d ${D}/boot-firmware
	install -d ${D}/boot-firmware/prebuilt
	install -d ${D}/boot-firmware/prebuilt/ufs

	if ${@bb.utils.contains_any('TCC_BSP_FEATURES', 'with-subcore', 'false', 'true', d)}; then
		sed -i 's%\(bootsel="\)dual%\1single%g'							${S}/tools/bconf_maker/tcc805x_bconf.xml
	fi

	python3 ${S}/tools/bconf_maker/bconf_maker_v01.py -i ${S}/tools/bconf_maker/tcc805x_bconf.xml -o ${D}/boot-firmware/prebuilt/bconf.bin

	install -m 0644 ${S}/prebuilt/mcert.bin 				${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/dram_params.bin 			${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/hsm.bin	 				${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/hsm.cs.bin 				${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/fwdn.rom 					${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/optee.rom 				${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/subcore_optee.rom 			${D}/boot-firmware/prebuilt
	install -m 0644 ${S}/prebuilt/scfw.rom                  ${D}/boot-firmware/prebuilt

	if ${@oe.utils.conditional('BOOT_STORAGE', 'ufs', 'true', 'false', d)}; then
		install -m 0644 ${S}/prebuilt/ufs/ca72_bl1.rom 		${D}/boot-firmware/prebuilt/ufs
		install -m 0644 ${S}/prebuilt/ufs/ca72_bl2.rom 		${D}/boot-firmware/prebuilt/ufs
		install -m 0644 ${S}/prebuilt/ufs/ca53_bl1.rom 		${D}/boot-firmware/prebuilt/ufs
		install -m 0644 ${S}/prebuilt/ufs/ca53_bl2.rom 		${D}/boot-firmware/prebuilt/ufs
		install -m 0644 ${S}/tcc805x_ufs_boot.json			${D}/boot-firmware/boot.json
	else
		install -m 0644 ${S}/prebuilt/ca72_bl1.rom 			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ca72_bl2.rom 			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ca53_bl1.rom 			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/prebuilt/ca53_bl2.rom 			${D}/boot-firmware/prebuilt
		install -m 0644 ${S}/tcc805x_boot.cs.json		 	${D}/boot-firmware/boot.json
	fi

	install -m 0644 ${S}/tcc805x_fwdn.cs.json				${D}/boot-firmware/fwdn.json

	sed -i 's%SOURCE_DIR%${S}%g' ${WORKDIR}/micom.cfg
	${S}/tools/tcc805x_snor_mkimage/tcc805x-snor-mkimage -i ${WORKDIR}/micom.cfg -o ${D}/boot-firmware/prebuilt/tcc8050_snor.cs.rom
}
