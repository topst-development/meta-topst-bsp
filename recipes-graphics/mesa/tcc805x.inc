
SRC_URI:append = " \
	file://0001-Add-PVR-Gallium-driver.patch \
	file://0002-dri-Add-some-new-DRI-formats-and-fourccs.patch \
	file://0003-GL_EXT_sparse_texture-entry-points.patch \
	file://0004-Add-support-for-various-GLES-extensions.patch \
	file://0005-Add-EGL_IMG_cl_image-extension.patch \
	file://0006-egl-optimise-eglMakeCurrent-for-the-case-where-nothi.patch \
	file://0007-GL_EXT_shader_pixel_local_storage2-entry-points.patch \
	file://0008-GL_IMG_framebuffer_downsample-entry-points.patch \
	file://0009-GL_OVR_multiview-entry-points.patch \
	file://0010-Add-OVR_multiview_multisampled_render_to_texture.patch \
	file://0011-wayland-drm-install-wayland-drm.xml-to-the-configure.patch \
	file://0012-Enable-buffer-sharing-in-the-kms_swrast-driver.patch \
	file://0013-egl-wayland-add-support-for-RGB565-back-buffers.patch \
	file://0014-egl-automatically-call-eglReleaseThread-on-thread-te.patch \
	file://0015-egl-wayland-post-maximum-damage-when-blitting.patch \
	file://0016-egl-wayland-flush-the-drawable-before-blitting.patch \
	file://0017-dri-use-a-supported-API-in-driCreateNewContext.patch \
	file://0018-gbm-add-gbm_bo_blit.patch \
	file://0019-gbm-don-t-assert-if-DRI-context-creation-fails.patch \
	file://0020-egl-wayland-add-pbuffer-support.patch \
	file://0021-egl-eglBindAPI-workaround-for-dEQP-bug.patch \
	file://0022-GL_EXT_multi_draw_indirect-entry-points.patch \
	file://0023-dri-add-support-for-YUV-DRI-config.patch \
	file://0024-egl-add-support-for-EXT_yuv_surface.patch \
	file://0025-dri-add-missing-__DRI_IMAGE_COMPONENTS-define-for-EG.patch \
	file://0026-egl-wayland-expose-EXT_yuv_surface-support.patch \
	file://0027-gbm-add-some-new-GBM-formats.patch \
	file://0028-egl-add-null-platform.patch \
	file://0029-egl-add-support-for-EXT_image_gl_colorspace.patch \
	file://0030-meson-force-C-2011-for-thread_local.patch \
	file://0031-dri2-add-support-for-swap-intervals-other-than-1.patch \
	file://0032-null_platform-add-support-for-explicit-synchronisati.patch \
	file://0033-egl-null-add-support-for-DRM-image-format-modifiers.patch \
	file://0034-egl-query-the-supported-ES2-context-version.patch \
	file://0035-meson-allow-libGL-to-be-built-without-GLX.patch \
	file://0036-egl-wayland-process-non-resized-window-movement.patch \
	file://0037-Separate-EXT_framebuffer_object-from-ARB-version.patch \
	file://0038-egl-null-add-support-for-async-flip-with-front-buffe.patch \
	file://0039-gbm-add-pbuffer-support.patch \
	file://0040-egl-null-expose-EXT_yuv_surface-support.patch \
	file://0041-dri-preserve-the-original-FD-for-driver-use.patch \
	file://0042-egl-wayland-a-linear-buffer-is-not-needed-with-DRM-f.patch \
	file://0043-dri3-a-linear-buffer-is-not-needed-with-DRM-format-m.patch \
	file://0044-egl-drm-add-support-for-DRI_PRIME-GPU-selection.patch \
	file://0045-egl-null-add-support-for-DRI_PRIME-GPU-selection.patch \
	file://0046-egl-null-introduce-NULL_DRM_DISPLAY.patch \
	file://0047-vulkan-wsi-check-the-DRI3-and-Present-XCB-reply-poin.patch \
	file://0048-vulkan-wsi-Allow-host-visible-memory-to-be-requested.patch \
	file://0049-vulkan-wsi-make-the-display-FD-available.patch \
	file://0050-pvr-wsi-add-PowerVR-Vulkan-WSI-library.patch \
	file://0051-vulkan-wsi-Disable-use-of-VK_EXT_pci_bus_info.patch \
	file://0052-vulkan-wsi-default-to-force_bgra8_unorm_first-true.patch \
	file://0053-vulkan-wsi-enable-additional-formats-for-Display.patch \
	file://0054-mesa-partially-revert-pbuffer-attribute-removal.patch \
	file://0055-egl_dri2-set-pbuffer-config-attribs-to-0-for-non-pbu.patch \
	file://0056-GL_ARB_geometry_shader4-entry-points.patch \
	file://0057-egl-wayland-add-EGL_BUFFER_PRESERVED-support.patch \
	file://0058-glapi-restore-exec-dynamic.patch \
	file://0059-Revert-meson-check-mtls-if-has_exe_wrapper.patch \
"

LINKER_HASH_STYLE = "sysv"
PE = ""
EXTRA_OEMESON:remove = "-Dglx-read-only-text=true"

PACKAGECONFIG:append:class-target = " \
        ${@bb.utils.filter('DISTRO_FEATURES', 'wayland vulkan', d)} \
        ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'opengl egl gles gbm', '', d)} \
"

VULKAN_DRIVERS:append ="${@bb.utils.contains('DISTRO_FEATURES', 'vulkan', ',pvr', '', d)}"

PACKAGECONFIG[gles] = "-Dgles1=enabled -Dgles2=enabled"
PACKAGECONFIG[egl] = "-Degl=enabled ${DRI_PLATFORMS}"

# -Dgallium-pvr-alias option is required to indicate the name of the display driver in the kernel, so that Mesa associates the PVR driver with it.
PACKAGECONFIG[gallium] = "-Dgallium-drivers=pvr -Dgallium-pvr-alias=tccdrm"


DRI_PLATFORMS:append:class-target = " \
	${@bb.utils.contains('TCC_BSP_FEATURES', 'drm', \
		bb.utils.contains('DISTRO_FEATURES', 'wayland', \
			bb.utils.contains('DISTRO_FEATURES', 'x11', \
			'-Dplatforms=wayland,x11', \
			'-Dplatforms=wayland', d), \
		' -Dplatforms=drm', d), \
	' -Dplatforms=null', d)} \
"

RDEPENDS:libgles1-mesa += " \
        libgles1-telechips \
"
RDEPENDS:libgles2-mesa += " \
        libgles2-telechips \
"

WAYLAND_DRM = '${@bb.utils.contains("DISTRO_FEATURES", "wayland", "${datadir}/pkgconfig/wayland-drm.pc", "", d)}'
FILES:libegl-mesa-dev += " ${@bb.utils.contains('TCC_BSP_FEATURES', 'drm', '${WAYLAND_DRM}', '', d)} "
FILES:mesa-vulkan-drivers += " ${@bb.utils.contains('DISTRO_FEATURES', 'vulkan', ' ${libdir}/libpvr_mesa_wsi.so', '', d)} "
