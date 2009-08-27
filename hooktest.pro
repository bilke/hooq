TEMPLATE = subdirs
SUBDIRS += \
	hooq \
	hooqcli \
	hooqInjector \
	injectedHooq \

# State dependencies to fix make -j
hooq.depends = hooqInjector
hooqcli.depends = hooqInjector
