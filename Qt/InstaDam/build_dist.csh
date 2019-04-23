#!/usr/bin/csh
mkdir InstaDam
mkdir -p InstaDam/app
mkdir -p InstaDam/Selector
mkdir -p InstaDam/quazip
make distclean
rm -rf CMakeFiles
rm -rf */CMakeFiles
rm -f Makefile*
rm -f */Makefile*
cp CMakeLists.txt InstaDam/.
cp moduleheader.h InstaDam/.
cp instadam.qdocconf InstaDam/.
cp app/*.cpp InstaDam/app/.
cp app/*.h InstaDam/app/.
cp app/*.ui InstaDam/app/.
cp quazip/* InstaDam/quazip/.
cp Selector/*.h InstaDam/Selector/.
cp Selector/*.cpp InstaDam/Selector/.
cp Selector/*.qdoc InstaDam/Selector/.
cp Selector/CMakeLists.txt InstaDam/Selector/.

tar zcvf InstaDam-1.0.tar.gz InstaDam
