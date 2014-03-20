AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -lgssapi
lib_LTLIBRARIES = libgsspp.la
libgsspp_la_SOURCES = gssbuffer.cpp gsscontext.cpp gsscredential.cpp gssexception.cpp gssmech.cpp gssname.cpp
pkglib_HEADERS = $(top_srcdir)/include/gsspp/gssbuffer.h     \
                 $(top_srcdir)/include/gsspp/gsscontext.h    \
                 $(top_srcdir)/include/gsspp/gsscredential.h \
                 $(top_srcdir)/include/gsspp/gssexception.h  \
                 $(top_srcdir)/include/gsspp/gssmech.h       \
                 $(top_srcdir)/include/gsspp/gssname.h
libgsspp_la_LDFLAGS = -version-info 0:0:0

