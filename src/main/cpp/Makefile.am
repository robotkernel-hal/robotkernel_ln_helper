INSTALL_HEADER = $(INSTALL_DATA) -C
ACLOCAL_AMFLAGS = -I m4

LDADD = @YAML_CPP_LIBS@

include_HEADERS = ln_md_helper.h

bin_PROGRAMS = bridge_ln_generator
bridge_ln_generator_SOURCES = generator.cpp fs_tools.cpp
bridge_ln_generator_CXXFLAGS = -I$(top_srcdir) -I$(top_builddir)/src/main/cpp/ @YAML_CPP_CFLAGS@

