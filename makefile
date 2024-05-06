
output_dir := ./bin
output_file := $(output_dir)/test
output_file_clang := $(output_file)_clang

source_dir := ./src
source_files := $(shell echo $(source_dir)/*.cpp)

header_dir := ./include
header_files := $(shell echo $(header_dir)/*.hpp)

test_dir := ./test
test_files := $(shell echo $(test_dir)/*.cpp)
test_header_files := $(shell echo $(test_dir)/*.hpp)

CPPFLAGS := -Iinclude

test: $(output_file)
	$(output_file)

$(output_file): $(source_files) $(header_files) $(test_files) $(test_header_files)
	g++ $(source_files) $(test_files) -o $(output_file) $(CPPFLAGS)

test-clang: $(output_file_clang)
	$(output_file_clang)

$(output_file_clang): $(source_files) $(header_files) $(test_files) $(test_header_files)
	clang++ $(source_files) $(test_files) -o $(output_file_clang) $(CPPFLAGS)

format:
	clang-format -i $(source_files) $(header_files) $(test_files) $(test_header_files)

clean:
	rm $(output_dir)/*

