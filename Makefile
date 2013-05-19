#############################################################################
# Makefile for building: opengl
# Generated by qmake (2.01a) (Qt 4.8.4) on: Sat May 18 21:15:24 2013
# Project:  opengl.pro
# Template: app
# Command: /usr/local/qt4/bin/qmake -o Makefile opengl.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/local/qt4/mkspecs/linux-g++-64 -I. -I/usr/local/qt4/include/QtCore -I/usr/local/qt4/include/QtGui -I/usr/local/qt4/include/QtOpenGL -I/usr/local/qt4/include -I. -I/usr/X11R6/include -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1 -Wl,-rpath,/usr/local/qt4/lib
LIBS          = $(SUBLIBS)  -L/usr/local/qt4/lib -L/usr/X11R6/lib64 -lQtOpenGL -L/usr/local/qt4/lib -L/usr/X11R6/lib64 -lQtGui -lQtCore -lGL -lpthread  -lGL -lGLU -lglut
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/local/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = hedgemesh.cpp \
		main.cpp \
		mainwindow.cpp \
		parser.cpp \
		viewer.cpp moc_mainwindow.cpp \
		moc_viewer.cpp
OBJECTS       = hedgemesh.o \
		main.o \
		mainwindow.o \
		parser.o \
		viewer.o \
		moc_mainwindow.o \
		moc_viewer.o
DIST          = /usr/local/qt4/mkspecs/common/unix.conf \
		/usr/local/qt4/mkspecs/common/linux.conf \
		/usr/local/qt4/mkspecs/common/gcc-base.conf \
		/usr/local/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/local/qt4/mkspecs/common/g++-base.conf \
		/usr/local/qt4/mkspecs/common/g++-unix.conf \
		/usr/local/qt4/mkspecs/qconfig.pri \
		/usr/local/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/qt4/mkspecs/features/qt_functions.prf \
		/usr/local/qt4/mkspecs/features/qt_config.prf \
		/usr/local/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/local/qt4/mkspecs/features/default_pre.prf \
		/usr/local/qt4/mkspecs/features/release.prf \
		/usr/local/qt4/mkspecs/features/default_post.prf \
		/usr/local/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/qt4/mkspecs/features/warn_on.prf \
		/usr/local/qt4/mkspecs/features/qt.prf \
		/usr/local/qt4/mkspecs/features/unix/opengl.prf \
		/usr/local/qt4/mkspecs/features/unix/thread.prf \
		/usr/local/qt4/mkspecs/features/moc.prf \
		/usr/local/qt4/mkspecs/features/resources.prf \
		/usr/local/qt4/mkspecs/features/uic.prf \
		/usr/local/qt4/mkspecs/features/yacc.prf \
		/usr/local/qt4/mkspecs/features/lex.prf \
		/usr/local/qt4/mkspecs/features/include_source_dir.prf \
		opengl.pro
QMAKE_TARGET  = opengl
DESTDIR       = 
TARGET        = opengl

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: opengl.pro  /usr/local/qt4/mkspecs/linux-g++-64/qmake.conf /usr/local/qt4/mkspecs/common/unix.conf \
		/usr/local/qt4/mkspecs/common/linux.conf \
		/usr/local/qt4/mkspecs/common/gcc-base.conf \
		/usr/local/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/local/qt4/mkspecs/common/g++-base.conf \
		/usr/local/qt4/mkspecs/common/g++-unix.conf \
		/usr/local/qt4/mkspecs/qconfig.pri \
		/usr/local/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/qt4/mkspecs/features/qt_functions.prf \
		/usr/local/qt4/mkspecs/features/qt_config.prf \
		/usr/local/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/local/qt4/mkspecs/features/default_pre.prf \
		/usr/local/qt4/mkspecs/features/release.prf \
		/usr/local/qt4/mkspecs/features/default_post.prf \
		/usr/local/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/qt4/mkspecs/features/warn_on.prf \
		/usr/local/qt4/mkspecs/features/qt.prf \
		/usr/local/qt4/mkspecs/features/unix/opengl.prf \
		/usr/local/qt4/mkspecs/features/unix/thread.prf \
		/usr/local/qt4/mkspecs/features/moc.prf \
		/usr/local/qt4/mkspecs/features/resources.prf \
		/usr/local/qt4/mkspecs/features/uic.prf \
		/usr/local/qt4/mkspecs/features/yacc.prf \
		/usr/local/qt4/mkspecs/features/lex.prf \
		/usr/local/qt4/mkspecs/features/include_source_dir.prf \
		/usr/local/qt4/lib/libQtOpenGL.prl \
		/usr/local/qt4/lib/libQtGui.prl \
		/usr/local/qt4/lib/libQtCore.prl
	$(QMAKE) -o Makefile opengl.pro
/usr/local/qt4/mkspecs/common/unix.conf:
/usr/local/qt4/mkspecs/common/linux.conf:
/usr/local/qt4/mkspecs/common/gcc-base.conf:
/usr/local/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/local/qt4/mkspecs/common/g++-base.conf:
/usr/local/qt4/mkspecs/common/g++-unix.conf:
/usr/local/qt4/mkspecs/qconfig.pri:
/usr/local/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/local/qt4/mkspecs/features/qt_functions.prf:
/usr/local/qt4/mkspecs/features/qt_config.prf:
/usr/local/qt4/mkspecs/features/exclusive_builds.prf:
/usr/local/qt4/mkspecs/features/default_pre.prf:
/usr/local/qt4/mkspecs/features/release.prf:
/usr/local/qt4/mkspecs/features/default_post.prf:
/usr/local/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/local/qt4/mkspecs/features/warn_on.prf:
/usr/local/qt4/mkspecs/features/qt.prf:
/usr/local/qt4/mkspecs/features/unix/opengl.prf:
/usr/local/qt4/mkspecs/features/unix/thread.prf:
/usr/local/qt4/mkspecs/features/moc.prf:
/usr/local/qt4/mkspecs/features/resources.prf:
/usr/local/qt4/mkspecs/features/uic.prf:
/usr/local/qt4/mkspecs/features/yacc.prf:
/usr/local/qt4/mkspecs/features/lex.prf:
/usr/local/qt4/mkspecs/features/include_source_dir.prf:
/usr/local/qt4/lib/libQtOpenGL.prl:
/usr/local/qt4/lib/libQtGui.prl:
/usr/local/qt4/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile opengl.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/opengl1.0.0 || $(MKDIR) .tmp/opengl1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/opengl1.0.0/ && $(COPY_FILE) --parents hedgemesh.h mainwindow.h parser.h parserstruct.h viewer.h .tmp/opengl1.0.0/ && $(COPY_FILE) --parents hedgemesh.cpp main.cpp mainwindow.cpp parser.cpp viewer.cpp .tmp/opengl1.0.0/ && (cd `dirname .tmp/opengl1.0.0` && $(TAR) opengl1.0.0.tar opengl1.0.0 && $(COMPRESS) opengl1.0.0.tar) && $(MOVE) `dirname .tmp/opengl1.0.0`/opengl1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/opengl1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_viewer.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_viewer.cpp
moc_mainwindow.cpp: viewer.h \
		parser.h \
		parserstruct.h \
		hedgemesh.h \
		mainwindow.h
	/usr/local/qt4/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_viewer.cpp: parser.h \
		parserstruct.h \
		hedgemesh.h \
		viewer.h
	/usr/local/qt4/bin/moc $(DEFINES) $(INCPATH) viewer.h -o moc_viewer.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

hedgemesh.o: hedgemesh.cpp hedgemesh.h \
		parserstruct.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o hedgemesh.o hedgemesh.cpp

main.o: main.cpp mainwindow.h \
		viewer.h \
		parser.h \
		parserstruct.h \
		hedgemesh.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		viewer.h \
		parser.h \
		parserstruct.h \
		hedgemesh.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

parser.o: parser.cpp parser.h \
		parserstruct.h \
		hedgemesh.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o parser.o parser.cpp

viewer.o: viewer.cpp viewer.h \
		parser.h \
		parserstruct.h \
		hedgemesh.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o viewer.o viewer.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_viewer.o: moc_viewer.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_viewer.o moc_viewer.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

