TESTS = check_gssbuffer check_gssname
check_PROGRAMS = check_gssbuffer check_gssname
check_gssbuffer_SOURCES = check_gssbuffer.cpp $(top_builddir)/include/gsspp/gssbuffer.h
check_gssbuffer_CFLAGS = @CHECK_FLAGS@
check_gssbuffer_LDADD = $(top_builddir)/src/.libs/libmoney.la @CHECK_LIBS@

check_gssname_SOURCES = check_gssname.cpp $(top_builddir)/include/gsspp/gssname.h $(top_builddir)/include/gsspp/gssbuffer.h
check_gssname_CFLAGS = @CHECK_FLAGS@
check_gssbname_LDADD = $(top_builddir)/src/.libs/libmoney.la @CHECK_LIBS@

