cmake_minimum_required(VERSION 3.3)
project(synthlib2parser CXX)

set(BUILD_TYPES debug opt optlto prof proflto eopt eoptlto eprof eproflto gperf)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'opt' as none was specified.")
  message(STATUS "Possible build types include: debug, opt, optlto, prof ...")
  set(CMAKE_BUILD_TYPE opt)
else()
  message(STATUS "Current build type is ${CMAKE_BUILD_TYPE}")
endif()

set(MY_LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


###############################################################################
## dependencies ###############################################################
###############################################################################

find_package(Boost 1.36.0 REQUIRED COMPONENTS program_options)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost was not found!! Please set variables BOOST_ROOT or"
  " CMAKE_PREFIX_PATH to search for it. Alternatively, you can manually set variables"
  " Boost_INCLUDE_DIRS and Boost_LIBRARY_DIRS")
endif()

###############################################################################
## compiler flags #########################################################
###############################################################################

message(STATUS "Boost includes found in: ${Boost_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wno-write-strings\
 -Wno-unused-function -D__STDC_LIMIT_MACROS -pipe -std=c++11" CACHE STRING
    "Flags used by the C++ compiler during all builds."
    FORCE)

include_directories(${CMAKE_HOME_DIRECTORY}/src/include ${Boost_INCLUDE_DIRS})
link_directories(${MY_LIBRARY_OUTPUT_PATH} ${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -fno-inline" CACHE STRING
    "Flags used by the C++ compiler during debug builds."
    FORCE)

set(CMAKE_CXX_FLAGS_OPT "-O2 -g" CACHE STRING
    "Flags used by the C++ compiler during opt builds."
    FORCE)

set(CMAKE_CXX_FLAGS_OPTLTO "-O3 -flto" CACHE STRING
    "Flags used by the C++ compiler during optlto builds."
    FORCE)

set(CMAKE_CXX_FLAGS_PROF "-O3 -pg" CACHE STRING
    "Flags used by the C++ compiler during prof builds."
    FORCE)

###############################################################################
## pre-build commands #########################################################
###############################################################################

set(bison-gen-files
  ${CMAKE_SOURCE_DIR}/src/SynthLib2Parser.tab.cpp
  ${CMAKE_SOURCE_DIR}/src/SynthLib2Parser.tab.h
  ${CMAKE_SOURCE_DIR}/src/SynthLib2Parser.output
)

set(flex-gen-files
  ${CMAKE_SOURCE_DIR}/src/SynthLib2Lexer.cpp
)

add_custom_target(synthlib-gen-files
  DEPENDS ${flex-gen-files} ${bison-gen-files}
)

add_custom_command(OUTPUT ${bison-gen-files}
    COMMAND bison -v --defines=SynthLib2Parser.tab.h -o SynthLib2Parser.tab.cpp SynthLib2Parser.y
    DEPENDS ${CMAKE_SOURCE_DIR}/src/SynthLib2Parser.y
    BYPRODUCTS ${bison-gen-files}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
    COMMENT "Bison is generating the required tokenizer ..."
)

add_custom_command(OUTPUT ${flex-gen-files}
    COMMAND flex -o SynthLib2Lexer.cpp SynthLib2Lexer.l
    DEPENDS ${CMAKE_SOURCE_DIR}/src/SynthLib2Lexer.l
    BYPRODUCTS ${flex-gen-files}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
    COMMENT "Flex is generating the required lexer ..."
)

###############################################################################
## target definitions #########################################################
###############################################################################

set(MAIN_SOURCES
  src/SynthLib2ParserAST.cpp
  src/SynthLib2ParserExceptions.cpp
  src/PrintVisitor.cpp
  ${flex-gen-files}
  ${bison-gen-files}
)

add_library(synthlib-static STATIC ${MAIN_SOURCES})
add_library(synthlib-shared SHARED ${MAIN_SOURCES})

set_target_properties(synthlib-shared PROPERTIES
  OUTPUT_NAME synthlib2parser
  CLEAN_DIRECT_OUTPUT 1
)

set_target_properties(synthlib-static PROPERTIES
  OUTPUT_NAME synthlib2parser
  CLEAN_DIRECT_OUTPUT 1
)

set_target_properties(synthlib-shared PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${MY_LIBRARY_OUTPUT_PATH}
)
set_target_properties(synthlib-static PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${MY_LIBRARY_OUTPUT_PATH}
)

add_dependencies(synthlib-static synthlib-gen-files)
add_dependencies(synthlib-shared synthlib-gen-files)

set(TESTER_SOURCES src/SynthLib2ParserTester.cpp)
add_executable(synthlib2tester ${TESTER_SOURCES})
target_link_libraries(synthlib2tester synthlib-static)

###############################################################################
## packaging ##################################################################
###############################################################################

# Install dir is relative to CMAKE_INSTALL_PREFIX which is /usr/local by default
install(TARGETS synthlib-shared synthlib-static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

set(public-headers
  ${CMAKE_SOURCE_DIR}/src/include/SynthLib2ParserCommon.hpp
  ${CMAKE_SOURCE_DIR}/src/include/SynthLib2ParserFwd.hpp
  ${CMAKE_SOURCE_DIR}/src/include/SynthLib2ParserIFace.hpp
  ${CMAKE_SOURCE_DIR}/src/include/PrintVisitor.hpp
  ${CMAKE_SOURCE_DIR}/src/include/SynthLib2ParserExceptions.hpp
)

# Install header files
install(FILES ${public-headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/synthlib2parser/)

# for uninstalling use the following command
# $ xargs rm < install_manifest.txt
