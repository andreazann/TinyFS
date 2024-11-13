cmake_minimum_required(VERSION 3.7)
project(TinyFS)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

set(SOURCE_FILES src/main.c lib/datastructures/hashtable.c lib/datastructures/hashtable.h lib/hashing/closedhashing.h lib/macros.h lib/datastructures/list.c lib/datastructures/list.h lib/hashing/openhashing.h src/utils/command.h src/types/tree.h lib/paths.h lib/logging/logging.h lib/logging/logging.c lib/sorting/sorting.c lib/sorting/sorting.h lib/logging/logginglevels.h src/utils/hashing/functions.c src/utils/hashing/functions.h src/utils/general.h)
add_executable(TinyFS ${SOURCE_FILES})