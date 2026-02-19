includedir= $(prefix)/include/ln_helper

LDADD = @YAML_CPP_LIBS@

include_HEADERS = $(top_srcdir)/include/ln_helper/service.h \
				  $(top_srcdir)/include/ln_helper/field.h \
				  $(top_srcdir)/include/ln_helper/datatype.h \
				  $(top_srcdir)/include/ln_helper/helper.h \
				  $(top_srcdir)/include/ln_helper/util.h

lib_LTLIBRARIES = libln_helper.la
libln_helper_la_SOURCES = fs_tools.cpp service.cpp field.cpp datatype.cpp helper.cpp
libln_helper_la_CXXFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/src/ @YAML_CPP_CFLAGS@

