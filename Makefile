#############################################################################
# Makefile for building: final
# Generated by qmake (2.01a) (Qt 4.6.3) on: Tue Dec 11 19:04:47 2012
# Project:  final.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile final.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -Ibranch -Ilib -I/usr/X11R6/include -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -L/usr/X11R6/lib -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
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

SOURCES       = main.cpp \
		mainwindow.cpp \
		view.cpp \
		settings.cpp \
		camera.cpp \
		bottom_terrain.cpp \
		branch/Branch.cpp \
		branch/BranchFactory.cpp \
		branch/LSystem.cpp \
		lib/CS123Matrix.cpp moc_mainwindow.cpp \
		moc_view.cpp
OBJECTS       = main.o \
		mainwindow.o \
		view.o \
		settings.o \
		camera.o \
		bottom_terrain.o \
		Branch.o \
		BranchFactory.o \
		LSystem.o \
		CS123Matrix.o \
		moc_mainwindow.o \
		moc_view.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		final.pro
QMAKE_TARGET  = final
DESTDIR       = 
TARGET        = final

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

$(TARGET): ui_mainwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: final.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtOpenGL.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile final.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtOpenGL.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile final.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/final1.0.0 || $(MKDIR) .tmp/final1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/final1.0.0/ && $(COPY_FILE) --parents mainwindow.h view.h camera.h settings.h bottom_terrain.h branch/Branc.h branch/BranchFactory.h branch/LSystem.h lib/CS123Algebra.h lib/CS123Common.h .tmp/final1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp view.cpp settings.cpp camera.cpp bottom_terrain.cpp branch/Branch.cpp branch/BranchFactory.cpp branch/LSystem.cpp lib/CS123Matrix.cpp .tmp/final1.0.0/ && $(COPY_FILE) --parents mainwindow.ui .tmp/final1.0.0/ && (cd `dirname .tmp/final1.0.0` && $(TAR) final1.0.0.tar final1.0.0 && $(COMPRESS) final1.0.0.tar) && $(MOVE) `dirname .tmp/final1.0.0`/final1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/final1.0.0


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

compiler_moc_header_make_all: moc_mainwindow.cpp moc_view.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_view.cpp
moc_mainwindow.cpp: mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_view.cpp: camera.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h \
		bottom_terrain.h \
		branch/BranchFactory.h \
		branch/Branch.h \
		branch/LSystem.h \
		view.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) view.h -o moc_view.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h
ui_mainwindow.h: mainwindow.ui \
		view.h \
		camera.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h \
		bottom_terrain.h \
		branch/BranchFactory.h \
		branch/Branch.h \
		branch/LSystem.h
	/usr/bin/uic-qt4 mainwindow.ui -o ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		settings.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		ui_mainwindow.h \
		view.h \
		camera.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h \
		bottom_terrain.h \
		branch/BranchFactory.h \
		branch/Branch.h \
		branch/LSystem.h \
		settings.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

view.o: view.cpp view.h \
		camera.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h \
		bottom_terrain.h \
		branch/BranchFactory.h \
		branch/Branch.h \
		branch/LSystem.h \
		settings.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view.o view.cpp

settings.o: settings.cpp settings.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o settings.o settings.cpp

camera.o: camera.cpp camera.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o camera.o camera.cpp

bottom_terrain.o: bottom_terrain.cpp bottom_terrain.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h \
		camera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o bottom_terrain.o bottom_terrain.cpp

Branch.o: branch/Branch.cpp branch/Branch.h \
		branch/LSystem.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Branch.o branch/Branch.cpp

BranchFactory.o: branch/BranchFactory.cpp branch/BranchFactory.h \
		branch/Branch.h \
		branch/LSystem.h \
		lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o BranchFactory.o branch/BranchFactory.cpp

LSystem.o: branch/LSystem.cpp branch/LSystem.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o LSystem.o branch/LSystem.cpp

CS123Matrix.o: lib/CS123Matrix.cpp lib/CS123Algebra.h \
		lib/CS123Vector.inl \
		lib/CS123Matrix.inl \
		lib/CS123Common.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CS123Matrix.o lib/CS123Matrix.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_view.o: moc_view.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_view.o moc_view.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

