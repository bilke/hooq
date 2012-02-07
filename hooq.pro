include(common.pri)

TEMPLATE = subdirs
SUBDIRS += \
	hooq \
	hooqbackup \
	hooqcli \
	hooqInjector \
	injectedHooq \
	uilib \

# State dependencies to fix make -j

hooq.depends += \
	hooqInjector \
	uilib \

hooqcli.depends += \
	hooqInjector \

!macx {
	CONFIG(debug, debug|release) {
		SUBDIRS += google-breakpad
		hooq.depends += google-breakpad
	}
}
