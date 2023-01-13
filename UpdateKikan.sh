rm -rf "./include/Kikan"

cp -rd "./../Kikan/include/Kikan" "./include/Kikan"

cp "./../Kikan/cmake-build-release/src/libKikan.a" "./lib/libKikan.a"

cp -r "./../Kikan/shaders/." "./kikan_shaders/"
