

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}/${TCC_ARCH_FAMILY}:"

inherit dos2unix

include ${TCC_ARCH_FAMILY}.inc

