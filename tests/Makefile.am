AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -lgssglue 

TESTS = check_gssbuffer check_gssname check_gssmech check_gsscredential
check_PROGRAMS = check_gssbuffer check_gssname check_gssmech check_gsscredential
check_gssbuffer_SOURCES = check_gssbuffer.cpp $(top_builddir)/include/gsspp/gssbuffer.h
check_gssbuffer_LDADD = $(top_builddir)/src/.libs/libgsspp.la

check_gssname_SOURCES = check_gssname.cpp $(top_builddir)/include/gsspp/gssname.h $(top_builddir)/include/gsspp/gssbuffer.h
check_gssname_LDADD = $(top_builddir)/src/.libs/libgsspp.la

check_gssmech_SOURCES = check_gssmech.cpp $(top_srcdir)/include/gsspp/gssmech.h $(top_srcdir)/include/gsspp/gssbuffer.h $(top_srcdir)/include/gsspp/gssexception.h
check_gssmech_LDADD = $(top_builddir)/src/.libs/libgsspp.la

check_gsscredential_SOURCES = check_gsscredential.cpp $(top_srcdir)/include/gsspp/gsscredential.h
check_gsscredential_LDADD = $(top_builddir)/src/.libs/libgsspp.la

