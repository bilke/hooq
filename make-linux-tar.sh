#!/bin/sh
rm -rf hooq-dist
mkdir hooq-dist
for file in $(find -type f -executable); do
	install -m755 $file hooq-dist/$(basename $file)
done
(
	cd hooq-dist
	mkdir lib bin
	mv *so* lib
	mv * bin
	mv bin/lib lib
	cd lib
	# make symlinks
	/sbin/ldconfig -n .
)
tar jcfv hooq-dist.tar.bz2 hooq-dist
