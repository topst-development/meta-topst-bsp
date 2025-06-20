SRC_URI:append = ""

SRCREV = "e4d14bd6dc498ff9451f0df8ae07f7572ca76387"

do_install() {
    install -d ${D}/boot-firmware
    install -d ${D}/boot-firmware/prebuilt

    install -m 0644 ${S}/prebuilt/bconf.bin             ${D}/boot-firmware/prebuilt
    install -m 0644 ${S}/prebuilt/mcert.bin             ${D}/boot-firmware/prebuilt
    install -m 0644 ${S}/prebuilt/dram_params.bin       ${D}/boot-firmware/prebuilt
    install -m 0644 ${S}/prebuilt/hsm.bin               ${D}/boot-firmware/prebuilt
    install -m 0644 ${S}/prebuilt/fwdn.rom              ${D}/boot-firmware/prebuilt
    install -m 0644 ${S}/prebuilt/optee.rom             ${D}/boot-firmware/prebuilt

    install -m 0644 ${S}/prebuilt/ca53_bl1.rom          ${D}/boot-firmware/prebuilt
    install -m 0644 ${S}/prebuilt/ca53_bl2.rom          ${D}/boot-firmware/prebuilt

    install -m 0644 ${S}/tcc750x_fwdn.json              ${D}/boot-firmware
    install -m 0644 ${S}/tcc750x_boot.json              ${D}/boot-firmware
}
