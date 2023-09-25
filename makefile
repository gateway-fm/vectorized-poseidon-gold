LIBOMP_SYM_PATH := $(shell find /usr/lib/llvm-* -name "libomp.so" 2> /dev/null)
ifdef LIBOMP_SYM_PATH
	LIBOMP_ABSOLUTE_PATH := $(shell readlink -f ${LIBOMP_SYM_PATH})
	LIBOMP_BASENAME := $(shell basename ${LIBOMP_ABSOLUTE_PATH})
endif
# LIBOMP_PATH := $(${LIBOMP_SYM_PATH} | sed 's/libomp.so//')

build-linux_x86_64:
	@cp $(LIBOMP_ABSOLUTE_PATH) ./build/linux
	@cp $(LIBOMP_ABSOLUTE_PATH) ./build/linux/libomp.so
	@g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapper.c -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -std=c++17 -L./build/linux
	@g++ --shared -o ./build/linux/libvectorizedposeidongold.linux.x86_64.so ./*.o
	@rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

build-linux_x86_64-avx2:
	@cp $(LIBOMP_ABSOLUTE_PATH) ./build/linux
	@cp $(LIBOMP_ABSOLUTE_PATH) ./build/linux/libomp.so
	@g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapperAvx2.c -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -mavx2 -std=c++17 -L./build/linux -D __AVX2__
	@g++ --shared -o ./build/linux/libvectorizedposeidongold.linux.x86_64.avx2.so ./*.o
	@rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

build-linux_x86_64-avx512:
	@cp $(LIBOMP_ABSOLUTE_PATH) ./build/linux
	@cp $(LIBOMP_ABSOLUTE_PATH) ./build/linux/libomp.so
	@g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapperAvx512.c -lgmp -lomp -fPIC -g -c -Wall -pthread -fopenmp -O2 -mavx2 -mavx512f -std=c++17 -L./build/linux -D __AVX512__
	@g++ --shared -o ./build/linux/libvectorizedposeidongold.linux.x86_64.avx512.so ./*.o
	@rm -rf ./*.o && rm -rf ./src/goldilocks/*.gch && rm -rf ./src/cwrapper/*.gch

build-apple_x86_64:
	echo "Under development"
	# g++ ./src/goldilocks/* ./src/cwrapper/CWrapper.h ./src/cwrapper/CWrapper.c -lgmp -lomp -fPIC -g -c -Wall -pthread -O2 -std=c++17 -L./build/macos -L/usr/local/opt/libomp/lib -I/usr/local/opt/libomp/include -L/usr/local/Cellar/gmp/6.2.1_1/lib -I/usr/local/Cellar/gmp/6.2.1_1/include

build-vectorizedposeidongold-test:
	@go build -mod=readonly -o ./build/vectorizedposeidongold -ldflags="-s -w" ./cmd

