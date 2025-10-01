require ${BPN}.inc

SRC_URI = "${TELECHIPS_TOPST_GIT}/gpu.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRC_URI += "file://pkgconfig"

SRCREV = "fe9f3d28db9731a5107c992d1bae3e9e8f2cc18c"

PATCHTOOL = "git"
COMPATIBLE_MACHINE = "tcc805x"
