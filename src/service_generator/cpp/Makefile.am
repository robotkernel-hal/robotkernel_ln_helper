INSTALL_HEADER = $(INSTALL_DATA) -C
ACLOCAL_AMFLAGS = -I m4

LDADD = @LIBSTRING_UTIL_LIBS@ @YAML_CPP_LIBS@

include_HEADERS = ln_md_helper.h

bin_PROGRAMS = service_generator
service_generator_SOURCES = generator.cpp fs_tools.cpp
service_generator_CXXFLAGS = -I$(top_srcdir) -I$(top_builddir)/src/main/cpp/ @LIBSTRING_UTIL_CFLAGS@ @YAML_CPP_CFLAGS@

