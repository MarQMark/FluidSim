@RD /S /Q "./include/Kikan"

cd ./lib
del libKikan.a
cd ..

mkdir "./include/Kikan"
ROBOCOPY "./../Kikan/include/Kikan" "./include/Kikan" /mir
copy ".\..\Kikan\cmake-build-debug\src\libKikan.a" ".\lib\libKikan.a"

ROBOCOPY "./../Kikan/shaders" "./kikan_shaders" /mir
