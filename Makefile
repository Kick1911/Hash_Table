include project.mk
include config.mk

all: dep ${APP_NAME}

${APP_NAME}: ${SRC_PATH}/${APP_NAME}.c ${COMP_O} ${UTILS_O}
	${call print,BIN $@}
	${Q}${CC} $^ -o $@ ${CFLAGS} ${LDFLAGS}

%.o: %.c
	${call print,CC $< -> $@}
	${Q}${CC} -c $< -o $@ ${CFLAGS}

static_library: lib${APP_NAME}.a.${VERSION}

lib${APP_NAME}.a.${VERSION}: ${COMP_O} ${UTILS_O}
	${call print,AR $@}
	${Q}cd ${LIB_PATH}; ar -x *.a.[0-9].[0-9].[0-9]
	${Q}ar -cvq $@ $^ ${shell find ${LIB_PATH} -name '*.o'}

set_pic:
	${eval CFLAGS += -fPIC}

shared_library: set_pic lib${APP_NAME}.so

lib${APP_NAME}.so: ${COMP_O} ${UTILS_O}
	${call print,LIB $@.${VERSION}}
	${Q}${CC} -shared -Wl,-soname,$@ -o $@.${VERSION} $^ ${LDFLAGS} ${shell find lib -name '*.o'}
	${call print,'SYMLINK $@'}
	${Q}ln -sf $@.${VERSION} $@

dep: ${DEPENDENCIES:%=${LIB_PATH}/%}

${LIB_PATH}/%:
	${eval LIB_NAME = ${notdir $@}}
	${eval PROJECT_NAME = ${shell echo ${LIB_NAME} | awk -v RS=' ' 'match($$0, "lib(.+).(a|so).[0-9].[0-9].[0-9]", a) {print a[1]}'}}
	${call download,${PROJECT_NAME},${LIB_NAME},${LIB_PATH}}
	${call download,${PROJECT_NAME},${PROJECT_NAME}.h,${INCLUDE_PATH}}

register_app:
	${call mkdir,${APP_NAME}}

upload_shared: set_pic lib${APP_NAME}.so
	${call upload,${APP_NAME},${filter %.so,$^}.${VERSION}}
	${call upload,${APP_NAME},${INCLUDE_PATH}/${APP_NAME}.h}

upload_static: lib${APP_NAME}.a.${VERSION}
	${call upload,${APP_NAME},$<}
	${call upload,${APP_NAME},${INCLUDE_PATH}/${APP_NAME}.h}

install:
	${call print,INSTALL ${INSTALL_PATH}}
	${Q}mkdir -p ${INSTALL_PATH}/{bin,share/${APP_NAME},include,lib}
	${Q}cp ${APP_NAME} ${INSTALL_PATH}/bin 2> /dev/null || :
	${Q}cp ${INCLUDE_PATH}/* ${INSTALL_PATH}/include 2> /dev/null || :
	${Q}cp lib${APP_NAME}.* ${INSTALL_PATH}/lib 2> /dev/null || :
	${Q}cp -R ${SHARE_PATH}/* ${INSTALL_PATH}/share/${APP_NAME} 2> /dev/null || :

clean:
	${call print,CLEAN ${APP_NAME}}
	${Q}${MAKE} -C tests clean
	${Q}${RM} ${APP_NAME} lib${APP_NAME}.* ${COMP_O} ${UTILS_O}

.PHONY: install clean all set_pic
