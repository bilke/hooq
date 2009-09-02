TEMPLATE = subdirs
SUBDIRS += \
	hooq \
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

