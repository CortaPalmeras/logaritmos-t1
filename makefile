
t1 := tarea
cp := ciaccia_patella
ss := sexton_swinbank
mm := minmax
bu := busqueda

output_dir := ./bin
cp_bin := $(output_dir)/$(cp)
ss_bin := $(output_dir)/$(ss)
mm_bin := $(output_dir)/$(mm)
bu_bin := $(output_dir)/$(bu)

source_dir := ./src
t1_src := $(source_dir)/$(t1).cpp
cp_src := $(source_dir)/$(cp).cpp
ss_src := $(source_dir)/$(ss).cpp
mm_src := $(source_dir)/$(mm).cpp
bu_src := $(source_dir)/$(bu).cpp

header_dir := ./include
t1_head := $(header_dir)/$(t1).hpp
cp_head := $(header_dir)/$(cp).hpp
ss_head := $(header_dir)/$(ss).hpp
mm_head := $(header_dir)/$(mm).hpp
bu_head := $(header_dir)/$(bu).hpp

test_dir := ./test
t1_test := $(test_dir)/$(t1)_test.cpp
cp_test := $(test_dir)/$(cp)_test.cpp
ss_test := $(test_dir)/$(ss)_test.cpp
mm_test := $(test_dir)/$(mm)_test.cpp
bu_test := $(test_dir)/$(bu)_test.cpp

test_head := $(test_dir)/$(t1)_test.hpp

t1_files := $(t1_test) $(t1_src)
cp_files := $(t1_files) $(cp_test) $(cp_src) 
mm_files := $(t1_files) $(mm_test) $(mm_src) 
ss_files := $(t1_files) $(ss_test) $(ss_src) $(mm_src)
bu_files := $(t1_files) $(bu_test) $(bu_src) $(cp_src) $(ss_src) $(mm_src)

t1_depends := $(t1_head) $(test_head)
cp_depends := $(t1_depends) $(cp_files) $(cp_head)
ss_depends := $(t1_depends) $(ss_files) $(ss_head)
mm_depends := $(t1_depends) $(mm_files) $(mm_head)
bu_depends := $(t1_depends) $(bu_files) $(bu_head)

CPPFLAGS := -Iinclude -Wall -Wextra -Werror

ifdef debug
	CPPFLAGS += -g
else ifdef san
	CPPFLAGS += -fsanitize=address
else
	CPPFLAGS += -O3
endif

# Crear binarios para el metodo de Ciaccia-Patella
$(cp): $(cp_bin)
	$(cp_bin)

$(cp_bin): $(cp_depends)
	$(CXX) $(CPPFLAGS) $(cp_files) -o $(cp_bin)

# Crear binarios para el metodo de Sexton-Swinbank
$(ss): $(ss_bin)
	$(ss_bin)

$(ss_bin): $(ss_depends)
	$(CXX) $(CPPFLAGS) $(ss_files) -o $(ss_bin)

# Crear binarios para la función min-max split

format:
	clang-format -i $(source_dir)/* $(header_dir)/* $(test_dir)/*

clean:
	rm $(output_dir)/*

