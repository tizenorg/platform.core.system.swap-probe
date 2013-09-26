CURDIR = `pwd`
INSTALLDIR = usr/lib

DUMMY_VERSION = 0.0.1

INC_COMMON = -I./include \
			 -I./probe_thread \
			 -I/usr/include/system \
			 -I/usr/include/appfw \
			 -I/usr/include/ecore-1 \
			 -I/usr/include/evas-1 \
			 -I/usr/include/eina-1 \
			 -I/usr/include/eina-1/eina \
			 -I/usr/include/elementary-1 \
			 -I/usr/include/eet-1 \
			 -I/usr/include/edje-1 \
			 -I/usr/include/efreet-1 \
			 -I/usr/include/ethumb-1 \
			 -I/usr/include/e_dbus-1 \
			 -I/usr/include/dbus-1.0 \
			 -I/usr/lib/dbus-1.0/include
INC_CAPI = $(INC_COMMON) #-I/usr/include/vconf -I/usr/include/pixman-1
INC_TIZEN = $(INC_COMMON) -I/usr/include/osp

COMMON_SRCS =	./helper/libdaprobe.c \
				./helper/dahelper.c \
				./helper/btsym.c \
				./helper/dacollection.c \
				./helper/dacapture.c \
				./probe_memory/libdamemalloc.c \
				./probe_memory/libdamemmanage.c \
				./custom_chart/da_chart.c \
				./probe_socket/libdasocket.c \
				./probe_event/da_event.c \
				./probe_event/keytouch.c \
				./probe_event/orientation.c \
				./probe_third/libdaemon.c \
				./probe_thread/libdathread.c \
				./probe_thread/libdasync.c \
				./probe_userfunc/libdauserfunc.c \
				./probe_file/da_io_posix.c \
				./probe_file/da_io_stdc.c

CAPI_SRCS =$(COMMON_SRCS) \
			./helper/addr-capi.c \
			./probe_capi/capi_appfw.c \
			./probe_ui/capi_capture.c

TIZEN_SRCS =	$(COMMON_SRCS) \
			./helper/addr-tizen.c \
			./probe_memory/libdanew.cpp \
			./probe_tizenapi/tizen_file.cpp \
			./probe_tizenapi/tizen_socket.cpp \
			./probe_tizenapi/tizen_http.cpp \
			./probe_tizenapi/tizen_thread.cpp \
			./probe_tizenapi/tizen_lifecycle.cpp \
			./probe_tizenapi/tizen_sync.cpp \
			./probe_tizenapi/tizen_controls.cpp \
			./probe_tizenapi/tizen_constructor.cpp \
			./probe_event/gesture.cpp \
			./probe_ui/tizen_capture.cpp \
			./probe_ui/tizen_scenemanager.cpp \
			./probe_ui/tizen_frameani.cpp \
			./probe_ui/tizen_display.cpp \
			./probe_graphics/da_gles20.cpp

DUMMY_SRCS = ./custom_chart/da_chart_dummy.c

CAPI_TARGET = da_probe_capi.so
TIZEN_TARGET = da_probe_tizen.so
DUMMY_TARGET = libdaprobe.so

COMMON_FLAGS = -D_GNU_SOURCE -fPIC -shared -Wall -funwind-tables -fomit-frame-pointer -Xlinker --no-undefined
CAPI_FLAGS = $(COMMON_FLAGS)
TIZEN_FLAGS = $(COMMON_FLAGS) -DTIZENAPP

LIBDIR_COMMON = 
LIBDIR_CAPI = $(LIBDIR_COMMON) 
LIBDIR_TIZEN = $(LIBDIR_COMMON) -L/usr/lib/osp

COMMON_LDFLAGS = -ldl -lpthread -lrt -lecore -levas -lecore_input -leina -lecore_x -lcapi-system-runtime-info -lcapi-appfw-application -lX11 -lXext
CAPI_LDFLAGS = $(COMMON_LDFLAGS)
TIZEN_LDFLAGS = $(COMMON_LDFLAGS) -lstdc++ -losp-uifw -losp-appfw
DUMMY_LDFLAGS =

all:	$(CAPI_TARGET) $(TIZEN_TARGET) $(DUMMY_TARGET)
capi:	$(CAPI_TARGET)
tizen:	$(TIZEN_TARGET)
dummy:	$(DUMMY_TARGET)

headers:
	cat ./scripts/api_names.txt | awk -f ./scripts/gen_api_id_mapping_header.awk > include/api_id_mapping.h
	cat ./scripts/api_names.txt | awk -f ./scripts/gen_api_id_mapping_header_list.awk > include/api_id_list.h
	cat ./scripts/api_names.txt | awk -f ./scripts/gen_api_id_mapping_list.awk > include/id_list

$(CAPI_TARGET): $(CAPI_SRCS)
	$(CC) $(INC_CAPI) $(CAPI_FLAGS) $(LIBDIR_CAPI) -o $@ $(CAPI_SRCS) $(CAPI_LDFLAGS)

$(TIZEN_TARGET): $(TIZEN_SRCS)
	$(CC) $(INC_TIZEN) $(TIZEN_FLAGS) $(LIBDIR_TIZEN) -o $@ $(TIZEN_SRCS) $(TIZEN_LDFLAGS)

$(DUMMY_TARGET): $(DUMMY_SRCS)
	$(CC) $(INC_TIZEN) $(COMMON_FLAGS) -o $@ $(DUMMY_SRCS) $(DUMMY_LDFLAGS)

install:
	[ -d "$(DESTDIR)/$(INSTALLDIR)" ] || mkdir -p $(DESTDIR)/$(INSTALLDIR)
	install $(TIZEN_TARGET) $(DUMMY_TARGET) $(DESTDIR)/$(INSTALLDIR)/

clean:
	rm -f *.so *.o

