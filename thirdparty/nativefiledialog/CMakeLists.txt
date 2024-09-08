include(ExternalProject)

set(ARCH "x64")
set(BUILD_TYPE "Release")

ExternalProject_Add(nfd_external
  GIT_REPOSITORY https://github.com/mlabbe/nativefiledialog
  GIT_TAG release_116
  GIT_SHALLOW ON
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} config=$<LOWER_CASE:${BUILD_TYPE}>_${ARCH} -C <SOURCE_DIR>/build/gmake_linux
  BUILD_BYPRODUCTS ${STAGED_INSTALL_PREFIX}/lib
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/build/lib/${BUILD_TYPE}/${ARCH} ${STAGED_INSTALL_PREFIX}/lib
  COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/src/include ${STAGED_INSTALL_PREFIX}/include/nfd
)
