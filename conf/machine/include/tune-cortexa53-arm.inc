TUNEVALID[cortexa53-arm] = "Enable Cortex-A53 specific processor optimizations"
TUNE_CCARGS .= " -mcpu=cortex-a53"
TUNE_CCARGS .= " -mtune=cortex-a53"

TUNEVALID[crypto] = "Enable ARMv8 crypto extension."

TUNEVALID[fp-armv8] = "Enable ARMv8 Vector Floating Point unit."
TUNE_CCARGS_MFPU .= "${@bb.utils.contains('TUNE_FEATURES', 'fp-armv8', ' fp-armv8', '', d)}"
TUNE_CCARGS_MFPU .= "${@bb.utils.contains('TUNE_FEATURES', [ 'fp-armv8', 'neon' ], ' neon-fp-armv8', '', d)}"
TUNE_CCARGS_MFPU .= "${@bb.utils.contains('TUNE_FEATURES', [ 'fp-armv8', 'neon', 'crypto' ], ' crypto-neon-fp-armv8', '', d)}"

PACKAGE_EXTRA_ARCHS:tune-armv8a            = "armv8a"
PACKAGE_EXTRA_ARCHS:tune-armv8a-crc        = "${PACKAGE_EXTRA_ARCHS:tune-armv8a} armv8a-crc"
PACKAGE_EXTRA_ARCHS:tune-armv8a-crypto     = "${PACKAGE_EXTRA_ARCHS:tune-armv8a} armv8a-crypto"
PACKAGE_EXTRA_ARCHS:tune-armv8a-crc-crypto = "${PACKAGE_EXTRA_ARCHS:tune-armv8a-crc} armv8a-crypto armv8a-crc-crypto"

# Little Endian base configs
AVAILTUNES += "cortexa53-arm cortexa53-arm-crypto"
ARMPKGARCH:tune-cortexa53-arm					= "cortexa53"
ARMPKGARCH:tune-cortexa53-arm-crypto			= "cortexa53"
TUNE_FEATURES:tune-cortexa53-arm				= "arm neon fp-armv8 callconvention-hard cortexa53 crc"
TUNE_FEATURES:tune-cortexa53-arm-crypto			= "arm neon fp-armv8 callconvention-hard cortexa53 crc crypto"
PACKAGE_EXTRA_ARCHS:tune-cortexa53-arm             = "${PACKAGE_EXTRA_ARCHS:tune-armv8a-crc} cortexa53hf-neon-fp-armv8"
PACKAGE_EXTRA_ARCHS:tune-cortexa53-arm-crypto      = "${PACKAGE_EXTRA_ARCHS:tune-armv8a-crc-crypto} cortexa53hf-crypto-neon-fp-armv8"
BASE_LIB:tune-cortexa53-arm               = "lib"
BASE_LIB:tune-cortexa53-arm-crypto        = "lib"
