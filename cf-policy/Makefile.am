# "Trick" to ensure m4 directory exists in source tarballs:
EXTRA_DIST = m4/NOTES

AM_YFLAGS = -d

bin_PROGRAMS = \
	cf-policy

# An example target using sequnce data structure from libutils:
cf_policy_SOURCES = \
	cf-policy.c

# Statically link to libutils:
cf_policy_LDADD = ../libpolicy/libpolicy.la
# Tell the c preprocessor (CPP) where to find headers:
cf_policy_CPPFLAGS = -I ../libpolicy/ -I ../libntech/libutils/ -I ../libntech/libcompat/
# Optimize for binary size and strip dead code:
cf_policy_CFLAGS = -Os -fdata-sections -ffunction-sections
cf_policy_LDFLAGS = $(STRIP_LDFLAGS) # Platform specific
