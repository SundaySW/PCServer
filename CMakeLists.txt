cmake_minimum_required(VERSION 3.23)
project(ProtosCloudServer)

set(CMAKE_CXX_STANDARD 20)

# options
option(ENABLE_TESTS "Set to ON to enable building of tests" OFF)
option(BUILD_SHARED_LIBS "Build ProtosCloudServer shared/static library." OFF)
option(BUILD_DOCUMENTATION "Build doxygen API documentation." OFF)

# sources
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#    add_definitions(-D_WIN32_WINNT=0x0601)
#    add_definitions(-DWIN32_LEAN_AND_MEAN)
#    add_definitions(-DNOMINMAX)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

include_directories(include)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src
    )

set(PROJECT_SOURCES
    ${SOURCES}
        src/net/BoostHttpOnlySslClient.cpp)

set(LIB_LIST
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    )

## openssl
find_package(OpenSSL REQUIRED)
IF (OpenSSL_FOUND)
    message("++ OpenSSL:: OpenSSL found in include dir: ${OPENSSL_INCLUDE_DIR}")
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message("++ OpenSSL:: OpenSSL NOT found, current OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
ENDIF()

## boost
find_package(Boost REQUIRED)
IF (Boost_FOUND)
    message("++ BOOST:: Boost found in root dir: ${BOOST_ROOT}")
    add_definitions( "-DHAS_BOOST" )
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIR_RELEASE})
else()
    message("++ BOOST:: Boost NOT found, current boost root dir: ${BOOST_ROOT}")
ENDIF()

# building project
add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

# tests
if (ENABLE_TESTS)
    message(STATUS "Building of tests is enabled")
    enable_testing()
    add_subdirectory(test)
endif()

# Documentation
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/ TYPE DOC)
endif()

if(BUILD_SHARED_LIBS)
    add_definitions(-DPROTOSCLOUDSERVER_DLL)
endif()