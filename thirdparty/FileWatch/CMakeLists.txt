include(ExternalProject)

add_dependencies(${PROJECT_NAME} FileWatch)

ExternalProject_Add(FileWatch
  URL "https://github.com/ThomasMonkman/filewatch/raw/a59891baf375b73ff28144973a6fafd3fe40aa21/FileWatch.hpp"
  DOWNLOAD_NO_EXTRACT ON
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy_directory <DOWNLOAD_DIR> ${STAGED_INSTALL_PREFIX}/include/FileWatch
)
