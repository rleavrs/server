# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_SOURCE_DIR = /home/rleavrs/serve

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rleavrs/serve/build

# Include any dependencies generated for this target.
include CMakeFiles/test_scheduler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_scheduler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_scheduler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_scheduler.dir/flags.make

CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o: ../test/test_scheduler.cc
CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o -MF CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o.d -o CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o -c /home/rleavrs/serve/test/test_scheduler.cc

CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/test/test_scheduler.cc > CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.i

CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/test/test_scheduler.cc -o CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.s

CMakeFiles/test_scheduler.dir/src/fiber.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/fiber.cc.o: ../src/fiber.cc
CMakeFiles/test_scheduler.dir/src/fiber.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_scheduler.dir/src/fiber.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/fiber.cc.o -MF CMakeFiles/test_scheduler.dir/src/fiber.cc.o.d -o CMakeFiles/test_scheduler.dir/src/fiber.cc.o -c /home/rleavrs/serve/src/fiber.cc

CMakeFiles/test_scheduler.dir/src/fiber.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/fiber.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/fiber.cc > CMakeFiles/test_scheduler.dir/src/fiber.cc.i

CMakeFiles/test_scheduler.dir/src/fiber.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/fiber.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/fiber.cc -o CMakeFiles/test_scheduler.dir/src/fiber.cc.s

CMakeFiles/test_scheduler.dir/src/log.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/log.cc.o: ../src/log.cc
CMakeFiles/test_scheduler.dir/src/log.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_scheduler.dir/src/log.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/log.cc.o -MF CMakeFiles/test_scheduler.dir/src/log.cc.o.d -o CMakeFiles/test_scheduler.dir/src/log.cc.o -c /home/rleavrs/serve/src/log.cc

CMakeFiles/test_scheduler.dir/src/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/log.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/log.cc > CMakeFiles/test_scheduler.dir/src/log.cc.i

CMakeFiles/test_scheduler.dir/src/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/log.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/log.cc -o CMakeFiles/test_scheduler.dir/src/log.cc.s

CMakeFiles/test_scheduler.dir/src/mutex.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/mutex.cc.o: ../src/mutex.cc
CMakeFiles/test_scheduler.dir/src/mutex.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_scheduler.dir/src/mutex.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/mutex.cc.o -MF CMakeFiles/test_scheduler.dir/src/mutex.cc.o.d -o CMakeFiles/test_scheduler.dir/src/mutex.cc.o -c /home/rleavrs/serve/src/mutex.cc

CMakeFiles/test_scheduler.dir/src/mutex.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/mutex.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/mutex.cc > CMakeFiles/test_scheduler.dir/src/mutex.cc.i

CMakeFiles/test_scheduler.dir/src/mutex.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/mutex.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/mutex.cc -o CMakeFiles/test_scheduler.dir/src/mutex.cc.s

CMakeFiles/test_scheduler.dir/src/scheduler.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/scheduler.cc.o: ../src/scheduler.cc
CMakeFiles/test_scheduler.dir/src/scheduler.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_scheduler.dir/src/scheduler.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/scheduler.cc.o -MF CMakeFiles/test_scheduler.dir/src/scheduler.cc.o.d -o CMakeFiles/test_scheduler.dir/src/scheduler.cc.o -c /home/rleavrs/serve/src/scheduler.cc

CMakeFiles/test_scheduler.dir/src/scheduler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/scheduler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/scheduler.cc > CMakeFiles/test_scheduler.dir/src/scheduler.cc.i

CMakeFiles/test_scheduler.dir/src/scheduler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/scheduler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/scheduler.cc -o CMakeFiles/test_scheduler.dir/src/scheduler.cc.s

CMakeFiles/test_scheduler.dir/src/thread.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/thread.cc.o: ../src/thread.cc
CMakeFiles/test_scheduler.dir/src/thread.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_scheduler.dir/src/thread.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/thread.cc.o -MF CMakeFiles/test_scheduler.dir/src/thread.cc.o.d -o CMakeFiles/test_scheduler.dir/src/thread.cc.o -c /home/rleavrs/serve/src/thread.cc

CMakeFiles/test_scheduler.dir/src/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/thread.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/thread.cc > CMakeFiles/test_scheduler.dir/src/thread.cc.i

CMakeFiles/test_scheduler.dir/src/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/thread.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/thread.cc -o CMakeFiles/test_scheduler.dir/src/thread.cc.s

CMakeFiles/test_scheduler.dir/src/timer.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/timer.cc.o: ../src/timer.cc
CMakeFiles/test_scheduler.dir/src/timer.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test_scheduler.dir/src/timer.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/timer.cc.o -MF CMakeFiles/test_scheduler.dir/src/timer.cc.o.d -o CMakeFiles/test_scheduler.dir/src/timer.cc.o -c /home/rleavrs/serve/src/timer.cc

CMakeFiles/test_scheduler.dir/src/timer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/timer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/timer.cc > CMakeFiles/test_scheduler.dir/src/timer.cc.i

CMakeFiles/test_scheduler.dir/src/timer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/timer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/timer.cc -o CMakeFiles/test_scheduler.dir/src/timer.cc.s

CMakeFiles/test_scheduler.dir/src/utility.cc.o: CMakeFiles/test_scheduler.dir/flags.make
CMakeFiles/test_scheduler.dir/src/utility.cc.o: ../src/utility.cc
CMakeFiles/test_scheduler.dir/src/utility.cc.o: CMakeFiles/test_scheduler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test_scheduler.dir/src/utility.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_scheduler.dir/src/utility.cc.o -MF CMakeFiles/test_scheduler.dir/src/utility.cc.o.d -o CMakeFiles/test_scheduler.dir/src/utility.cc.o -c /home/rleavrs/serve/src/utility.cc

CMakeFiles/test_scheduler.dir/src/utility.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/src/utility.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rleavrs/serve/src/utility.cc > CMakeFiles/test_scheduler.dir/src/utility.cc.i

CMakeFiles/test_scheduler.dir/src/utility.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/src/utility.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rleavrs/serve/src/utility.cc -o CMakeFiles/test_scheduler.dir/src/utility.cc.s

# Object files for target test_scheduler
test_scheduler_OBJECTS = \
"CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o" \
"CMakeFiles/test_scheduler.dir/src/fiber.cc.o" \
"CMakeFiles/test_scheduler.dir/src/log.cc.o" \
"CMakeFiles/test_scheduler.dir/src/mutex.cc.o" \
"CMakeFiles/test_scheduler.dir/src/scheduler.cc.o" \
"CMakeFiles/test_scheduler.dir/src/thread.cc.o" \
"CMakeFiles/test_scheduler.dir/src/timer.cc.o" \
"CMakeFiles/test_scheduler.dir/src/utility.cc.o"

# External object files for target test_scheduler
test_scheduler_EXTERNAL_OBJECTS =

../bin/test_scheduler: CMakeFiles/test_scheduler.dir/test/test_scheduler.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/fiber.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/log.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/mutex.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/scheduler.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/thread.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/timer.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/src/utility.cc.o
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/build.make
../bin/test_scheduler: CMakeFiles/test_scheduler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rleavrs/serve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable ../bin/test_scheduler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_scheduler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_scheduler.dir/build: ../bin/test_scheduler
.PHONY : CMakeFiles/test_scheduler.dir/build

CMakeFiles/test_scheduler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_scheduler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_scheduler.dir/clean

CMakeFiles/test_scheduler.dir/depend:
	cd /home/rleavrs/serve/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rleavrs/serve /home/rleavrs/serve /home/rleavrs/serve/build /home/rleavrs/serve/build /home/rleavrs/serve/build/CMakeFiles/test_scheduler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_scheduler.dir/depend

