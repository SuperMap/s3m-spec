#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tinyxml2::tinyxml2" for configuration "Release"
set_property(TARGET tinyxml2::tinyxml2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tinyxml2::tinyxml2 PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/tinyxml2.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/tinyxml2.dll"
  )

list(APPEND _cmake_import_check_targets tinyxml2::tinyxml2 )
list(APPEND _cmake_import_check_files_for_tinyxml2::tinyxml2 "${_IMPORT_PREFIX}/lib/tinyxml2.lib" "${_IMPORT_PREFIX}/bin/tinyxml2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
