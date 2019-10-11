# "Trick" to ensure m4 directory exists in source tarballs:
EXTRA_DIST = m4/NOTES

AM_YFLAGS = -d

bin_PROGRAMS = \
	cf-syntax

# An example target using sequnce data structure from libutils:
cf_syntax_SOURCES = \
	cf_tokenizer.l \
	cf_parser.y cf_parser.h \
	parse_lib.c parse_lib.h \
	cf-syntax.c

# Statically link to libutils:
cf_syntax_LDADD = ../libntech/libutils/.libs/libutils.la
# Tell the c preprocessor (CPP) where to find headers:
cf_syntax_CPPFLAGS = -I ../libntech/libutils/ -I ../libntech/libcompat/
# Optimize for binary size and strip dead code:
cf_syntax_CFLAGS = -Os -fdata-sections -ffunction-sections
cf_syntax_LDFLAGS = $(STRIP_LDFLAGS) # Platform specific
