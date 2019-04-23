#!/usr/bin/csh
mkdir InstaDam-1.0
mkdir -p InstaDam-1.0/app
mkdir -p InstaDam-1.0/Selector
mkdir -p InstaDam-1.0/quazip
make distclean
rm -rf CMakeFiles
rm -rf */CMakeFiles
rm -f Makefile*
rm -f */Makefile*
cp CMakeLists.txt InstaDam-1.0/.
cp moduleheader.h InstaDam-1.0/.
cp instadam.qdocconf InstaDam-1.0/.
cp app/*.cpp InstaDam-1.0/app/.
cp app/*.h InstaDam-1.0/app/.
cp app/*.ui InstaDam-1.0/app/.
cp quazip/* InstaDam-1.0/quazip/.
cp Selector/*.h InstaDam-1.0/Selector/.
cp Selector/*.cpp InstaDam-1.0/Selector/.
cp Selector/*.qdoc InstaDam-1.0/Selector/.
cp Selector/CMakeLists.txt InstaDam-1.0/Selector/.
cp -r doc InstaDam-1.0/.
cp INSTALL InstaDam-1.0/.
tar zcvf InstaDam-1.0.tar.gz InstaDam-1.0
rm -rf InstaDam-1.0
