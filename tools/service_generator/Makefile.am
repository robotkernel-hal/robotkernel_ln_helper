INSTALL_HEADER = $(INSTALL_DATA) -C
ACLOCAL_AMFLAGS = -I m4

LDADD = @YAML_CPP_LIBS@ $(top_builddir)/src/.libs/libln_helper.la

#include_HEADERS = ln_md_helper.h

bin_PROGRAMS = service_generate
service_generate_SOURCES = generator.cpp 
service_generate_CXXFLAGS = -I$(top_srcdir)/include @YAML_CPP_CFLAGS@

