APP_NAME = hash_table
VERSION = 0.0.2
DEPENDENCIES = dlinked_list/0.1.0
INSTALL_PATH = /opt
CFLAGS +=

STATIC_DEP = dlinked_list
SHARED_DEP =

LDFLAGS += ${STATIC_DEP:%=-l%} ${SHARED_DEP:%=-l%}
