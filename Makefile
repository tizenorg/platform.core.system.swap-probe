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
INC_TIZEN = $(INC_COMMON) #-I/usr/include/vconf -I/usr/include/pixman-1
INC_OSP = $(INC_COMMON) -I/usr/include/osp

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

TIZEN_SRCS =$(COMMON_SRCS) \
			./helper/addr-capi.c \
			./probe_tizenapi/tizen_appfw.c \
			./probe_ui/tizen_capture.c

OSP_SRCS =	$(COMMON_SRCS) \
			./helper/addr-tizen.c \
			./probe_memory/libdanew.cpp \
			./probe_badaapi/bada_file.cpp \
			./probe_badaapi/bada_thread.cpp \
			./probe_badaapi/bada_lifecycle.cpp \
			./probe_badaapi/bada_sync.cpp \
			./probe_badaapi/osp_controls.cpp \
			./probe_badaapi/osp_constructor.cpp \
			./probe_event/gesture.cpp \
			./probe_ui/osp_capture.cpp \
			./probe_ui/osp_scenemanager.cpp \
			./probe_ui/osp_frameani.cpp \
			./probe_ui/osp_display.cpp \
			./probe_graphics/da_gles20.cpp

DUMMY_SRCS = ./custom_chart/da_chart_dummy.c

TIZEN_TARGET = da_probe_tizen.so
OSP_TARGET = da_probe_osp.so
DUMMY_TARGET = libdaprobe.so

COMMON_FLAGS = -D_GNU_SOURCE -fPIC -shared -Wall -funwind-tables -fomit-frame-pointer -Xlinker --no-undefined
TIZEN_FLAGS = $(COMMON_FLAGS)
OSP_FLAGS = $(COMMON_FLAGS) -DOSPAPP

LIBDIR_COMMON = 
LIBDIR_TIZEN = $(LIBDIR_COMMON) 
LIBDIR_OSP = $(LIBDIR_COMMON) -L/usr/lib/osp

COMMON_LDFLAGS = -ldl -lpthread -lrt -lecore -levas -lecore_input -leina -lecore_x -lcapi-system-runtime-info -lcapi-appfw-application -lX11 -lXext
TIZEN_LDFLAGS = $(COMMON_LDFLAGS)
OSP_LDFLAGS = $(COMMON_LDFLAGS) -lstdc++ -losp-uifw -losp-appfw
DUMMY_LDFLAGS =

all:	$(TIZEN_TARGET) $(OSP_TARGET) $(DUMMY_TARGET)
tizen:	$(TIZEN_TARGET)
osp:	$(OSP_TARGET)
dummy:	$(DUMMY_TARGET)

headers:
	cat ./scripts/api_names.txt | awk -f ./scripts/gen_api_id_mapping_header.awk > include/api_id_mapping.h
	cat ./scripts/api_names.txt | awk -f ./scripts/gen_api_id_mapping_header_list.awk > include/api_id_list.h

$(TIZEN_TARGET): $(TIZEN_SRCS)
	$(CC) $(INC_TIZEN) $(TIZEN_FLAGS) $(LIBDIR_TIZEN) -o $@ $(TIZEN_SRCS) $(TIZEN_LDFLAGS)

$(OSP_TARGET): $(OSP_SRCS)
	$(CC) $(INC_OSP) $(OSP_FLAGS) $(LIBDIR_OSP) -o $@ $(OSP_SRCS) $(OSP_LDFLAGS)

$(DUMMY_TARGET): $(DUMMY_SRCS)
	$(CC) $(INC_OSP) $(COMMON_FLAGS) -o $@ $(DUMMY_SRCS) $(DUMMY_LDFLAGS)


install:
	[ -d "$(DESTDIR)/$(INSTALLDIR)" ] || mkdir -p $(DESTDIR)/$(INSTALLDIR)
	install $(OSP_TARGET) $(DUMMY_TARGET) $(DESTDIR)/$(INSTALLDIR)/

clean:
	rm -f *.so *.o

