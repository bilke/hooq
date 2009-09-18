#
# Regular cron jobs for the hooq package
#
0 4	* * *	root	[ -x /usr/bin/hooq_maintenance ] && /usr/bin/hooq_maintenance
