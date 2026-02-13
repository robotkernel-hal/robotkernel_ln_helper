INSTALL_HEADER = $(INSTALL_DATA) -C
ACLOCAL_AMFLAGS = -I m4

LDADD = @YAML_CPP_LIBS@

include_HEADERS = ln_md_helper.h

bin_PROGRAMS = service_generator
service_generator_SOURCES = generator.cpp fs_tools.cpp service.cpp field.cpp datatype.cpp helper.cpp
service_generator_CXXFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/src/main/cpp/ @YAML_CPP_CFLAGS@

