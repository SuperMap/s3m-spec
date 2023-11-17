cmake_minimum_required(VERSION 3.10)

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE} CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(s3m)

if(WIN32)
	option(build_Shared ON)
else()
	option(build_Shared ON)
	set(build_Shared ON)
endif()
option(build_sample ON)
set(build_sample ON)

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rdparty)
set(SAMPLEDATA_DIR ${PROJECT_SOURCE_DIR}/sampledata)

AUX_SOURCE_DIRECTORY(s3m/src S3M_SRCS)

file(GLOB_RECURSE S3M_INCS ${PROJECT_SOURCE_DIR}/s3m/include/*.h)
source_group("Header Files" FILES ${S3M_INCS})

if(build_Shared)
	add_library(s3m SHARED ${S3M_SRCS} ${S3M_INCS})
else()
	add_library(s3m ${S3M_SRCS} ${S3M_INCS})
endif()

set_target_properties(s3m PROPERTIES OUTPUT_NAME "s3m")

if(WIN32)
	target_compile_definitions(s3m PRIVATE -D_UNICODE -DUNICODE)
	if(build_Shared)
		target_compile_definitions(s3m PRIVATE -DS3MB_EXPORTS)
	else()
		target_compile_definitions(s3m PUBLIC -DS3MB_LIB)
	endif()
	target_compile_definitions(s3m PUBLIC -D_AMD64_)
else()
        target_compile_definitions(s3m PRIVATE -DS3MB_EXPORTS)
        target_compile_definitions(s3m PUBLIC -D_AMD64_)
endif()

target_include_directories(s3m PUBLIC ${PROJECT_SOURCE_DIR}/s3m/include)
target_include_directories(s3m PRIVATE 
	${THIRD_PARTY_DIR}
	${THIRD_PARTY_DIR}/zlib/include
	${THIRD_PARTY_DIR}/draco/include
	${THIRD_PARTY_DIR}/libjpeg/include
	${THIRD_PARTY_DIR}/jsoncpp/include
	${THIRD_PARTY_DIR}/libsquish/include
	${THIRD_PARTY_DIR}/libpng/include)

if(WIN32)
	add_library(draco UNKNOWN IMPORTED)
	set_target_properties(draco PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/draco/lib/draco.lib)
	
	add_library(zlib UNKNOWN IMPORTED)
	set_target_properties(zlib PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/zlib/lib/x64/Debug/zlibstaticd.lib)
	set_target_properties(zlib PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/zlib/lib/x64/Release/zlibstatic.lib)
	
	add_library(libpng UNKNOWN IMPORTED)
	set_target_properties(libpng PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/libpng/lib/libpng16_staticd.lib)
	set_target_properties(libpng PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/libpng/lib/libpng16_static.lib)
	
	add_library(libjpeg UNKNOWN IMPORTED)
	set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/libjpeg/lib/x64/Debug/jpeg-static.lib)
	set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/libjpeg/lib/x64/Release/jpeg-static.lib)
	
	add_library(libsquish UNKNOWN IMPORTED)
	set_target_properties(libsquish PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/libsquish/lib/squishd.lib)
	set_target_properties(libsquish PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/libsquish/lib/squish.lib)
	
	add_library(jsoncpp UNKNOWN IMPORTED)
	set_target_properties(jsoncpp PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/jsoncpp/lib/x64/Debug/jsoncpp.lib)
	set_target_properties(jsoncpp PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/jsoncpp/lib/x64/Release/jsoncpp.lib)
else()
	add_library(draco UNKNOWN IMPORTED)
        set_target_properties(draco PROPERTIES IMPORTED_LOCATION ${THIRD_PARTY_DIR}/draco/lib/libdraco.so)

        add_library(zlib UNKNOWN IMPORTED)
	set_target_properties(zlib PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/zlib/lib/x64/Release/libz.so)
        set_target_properties(zlib PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/zlib/lib/x64/Release/libz.so)

        add_library(libpng UNKNOWN IMPORTED)
        set_target_properties(libpng PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/libpng/lib/libpng16.so)
        set_target_properties(libpng PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/libpng/lib/libpng16.so)

        add_library(libjpeg UNKNOWN IMPORTED)
	set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/libjpeg/lib/x64/Release/libjpeg.so)
        set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/libjpeg/lib/x64/Release/libjpeg.so)

        add_library(libsquish UNKNOWN IMPORTED)
        set_target_properties(libsquish PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/libsquish/lib/libsquish.so)
        set_target_properties(libsquish PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/libsquish/lib/libsquish.so)

        add_library(jsoncpp UNKNOWN IMPORTED)
	set_target_properties(jsoncpp PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_DIR}/jsoncpp/lib/x64/Release/libjsoncpp.so)
        set_target_properties(jsoncpp PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_DIR}/jsoncpp/lib/x64/Release/libjsoncpp.so)
endif()

if(WIN32)
target_link_libraries(s3m zlib libpng libjpeg draco libsquish jsoncpp)
else()
target_link_libraries(s3m dl zlib libpng libjpeg draco libsquish jsoncpp)
endif()

install(TARGETS s3m
		PUBLIC_HEADER DESTINATION include
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
)
install(FILES ${S3M_INCS} DESTINATION include)

if(build_sample)
	AUX_SOURCE_DIRECTORY(sample/src SAMPLE_SRCS)

	file(GLOB_RECURSE SAMPLE_INCS ${PROJECT_SOURCE_DIR}/sample/include/*.h)
	source_group("Header Files" FILES ${SAMPLE_INCS})

	add_executable(sample ${SAMPLE_SRCS} ${SAMPLE_INCS})

	if(WIN32)
		target_compile_definitions(sample PRIVATE -D_UNICODE -DUNICODE)
	endif()

	target_include_directories(sample PRIVATE 	sample/include)
	target_compile_definitions(sample PRIVATE -DSAMPLEDATA_DIR=L"${SAMPLEDATA_DIR}")
	
	target_link_libraries(sample s3m)
	
	install(TARGETS sample
		PUBLIC_HEADER DESTINATION include
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
	)
endif()
