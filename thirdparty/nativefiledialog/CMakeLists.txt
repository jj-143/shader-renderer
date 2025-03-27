include(ExternalProject)

set(ARCH "x64")
set(BUILD_TYPE "Release")

if (WIN32)
  set(NFD_LIBS nfd)
  # SHELL=bash to force SHELLTYPE=posix in .make files, to call "mkdir -p" instead of "mkdir" for cmd.exe
  set(NFD_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} SHELL=bash config=$<LOWER_CASE:${BUILD_TYPE}>_${ARCH} -C <SOURCE_DIR>/build/gmake_windows)
elseif(UNIX)
  set(NFD_LIBS nfd gtk-3 glib-2.0)
  set(NFD_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} config=$<LOWER_CASE:${BUILD_TYPE}>_${ARCH} -C <SOURCE_DIR>/build/gmake_linux)
endif()

add_dependencies(${PROJECT_NAME} nfd_external)
target_link_libraries(${PROJECT_NAME} ${NFD_LIBS})

ExternalProject_Add(nfd_external
  GIT_REPOSITORY https://github.com/mlabbe/nativefiledialog
  GIT_TAG release_116
  GIT_SHALLOW ON
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${NFD_BUILD_COMMAND}
  BUILD_BYPRODUCTS ${STAGED_INSTALL_PREFIX}/lib
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/build/lib/${BUILD_TYPE}/${ARCH} ${STAGED_INSTALL_PREFIX}/lib
  COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/src/include ${STAGED_INSTALL_PREFIX}/include/nfd
)
