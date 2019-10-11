noinst_LTLIBRARIES = libpolicy.la

# TODO remove the openssl dependency! It's only there for base64 encoding.
AM_CFLAGS   = $(CORE_CFLAGS)   $(PCRE_CFLAGS)   $(OPENSSL_CFLAGS)
AM_CPPFLAGS = $(CORE_CPPFLAGS) $(PCRE_CPPFLAGS) $(OPENSSL_CPPFLAGS)
AM_LDFLAGS  = $(CORE_LDFLAGS)  $(PCRE_LDFLAGS)  $(OPENSSL_LDFLAGS)

libpolicy_la_LIBADD = ../libntech/libutils/libutils.la
libpolicy_la_LIBS   = $(PCRE_LIBS) $(OPENSSL_LIBS)

AM_YFLAGS = -d

libpolicy_la_SOURCES = \
	cf_tokenizer.l \
	cf_parser.y cf_parser.h \
	parse_lib.c parse_lib.h

# Tell the c preprocessor (CPP) where to find headers:
libpolicy_la_CPPFLAGS = -I ../libntech/libutils/ -I ../libntech/libcompat/
