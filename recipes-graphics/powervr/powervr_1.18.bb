require ${BPN}.inc

SRC_URI = "${TELECHIPS_TOPST_GIT}/gpu.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRC_URI += "file://pkgconfig"

SRCREV = "${AUTOREV}"

PATCHTOOL = "git"
COMPATIBLE_MACHINE = "tcc805x"
