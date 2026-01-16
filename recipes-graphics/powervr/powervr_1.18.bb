require ${BPN}.inc

SRC_URI = "${TELECHIPS_TOPST_GIT}/gpu.git;protocol=${TOPST_GIT_PROTOCOL};branch=${TOPST_BRANCH}"
SRC_URI += "file://pkgconfig"

SRCREV = "233fd2637dd5e7d3e16eb5d96826f2cccb45086d"

PATCHTOOL = "git"
COMPATIBLE_MACHINE = "tcc805x"
