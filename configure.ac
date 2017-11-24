# ================= initialization =================== #

AC_INIT([MATE Sensors Applet], [1.19.1], [http://www.mate-desktop.org],
[mate-sensors-applet])

AC_CONFIG_SRCDIR([sensors-applet/main.c])
AC_CONFIG_HEADERS([sensors-applet/config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([dist-xz no-dist-gzip check-news])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE

# ============== basic compiler settings ============= #
AC_PROG_CC
AC_HEADER_STDC

# =================== for plugins ==================== #
AC_PROG_LIBTOOL

# =========== take care of some localization ========= #
AH_TEMPLATE([GETTEXT_PACKAGE], [Package name for gettext])
GETTEXT_PACKAGE=mate-sensors-applet
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE")
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.50.1])

# ========== export compiler / linker options ======== #
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)

# ============== look for dependencies =============== #
# check for headers needed for standard interfaces
AC_CHECK_HEADERS(
	stdlib.h \
	string.h \ 
	stdio.h \
	sys/types.h \ 
	sys/socket.h \
	netinet/in.h \
	arpa/inet.h \
	fcntl.h \
	sys/ioctl.h \
	unistd.h \
	dlfcn.h
)   

GLIB_REQUIRED=2.36.0
GTK_REQUIRED=3.14.0
LIBPANEL_REQUIRED=1.17.0
LIBNOTIFY_REQUIRED=0.7.0
LIBCAIRO_REQUIRED=1.0.4
LIBDBUSGLIB_REQUIRED=0.80
LIBATASMART_REQUIRED=0.16

PKG_CHECK_MODULES(GLIB, glib-2.0 >= $GLIB_REQUIRED gio-2.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GTK, gtk+-3.0 >= $GTK_REQUIRED)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(MATE, libmatepanelapplet-4.0 >= $LIBPANEL_REQUIRED)
AC_SUBST(MATE_CFLAGS)
AC_SUBST(MATE_LIBS)

PKG_CHECK_MODULES(CAIRO, cairo >= $LIBCAIRO_REQUIRED)
AC_SUBST(CAIRO_CFLAGS)
AC_SUBST(CAIRO_LIBS)

# for enum generation
PKG_PROG_PKG_CONFIG([0.28])
PKG_CHECK_VAR([GLIB_MKENUMS], [glib-2.0], [glib_mkenums])

# enable support for udisks (via dbus) by default if its available
AC_ARG_ENABLE(udisks,
	      AS_HELP_STRING([--disable-udisks],
			     [Disable support for udisks sensor readings via DBUS.]),
			     enable_udisks=$enableval, enable_udisks="yes")

if test "x$enable_udisks" = "xno" ; then
	echo "Disabling udisks support"
else
	PKG_CHECK_MODULES(DBUSGLIB, dbus-glib-1 >= $LIBDBUSGLIB_REQUIRED,
		          HAVE_DBUSGLIB="yes", HAVE_DBUSGLIB="no")
	AC_SUBST(DBUSGLIB_CFLAGS)
	AC_SUBST(DBUSGLIB_LIBS)

	PKG_CHECK_MODULES(LIBATASMART, libatasmart >= $LIBATASMART_REQUIRED,
			  HAVE_LIBATASMART="yes", HAVE_LIBATASMART="no")
	AC_SUBST(LIBATASMART_CFLAGS)
	AC_SUBST(LIBATASMART_LIBS)
fi

AM_CONDITIONAL(UDISKS, test -n "$DBUSGLIB_LIBS" -a -n "$LIBATASMART_LIBS")

# enable support for udisks2 (via dbus) by default if its available
AC_ARG_ENABLE(udisks2,
	      AS_HELP_STRING([--disable-udisks2],
			     [Disable support for udisks2 sensor readings via DBUS.]),
			     enable_udisks2=$enableval, enable_udisks2="yes")

if test "x$enable_udisks2" = "xno" ; then
	echo "Disabling udisks2 support"
#else
#	PKG_CHECK_MODULES(DBUSGLIB, dbus-glib-1 >= $LIBDBUSGLIB_REQUIRED,
#		          HAVE_DBUSGLIB="yes", HAVE_DBUSGLIB="no")
#	AC_SUBST(DBUSGLIB_CFLAGS)
#	AC_SUBST(DBUSGLIB_LIBS)

#	PKG_CHECK_MODULES(LIBATASMART, libatasmart >= $LIBATASMART_REQUIRED,
#			  HAVE_LIBATASMART="yes", HAVE_LIBATASMART="no")
#	AC_SUBST(LIBATASMART_CFLAGS)
#	AC_SUBST(LIBATASMART_LIBS)
fi

# use same test as above, bc AM_CONDITIONAL may not be in if/else
AM_CONDITIONAL(UDISKS2, test "x$enable_udisks2" = "xyes")

# support for libnotify
LIBNOTIFY_CFLAGS=
LIBNOTIFY_LIBS=

# enable support even if not specifically asked for
AC_ARG_ENABLE(libnotify, 
[  --enable-libnotify    Enable libnotify support], [
         enable_libnotify=$enableval], [
         enable_libnotify="yes"
])

if test "x$enable_libnotify" = "xno"
then
   echo "Disabling libnotify support"
else
	PKG_CHECK_MODULES(LIBNOTIFY, libnotify >= $LIBNOTIFY_REQUIRED,
	HAVE_LIBNOTIFY="yes", HAVE_LIBNOTIFY="no")

	if test "x$HAVE_LIBNOTIFY" = "xyes"
	then
	   AC_DEFINE(HAVE_LIBNOTIFY, 1, [libnotify available])
	fi	
fi

AM_CONDITIONAL(LIBNOTIFY, test -n "$LIBNOTIFY_LIBS")

AC_SUBST(LIBNOTIFY_CFLAGS)
AC_SUBST(LIBNOTIFY_LIBS)


# allow user to specify libsensors support, but support it anyway if possible
AC_ARG_WITH(libsensors,
[  --with-libsensors[=DIR]   use libsensors in DIR],[
        if test "$withval" != "no" -a "$withval" != "yes"; then
                SENSORS_DIR=$withval
                CPPFLAGS="${CPPFLAGS} -I${SENSORS_DIR}/include/sensors"
                LIBS="${LIBS} -L${SENSORS_DIR}/lib"
        fi
	with_libsensors=$withval],[
	with_libsensors="yes"
])

if test "$with_libsensors" = "no"; then
        echo "Disabling lmsensors support"
else
	# check for specfic headers needed for libsensors-sensors-interface
	AC_CHECK_HEADERS(regex.h)
        AC_CHECK_HEADERS(sensors/sensors.h,
                AC_CHECK_LIB(sensors, sensors_init,[
                        AC_DEFINE(HAVE_LIBSENSORS,1,[libsensors is available])
                        if test "x${SENSORS_DIR}" != "x"; then
                                LIBSENSORS_CFLAGS="-I${SENSORS_DIR}/include"
                                LIBSENSORS_LIBS="-L${SENSORS_DIR}/lib -lsensors"
                                [case ${host} in
                                        *-*-solaris*)
                                                SENSORS_LIBS="-L${SENSORS_DIR}/lib -R${SENSORS_DIR}/lib -lsensors"
                                                ;;
                                esac]
                        else
                                LIBSENSORS_LIBS="-lsensors"
                        fi]))
fi			

AM_CONDITIONAL(LIBSENSORS, test -n "$LIBSENSORS_LIBS")

AC_SUBST(LIBSENSORS_CFLAGS)
AC_SUBST(LIBSENSORS_LIBS)

# do nvidia by default if available
AC_ARG_WITH(nvidia,
[  --with-nvidia[=DIR]       use nvidia in DIR],[
        if test "$withval" != "no" -a "$withval" != "yes"; then
                NVIDIA_DIR=$withval
                CFLAGS="${CFLAGS} -I${NVIDIA_DIR}/include"
                LIBS="${LIBS} -L${NVIDIA_DIR}/lib"
        fi
	with_nvidia=$withval],[
	with_nvidia="yes"
])

if test "$with_nvidia" = "no"; then
        echo "Disabling nvidia support"
else
	AC_ARG_WITH(x11,
		[  --with-x11[=DIR]          use x11 in DIR],[
        	if test "$withval" != "no" -a "$withval" != "yes"; then
                   	X11_DIR=$withval
		   	CFLAGS="${CFLAGS} -I${X11_DIR}/include"
			LIBS="${LIBS} -L${X11_DIR}/lib"
        	fi
		with_x11=$withval],[
		with_x11="yes"
	])

	AC_CHECK_HEADERS(X11/Xlib.h, 
                AC_CHECK_LIB(X11, XOpenDisplay, [ HAVE_X11=true ]))
	# can't embed next AC_CHECK_HEADERS in true case of AC_CHECK_LIB
	if test "${HAVE_X11}" = "true"; then
	        LIBS="${LIBS} -lX11 -lXext"
		# need to specifically include X11/Xlib header when
		# doing checks for NVCtrl headers
	        AC_CHECK_HEADERS(NVCtrl/NVCtrl.h NVCtrl/NVCtrlLib.h, [ HAVE_NVIDIA=true ], [], 
	        [
	        #include <X11/Xlib.h>
         	])
	        if test "${HAVE_NVIDIA}" = "true"; then
	                AC_CHECK_LIB(XNVCtrl, XNVCTRLQueryExtension, [ 
                        NVIDIA_LIBS="-lXNVCtrl"
	                AC_DEFINE(HAVE_NVIDIA,1,[nvidia sensors available])
	                ])			
	        fi
	fi
fi
AM_CONDITIONAL(NVIDIA, test -n "$NVIDIA_LIBS")

AC_SUBST(NVIDIA_CFLAGS)
AC_SUBST(NVIDIA_LIBS)

AC_ARG_WITH(aticonfig, 
[  --with-aticonfig[=PATH]   use aticonfig plugin with optional binary provided         
                          by PATH], [
	if test "$withval" != "no" -a "$withval" != "yes"; then
		aticonfig_exe=$withval
        else
		aticonfig_exe=aticonfig
        fi
        with_aticonfig=$withval ], [
	aticonfig_exe=aticonfig
	with_aticonfig="check"
])

if test "x$with_aticonfig" = "xcheck"; then
	AC_CHECK_PROG(ATICONFIG_EXE, ["$aticonfig_exe"], yes, no)
        if test "x$ATICONFIG_EXE" = "xno"; then
		with_aticonfig="no"
        fi
fi

AM_CONDITIONAL(ATICONFIG, test "$with_aticonfig" != "no")
if test "$with_aticonfig" = "no"; then
  echo "Disabling aticonfig support"
else
  AC_DEFINE(HAVE_ATICONFIG,1,[using aticonfig])
  AC_DEFINE_UNQUOTED(ATICONFIG_EXE, ["$aticonfig_exe"], [aticonfig executable])
fi

AC_SUBST(CFLAGS)
# for help docs stuff
AC_PATH_PROG(XSLTPROC, xsltproc, no)
if test x"$XSLTPROC" = xno; then
  AC_MSG_ERROR([xsltproc executable not found in your path - should be installed with libxslt])
fi

MATE_COMPILE_WARNINGS(maximum)

# ================ yelp-tools stuff ============= #
YELP_HELP_INIT

# ================ GSettings stuff ============= #
GLIB_GSETTINGS

# ==================== plugin stuff ================== #
PLUGIN_LIBTOOL_FLAGS="-module -avoid-version"
AC_SUBST(PLUGIN_LIBTOOL_FLAGS)

# ================= generate files =================== #
AC_CONFIG_FILES([
	Makefile
	org.mate.sensors-applet.gschema.xml
	org.mate.sensors-applet.sensor.gschema.xml
	sensors-applet/Makefile
	lib/Makefile
	plugins/Makefile
	plugins/acpi/Makefile
	plugins/udisks/Makefile
	plugins/udisks2/Makefile
	plugins/eee/Makefile
	plugins/hddtemp/Makefile
	plugins/i2c-proc/Makefile
	plugins/i2c-sys/Makefile
	plugins/i8k/Makefile
	plugins/ibm-acpi/Makefile
	plugins/libsensors/Makefile
	plugins/mbmon/Makefile
	plugins/nvidia/Makefile
	plugins/aticonfig/Makefile
	plugins/omnibook/Makefile
	plugins/pmu-sys/Makefile
	plugins/smu-sys/Makefile
	plugins/sonypi/Makefile
	pixmaps/Makefile
	po/Makefile.in
	help/Makefile
])
AC_OUTPUT
