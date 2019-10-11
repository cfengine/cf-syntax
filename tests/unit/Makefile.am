AUTOMAKE_OPTIONS = subdir-objects

check_LTLIBRARIES = libcmocka.la
libcmocka_la_SOURCES = cmocka/src/cmocka.c
libcmocka_la_CPPFLAGS = -Icmocka/include

TESTS = \
	test_parse_lib

check_PROGRAMS = \
	$(TESTS)

test_parse_lib_SOURCES = test_parse_lib.c \
	../../cf-syntax/parse_lib.c \
	../../cf-syntax/cf_tokenizer.l \
	../../cf-syntax/cf_parser.y
test_parse_lib_LDADD = libcmocka.la $(top_srcdir)/libntech/libutils/libutils.la
test_parse_lib_CPPFLAGS = -I$(top_srcdir)/cf-syntax \
	-I$(top_srcdir)/libntech/libutils/ \
	-I$(top_srcdir)/libntech/libcompat/
