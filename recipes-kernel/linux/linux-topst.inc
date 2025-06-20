inherit kernel
inherit kernel-yocto
inherit pkgconfig

LICENSE = "GPL-2.0-only"

S = "${WORKDIR}/git"

KBRANCH = "${TOPST_BRANCH}"
KERNEL_CC:append = " ${TOOLCHAIN_OPTIONS}"
KERNEL_IMAGETYPE = "Image"

SRCREV_machine = "${SRCREV}"

PV = "${LINUX_VERSION}"

RAMDISK_NAME="ramdisk_dummy.rom"

BOOT_IMAGE_SUFFIX ?= "img"
BOOT_IMAGE = "${MACHINE}-tc-boot-${PV}-${PR}.${BOOT_IMAGE_SUFFIX}"
BOOT_IMAGE_BINARY = "tc-boot.${BOOT_IMAGE_SUFFIX}"
BOOT_IMAGE_SYMLINK = "tc-boot-${MACHINE}.${BOOT_IMAGE_SUFFIX}"

INSANE_SKIP:${PN} += "installed-vs-shipped"
PATCHTOOL = "git"

python __anonymous() {
    bsp_features = d.getVar('TCC_BSP_FEATURES', True)
    tcc_arch_family = d.getVar('TCC_ARCH_FAMILY', True)
    boot_storage = d.getVar('BOOT_STORAGE', True)
    src_uri = d.getVar('SRC_URI').split()

    src_uri.append('file://initramfs-disable.cfg')

    if boot_storage == 'ufs':
        src_uri.append('file://boot-ufs.cfg')
    elif boot_storage == 'emmc':
        src_uri.append('file://boot-emmc.cfg')

    if 'network' in bsp_features:
        src_uri.append('file://network.cfg')

    if 'audio' in bsp_features:
        src_uri.append('file://audio.cfg')

    if 'display' in bsp_features:
        src_uri.append('file://display-vioc.cfg')
        if 'drm' in bsp_features:
            src_uri.append('file://display-drm.cfg')
        else:
            src_uri.append('file://display-fb.cfg')

    if 'graphics' in bsp_features:
        if tcc_arch_family == 'tcc805x':
            if 'drm' in bsp_features:
                src_uri.append('file://gpu-drm.cfg')
            else:
                src_uri.append('file://gpu-fbdev.cfg')
        else:
            src_uri.append('file://gpu.cfg')

    if 'camera' in bsp_features:
        if tcc_arch_family == 'tcc805x':
            src_uri.append('file://camera.cfg')

    if 'with-subcore' in bsp_features:
        if 'gpu-vz' in bsp_features:
            src_uri.append('file://gpu-vz.cfg')
        if 'camera' in bsp_features:
            src_uri.append('file://camipc.cfg')
        if 'screen-share' in bsp_features:
            src_uri.append('file://screen-share.cfg')

    d.setVar('SRC_URI', ' '.join(src_uri))
}

do_tc_make_image() {
	cd ${D}/${KERNEL_IMAGEDEST}
	touch ${RAMDISK_NAME}
	kernel_image_name="${KERNEL_IMAGETYPE}-${KERNEL_VERSION}"
	if [ ! -z "${INITRAMFS_IMAGE}" -a x"${INITRAMFS_IMAGE_BUNDLE}" = x1 ]; then
		kernel_image_name="${B}/${KERNEL_OUTPUT_DIR}/${KERNEL_IMAGETYPE}.initramfs"
	fi

	python3 ${S}/scripts/mkbootimg --kernel $kernel_image_name --ramdisk ${RAMDISK_NAME} --base ${KERNEL_BASE_ADDR} --kernel_offset ${KERNEL_OFFSET} --output ${BOOT_IMAGE} --cmdline ${CMDLINE}

	rm  ${RAMDISK_NAME}
	cd -
}

addtask tc_make_image before do_deploy after do_bundle_initramfs

do_configure:prepend() {
	if ${@bb.utils.contains('IMAGE_FEATURES', 'uboot-fit', 'true', 'false', d)}; then
		default_cmdline=${CMDLINE}
		echo "CONFIG_CMDLINE=\"$default_cmdline\"" >> ${B}/.config
	fi
}

do_compile:prepend() {
	# gpu_configuration.sh needs .config file for separate GPU VZ / Non-VZ
	ln -sf ${B}/.config ${STAGING_KERNEL_DIR}/.config
	cd ${STAGING_KERNEL_DIR}

	if ${@bb.utils.contains('TCC_BSP_FEATURES', "graphics", 'true', 'false', d)}; then
		./${TCC_ARCH_FAMILY}_customized_gpu_configuration.sh
	fi
	rm -rf ${STAGING_KERNEL_DIR}/.config
	cd -
}

do_deploy:append() {
	install -m 0644 ${D}/${KERNEL_IMAGEDEST}/${BOOT_IMAGE} ${DEPLOYDIR}/

	cd ${DEPLOYDIR}

    rm -f ${BOOT_IMAGE_BINARY} ${BOOT_IMAGE_SYMLINK}
    ln -sf ${BOOT_IMAGE} ${BOOT_IMAGE_SYMLINK}
    ln -sf ${BOOT_IMAGE} ${BOOT_IMAGE_BINARY}

	cd -
}

PACKAGE_KERNEL_BOOT_IMAGE = " \
	/${KERNEL_IMAGEDEST}/${BOOT_IMAGE} \
	/${KERNEL_IMAGEDEST}/${BOOT_IMAGE_BINARY} \
"
FILES:${KERNEL_PACKAGE_NAME}-image += " \
	${PACKAGE_KERNEL_BOOT_IMAGE} \
"

FILES:${KERNEL_PACKAGE_NAME}-modules += " \
	/lib/modules/${KERNEL_VERSION}/modules.builtin \
	/lib/modules/${KERNEL_VERSION}/modules.order \
"

# disable unneeded tasks
do_uboot_mkimage[noexec] = "1"
