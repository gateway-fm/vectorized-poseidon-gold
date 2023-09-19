LIBOMP_SYM_PATH := $(shell find /usr/lib/llvm-* -name "libomp.so")
LIBOMP_ABSOLUTE_PATH := $(shell readlink -f ${LIBOMP_SYM_PATH})
LIBOMP_BASENAME := $(shell basename ${LIBOMP_ABSOLUTE_PATH})
# LIBOMP_PATH := $(${LIBOMP_SYM_PATH} | sed 's/libomp.so//')

build-x86_64:
	cp $(LIBOMP_ABSOLUTE_PATH) ./build/
	ln -s -f ${LIBOMP_BASENAME} ./build/libomp.so
	g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapper.c -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -std=c++17 -L./build
	g++ --shared -o ./build/libvectorizedposeidongold.x86_64.so ./*.o
	rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

build-x86_64-avx2:
	cp $(LIBOMP_ABSOLUTE_PATH) ./build/
	ln -s -f ${LIBOMP_BASENAME} ./build/libomp.so
	g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapperAvx2.c -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -mavx2 -std=c++17 -L./build -D __AVX2__
	g++ --shared -o ./build/libvectorizedposeidongold.x86_64.avx2.so ./*.o
	rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

build-x86_64-avx512:
	cp $(LIBOMP_ABSOLUTE_PATH) ./build/
	ln -s -f ${LIBOMP_BASENAME} ./build/libomp.so
	g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapperAvx512.c -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -mavx2 -mavx512f -std=c++17 -L./build -D __AVX512__
	g++ --shared -o ./build/libvectorizedposeidongold.x86_64.avx512.so ./*.o
	rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

build:
	go build -mod=readonly -o ./build/wrapper -ldflags="-s -w" ./src/gowrapper/

