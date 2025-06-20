require ${BPN}.inc

SRC_URI = "${TELECHIPS_TOPST_GIT}/3dmali.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRCREV = "413f0a28740504282f30e2be87c25b0d681fc02e"

MALI_VER = "r${@d.getVar("PV").split(".")[1]}"

PATCHTOOL = "git"

COMPATIBLE_MACHINE = "(tcc897x|tcc803x|tcc807x)"
