UFS_MKIMGCMD = "${@oe.utils.conditional('BOOT_STORAGE', 'ufs', '--sector_size 4096', '', d)}"
MAKE_PLIST ?= "make_plist_${TCC_ARCH_FAMILY}"

BOOT_IMAGE_NAME = "${@bb.utils.contains('IMAGE_FEATURES', 'uboot-fit', 'fitImage', 'tc-boot-${MACHINE}.img$1', d)}"
TEEFS_IMAGE_NAME ?= ""

do_cleanfwdn() {
	rm -rf ${DEPLOY_DIR}/fwdn
}

do_make_fai() {
	install -d ${DEPLOY_DIR}/fwdn
	dtb=`echo ${KERNEL_DEVICETREE} | cut -d ' ' -f1`
	dtb_ext=${dtb##*.}
	dtb_name=`basename $dtb .$dtb_ext`

	system_name=`ls -lh --block-size=M ${DEPLOY_DIR}/images/${MACHINE}/${IMAGE_LINK_NAME}.${DEFAULT_IMAGE_FSTYPE}`
	for i in ${system_name}; do true; done
	system_size=`stat -c%s ${DEPLOY_DIR}/images/${MACHINE}/${i}`
	system_size=`expr ${system_size} / 1024 / 1024  + ${system_size} / 1024 / 1024 / 10`

	rm -f ${DEPLOY_DIR}/fwdn/partition.list*

	if ${@bb.utils.contains('TCC_BSP_FEATURES', 'with-subcore', 'true', 'false', d)}; then
		if [ -L ${SUBCORE_DEPLOY_DIR}/${SUBCORE_ROOTFS_IMAGE_NAME} ]; then
			subcore_item_real_name=`readlink ${SUBCORE_DEPLOY_DIR}/${SUBCORE_ROOTFS_IMAGE_NAME}`
		else
			subcore_item_real_name=${SUBCORE_ROOTFS_IMAGE_NAME}
		fi
		for i in ${subcore_item_real_name}; do true; done
		subcore_item_size=`stat -c%s ${SUBCORE_DEPLOY_DIR}/${i}`
		if [ ${TCC_ARCH_FAMILY} == "tcc805x" ]; then
			subcore_item_size=`expr 10 + ${subcore_item_size} / 1024 / 1024 + ${subcore_item_size} / 1024 / 1024 / 10`
		else
			subcore_item_size=`expr 1 + ${subcore_item_size} / 1024 / 1024 + ${subcore_item_size} / 1024 / 1024 / 10`
		fi
	fi

#create partition list
	touch ${DEPLOY_DIR}/fwdn/partition.list
	${MAKE_PLIST}

	if ${@oe.utils.conditional('TEEFS_IMAGE_NAME', '', 'false', 'true', d)}; then
		sest_size=`stat -c%s ${DEPLOY_DIR}/images/${MACHINE}/${TEEFS_IMAGE_NAME}`
		sest_size=`expr ${sest_size} / 1024 / 1024`
		echo "sest:${sest_size}M@${DEPLOY_DIR}/images/${MACHINE}/${TEEFS_IMAGE_NAME}" >> ${DEPLOY_DIR}/fwdn/partition.list
	fi

	add_additional_partitions "${DEPLOY_DIR}/fwdn/partition.list"

#symbolic link boot-firmware
	if [ ! -e ${DEPLOY_DIR}/fwdn/boot-firmware ]; then
		ln -sf ${DEPLOY_DIR_IMAGE}/boot-firmware ${DEPLOY_DIR}/fwdn/boot-firmware
	fi

#make SD_Data.fai
	${STAGING_BINDIR_NATIVE}/mktcimg \
		--parttype gpt \
		--storage_size ${STORAGE_SIZE} \
		--fplist ${DEPLOY_DIR}/fwdn/partition.list \
		--outfile ${DEPLOY_DIR}/fwdn/SD_Data.fai \
		--area_name "SD Data" \
		--gptfile ${DEPLOY_DIR}/fwdn/SD_Data.gpt ${UFS_MKIMGCMD}
}

make_plist_tcc807x() {
	echo "bl3_ap0_a:2M@${DEPLOY_DIR}/images/${MACHINE}/ap0_bl3.rom$1"									>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "bl3_ap0_b:2M@${DEPLOY_DIR}/images/${MACHINE}/ap0_bl3.rom$1"									>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "boot:40M@${DEPLOY_DIR}/images/${MACHINE}/${BOOT_IMAGE_NAME}"									>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "system:${SYSTEM_PARTITION_SIZE}M@${DEPLOY_DIR}/images/${MACHINE}/${IMAGE_LINK_NAME}.${DEFAULT_IMAGE_FSTYPE}"	>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "dtb:1024K@${DEPLOY_DIR}/images/${MACHINE}/$dtb_name.$dtb_ext"									>> ${DEPLOY_DIR}/fwdn/partition.list$1

	echo "misc:1M@"																						>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "splash:40M@"																					>> ${DEPLOY_DIR}/fwdn/partition.list$1

	if ${@bb.utils.contains('TCC_BSP_FEATURES', 'with-subcore', 'true', 'false', d)}; then
		echo "bl3_ap1_a:2M@${SUBCORE_DEPLOY_DIR}/ap1_bl3.rom$1"											>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "bl3_ap1_b:2M@${SUBCORE_DEPLOY_DIR}/ap1_bl3.rom$1"											>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "subcore_boot:40M@${SUBCORE_DEPLOY_DIR}/${SUBCORE_BOOT_IMAGE_NAME}$1"						>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "subcore_dtb:1024K@${SUBCORE_DEPLOY_DIR}/${SUBCORE_DTB_IMAGE_NAME}"						>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "subcore_root:${subcore_item_size}M@${SUBCORE_DEPLOY_DIR}/${SUBCORE_ROOTFS_IMAGE_NAME}"	>> ${DEPLOY_DIR}/fwdn/partition.list$1

		if [ -e ${SUBCORE_DEPLOY_DIR}/${SPLASH_IMAGE} ]; then
			echo "subcore_splash:40M@"																	>> ${DEPLOY_DIR}/fwdn/partition.list$1
		fi
		echo "subcore_misc:1M@"																				>> ${DEPLOY_DIR}/fwdn/partition.list$1
		add_additional_subcore_partitions "${DEPLOY_DIR}/fwdn/partition.list$1"
	fi
}

make_plist_tcc805x() {
	echo "bl3_ca72_a:2M@${DEPLOY_DIR}/images/${MACHINE}/ca72_bl3.rom$1"									>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "bl3_ca72_b:2M@${DEPLOY_DIR}/images/${MACHINE}/ca72_bl3.rom$1"									>> ${DEPLOY_DIR}/fwdn/partition.list$1

	echo "boot:30M@${DEPLOY_DIR}/images/${MACHINE}/${BOOT_IMAGE_NAME}"									>> ${DEPLOY_DIR}/fwdn/partition.list$1
#echo "system:${system_size}M@${DEPLOY_DIR}/images/${MACHINE}/${IMAGE_LINK_NAME}.${DEFAULT_IMAGE_FSTYPE}"	>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "system:${SYSTEM_PARTITION_SIZE}M@${DEPLOY_DIR}/images/${MACHINE}/${IMAGE_LINK_NAME}.${DEFAULT_IMAGE_FSTYPE}"	>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "dtb:200K@${DEPLOY_DIR}/images/${MACHINE}/$dtb_name.$dtb_ext"									>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "misc:1M@"																						>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "splash:40M@"																					>> ${DEPLOY_DIR}/fwdn/partition.list$1

	if ${@bb.utils.contains('TCC_BSP_FEATURES', 'with-subcore', 'true', 'false', d)}; then
		echo "bl3_ca53_a:2M@${SUBCORE_DEPLOY_DIR}/ca53_bl3.rom$1"											>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "bl3_ca53_b:2M@${SUBCORE_DEPLOY_DIR}/ca53_bl3.rom$1"											>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "subcore_boot:30M@${SUBCORE_DEPLOY_DIR}/${SUBCORE_BOOT_IMAGE_NAME}$1"						>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "subcore_dtb:200K@${SUBCORE_DEPLOY_DIR}/${SUBCORE_DTB_IMAGE_NAME}"							>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "subcore_root:${subcore_item_size}M@${SUBCORE_DEPLOY_DIR}/${SUBCORE_ROOTFS_IMAGE_NAME}"	>> ${DEPLOY_DIR}/fwdn/partition.list$1

		if [ -e ${SUBCORE_DEPLOY_DIR}/${SPLASH_IMAGE} ]; then
			echo "subcore_splash:40M@${SUBCORE_DEPLOY_DIR}/${SPLASH_IMAGE}"								>> ${DEPLOY_DIR}/fwdn/partition.list$1
		fi

		echo "subcore_misc:1M@"																			>> ${DEPLOY_DIR}/fwdn/partition.list$1
		add_additional_subcore_partitions "${DEPLOY_DIR}/fwdn/partition.list$1"
	fi
}
make_plist_tcc750x() {
	echo "bl3_ca53_a:2M@${DEPLOY_DIR}/images/${MACHINE}/u-boot.rom$1"								>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "bl3_ca53_b:2M@${DEPLOY_DIR}/images/${MACHINE}/u-boot.rom$1"								>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "boot:40M@${DEPLOY_DIR}/images/${MACHINE}/${BOOT_IMAGE_NAME}"							>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "system:${SYSTEM_PARTITION_SIZE}M@${DEPLOY_DIR}/images/${MACHINE}/${IMAGE_LINK_NAME}.${DEFAULT_IMAGE_FSTYPE}"	>> ${DEPLOY_DIR}/fwdn/partition.list$1
		echo "dtb:200K@${DEPLOY_DIR}/images/${MACHINE}/$dtb_name.$dtb_ext"							>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "env:1M@"																					>> ${DEPLOY_DIR}/fwdn/partition.list$1
	echo "misc:1M@"																					>> ${DEPLOY_DIR}/fwdn/partition.list$1
}

add_additional_partitions() {
	echo "No exist additional partition"
}

add_additional_subcore_partitions() {
	echo "No exist additional partition"
}

do_clean[depends] += "${@oe.utils.less_or_equal('FWDN_VERSION', '7', '', 'boot-firmware:do_clean', d)}"
do_make_fai[depends] += "${@oe.utils.less_or_equal('FWDN_VERSION', '7', '', 'tc-fai-generator-native:do_populate_sysroot boot-firmware:do_deploy', d)}"
addtask cleanfwdn after do_clean before do_cleansstate
addtask make_fai after do_image_complete before do_rm_work
