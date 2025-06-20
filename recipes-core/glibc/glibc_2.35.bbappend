PROVIDES += "virtual/crypt"

EXTRA_OECONF:remove = "--disable-crypt"

FILES:${PN} += "${base_libdir}/libcrypt-*.so ${base_libdir}/libcrypt*.so.*"
