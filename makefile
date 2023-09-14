LIBOMP_PATH := $(shell find /usr/lib/llvm-* -name "libomp.so" | sed 's/libomp.so//')

build-x86_64:
	cp $(LIBOMP_PATH)/libomp.so ./build/
	g++ ./src/goldilocks/* ./src/cwrapper/* -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -mavx2 -std=c++17 -L./build
	g++ --shared -o ./build/libvectorizedposeidongold.x86_64.so ./*.o
	rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

