# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/utsav/proj/expl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/utsav/proj/expl

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/utsav/proj/expl/CMakeFiles /home/utsav/proj/expl//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/utsav/proj/expl/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named exp

# Build rule for target.
exp: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 exp
.PHONY : exp

# fast build rule for target.
exp/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/build
.PHONY : exp/fast

src/WinTerm/ansi/stdinReader.o: src/WinTerm/ansi/stdinReader.cpp.o
.PHONY : src/WinTerm/ansi/stdinReader.o

# target to build an object file
src/WinTerm/ansi/stdinReader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/ansi/stdinReader.cpp.o
.PHONY : src/WinTerm/ansi/stdinReader.cpp.o

src/WinTerm/ansi/stdinReader.i: src/WinTerm/ansi/stdinReader.cpp.i
.PHONY : src/WinTerm/ansi/stdinReader.i

# target to preprocess a source file
src/WinTerm/ansi/stdinReader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/ansi/stdinReader.cpp.i
.PHONY : src/WinTerm/ansi/stdinReader.cpp.i

src/WinTerm/ansi/stdinReader.s: src/WinTerm/ansi/stdinReader.cpp.s
.PHONY : src/WinTerm/ansi/stdinReader.s

# target to generate assembly for a file
src/WinTerm/ansi/stdinReader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/ansi/stdinReader.cpp.s
.PHONY : src/WinTerm/ansi/stdinReader.cpp.s

src/WinTerm/events/event.o: src/WinTerm/events/event.cpp.o
.PHONY : src/WinTerm/events/event.o

# target to build an object file
src/WinTerm/events/event.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/events/event.cpp.o
.PHONY : src/WinTerm/events/event.cpp.o

src/WinTerm/events/event.i: src/WinTerm/events/event.cpp.i
.PHONY : src/WinTerm/events/event.i

# target to preprocess a source file
src/WinTerm/events/event.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/events/event.cpp.i
.PHONY : src/WinTerm/events/event.cpp.i

src/WinTerm/events/event.s: src/WinTerm/events/event.cpp.s
.PHONY : src/WinTerm/events/event.s

# target to generate assembly for a file
src/WinTerm/events/event.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/events/event.cpp.s
.PHONY : src/WinTerm/events/event.cpp.s

src/WinTerm/events/queue.o: src/WinTerm/events/queue.cpp.o
.PHONY : src/WinTerm/events/queue.o

# target to build an object file
src/WinTerm/events/queue.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/events/queue.cpp.o
.PHONY : src/WinTerm/events/queue.cpp.o

src/WinTerm/events/queue.i: src/WinTerm/events/queue.cpp.i
.PHONY : src/WinTerm/events/queue.i

# target to preprocess a source file
src/WinTerm/events/queue.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/events/queue.cpp.i
.PHONY : src/WinTerm/events/queue.cpp.i

src/WinTerm/events/queue.s: src/WinTerm/events/queue.cpp.s
.PHONY : src/WinTerm/events/queue.s

# target to generate assembly for a file
src/WinTerm/events/queue.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/events/queue.cpp.s
.PHONY : src/WinTerm/events/queue.cpp.s

src/WinTerm/misc/size.o: src/WinTerm/misc/size.cpp.o
.PHONY : src/WinTerm/misc/size.o

# target to build an object file
src/WinTerm/misc/size.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/misc/size.cpp.o
.PHONY : src/WinTerm/misc/size.cpp.o

src/WinTerm/misc/size.i: src/WinTerm/misc/size.cpp.i
.PHONY : src/WinTerm/misc/size.i

# target to preprocess a source file
src/WinTerm/misc/size.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/misc/size.cpp.i
.PHONY : src/WinTerm/misc/size.cpp.i

src/WinTerm/misc/size.s: src/WinTerm/misc/size.cpp.s
.PHONY : src/WinTerm/misc/size.s

# target to generate assembly for a file
src/WinTerm/misc/size.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/misc/size.cpp.s
.PHONY : src/WinTerm/misc/size.cpp.s

src/WinTerm/render/canvas.o: src/WinTerm/render/canvas.cpp.o
.PHONY : src/WinTerm/render/canvas.o

# target to build an object file
src/WinTerm/render/canvas.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/render/canvas.cpp.o
.PHONY : src/WinTerm/render/canvas.cpp.o

src/WinTerm/render/canvas.i: src/WinTerm/render/canvas.cpp.i
.PHONY : src/WinTerm/render/canvas.i

# target to preprocess a source file
src/WinTerm/render/canvas.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/render/canvas.cpp.i
.PHONY : src/WinTerm/render/canvas.cpp.i

src/WinTerm/render/canvas.s: src/WinTerm/render/canvas.cpp.s
.PHONY : src/WinTerm/render/canvas.s

# target to generate assembly for a file
src/WinTerm/render/canvas.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/render/canvas.cpp.s
.PHONY : src/WinTerm/render/canvas.cpp.s

src/WinTerm/render/render.o: src/WinTerm/render/render.cpp.o
.PHONY : src/WinTerm/render/render.o

# target to build an object file
src/WinTerm/render/render.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/render/render.cpp.o
.PHONY : src/WinTerm/render/render.cpp.o

src/WinTerm/render/render.i: src/WinTerm/render/render.cpp.i
.PHONY : src/WinTerm/render/render.i

# target to preprocess a source file
src/WinTerm/render/render.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/render/render.cpp.i
.PHONY : src/WinTerm/render/render.cpp.i

src/WinTerm/render/render.s: src/WinTerm/render/render.cpp.s
.PHONY : src/WinTerm/render/render.s

# target to generate assembly for a file
src/WinTerm/render/render.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/render/render.cpp.s
.PHONY : src/WinTerm/render/render.cpp.s

src/WinTerm/winTerm.o: src/WinTerm/winTerm.cpp.o
.PHONY : src/WinTerm/winTerm.o

# target to build an object file
src/WinTerm/winTerm.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/winTerm.cpp.o
.PHONY : src/WinTerm/winTerm.cpp.o

src/WinTerm/winTerm.i: src/WinTerm/winTerm.cpp.i
.PHONY : src/WinTerm/winTerm.i

# target to preprocess a source file
src/WinTerm/winTerm.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/winTerm.cpp.i
.PHONY : src/WinTerm/winTerm.cpp.i

src/WinTerm/winTerm.s: src/WinTerm/winTerm.cpp.s
.PHONY : src/WinTerm/winTerm.s

# target to generate assembly for a file
src/WinTerm/winTerm.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/WinTerm/winTerm.cpp.s
.PHONY : src/WinTerm/winTerm.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/exp.dir/build.make CMakeFiles/exp.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... exp"
	@echo "... src/WinTerm/ansi/stdinReader.o"
	@echo "... src/WinTerm/ansi/stdinReader.i"
	@echo "... src/WinTerm/ansi/stdinReader.s"
	@echo "... src/WinTerm/events/event.o"
	@echo "... src/WinTerm/events/event.i"
	@echo "... src/WinTerm/events/event.s"
	@echo "... src/WinTerm/events/queue.o"
	@echo "... src/WinTerm/events/queue.i"
	@echo "... src/WinTerm/events/queue.s"
	@echo "... src/WinTerm/misc/size.o"
	@echo "... src/WinTerm/misc/size.i"
	@echo "... src/WinTerm/misc/size.s"
	@echo "... src/WinTerm/render/canvas.o"
	@echo "... src/WinTerm/render/canvas.i"
	@echo "... src/WinTerm/render/canvas.s"
	@echo "... src/WinTerm/render/render.o"
	@echo "... src/WinTerm/render/render.i"
	@echo "... src/WinTerm/render/render.s"
	@echo "... src/WinTerm/winTerm.o"
	@echo "... src/WinTerm/winTerm.i"
	@echo "... src/WinTerm/winTerm.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

