# This Makefile provides 2 example targets, they both print argv, but
# one uses a libutils data structure to do it. Both binaries are
# built using size-optimizing flags and options for stripping dead code.
# On my system the binaries are 9KB (without libutils) and 11KB (with libutils).

# In my testing, trying to include individual files from libutils
# is actually worse than linking to libutils.la and letting the compiler/linker
# optimize for size and strip dead code.

# Run make inside libutils as well:
SUBDIRS = libntech libpolicy cf-policy tests/unit

# Look for macros inside m4 directory:
ACLOCAL_AMFLAGS = -I m4 --install
