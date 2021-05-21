.PHONY: all clean

all: ../build
	$(MAKE) -C Util/Lzma
	ninja -C $<
	ninja -C $< lcov

check: ../build
	run-clang-tidy.py -p $< -checks='-readability-*,-cppcoreguidelines-init-variables,-cert-dcl37-c,-cert-dcl51-cpp'

../build:
	cmake -G Ninja -B $@ -S . -DCMAKE_BUILD_TYPE=Coverage

clean:
	$(MAKE) -C Util/Lzma $@
	cmake -E rm -rf ../build build tags
	find . -name '*~' -delete
