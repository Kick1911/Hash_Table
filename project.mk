APP_NAME = hash_table
VERSION = 0.1.2
INSTALL_PATH = /opt
CFLAGS +=

STATIC_DEP = dlinked_list
SHARED_DEP =

DEPENDENCIES = Kick1911/dlinked_list/v0.1.0/libdlinked_list.a

LDFLAGS += ${STATIC_DEP:%=-l%} ${SHARED_DEP:%=-l%}
