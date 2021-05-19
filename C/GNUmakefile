.PHONY: all clean

all: build
	ninja -C $<
	ninja -C $< test
	run-clang-tidy.py -p $< -checks='-readability-*,-cppcoreguidelines-init-variables'

build:
	cmake -G Ninja -B $@ -S .

clean:
	rm -rf build
	find . -name '*~' -delete
