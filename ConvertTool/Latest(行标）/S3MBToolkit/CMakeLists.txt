cmake_minimum_required(VERSION 3.10)

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE}  CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (NOT WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1 -std=c++11")
endif()

#If not found Qt5, please install Qt5 On divices
if(NOT ${Qt5_INSTALL_DIR})
	message(STATUS "Set Qt5 Directory to Qt5_DIR as none was specified." )
endif()
set(CMAKE_PREFIX_PATH ${Qt5_INSTALL_DIR})
set(Qt5_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5")
set(Qt5Widgets_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Widgets")
set(Qt5Core_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Core")
set(Qt5Gui_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Gui")
#set(Qt5X11Extras_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5X11Extras")
find_package(Qt5 COMPONENTS Gui Widgets Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR "YES") 
set(CMAKE_AUTOMOC "YES") 
set(CMAKE_AUTOUIC "YES") 

project(S3MBToolkit)

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rdParty)
if(NOT EXISTS ${THIRD_PARTY_DIR}/rapidjson/include)
	 add_subdirectory(${PROJECT_SOURCE_DIR}/3rdParty)
endif()
set(COLLADA2GLTF_LIBRARY_DIR ${THIRD_PARTY_DIR}/COLLADA2GLTF/lib)
set(s3m_LIBRARY_DIR ${THIRD_PARTY_DIR}/s3m/lib)
if (NOT WIN32)
set(libuuid_LIBRARY_DIR ${THIRD_PARTY_DIR}/libuuid/lib)
endif()
set(pcre_LIBRARY_DIR ${THIRD_PARTY_DIR}/pcre/lib)
set(tinyxml2_LIBRARY_DIR ${THIRD_PARTY_DIR}/tinyxml2/lib)
set(zlib_LIBRARY_DIR ${THIRD_PARTY_DIR}/zlib/lib)
set(OPENSCENEGRAPH_LIBRARY_DIR ${THIRD_PARTY_DIR}/openscenegraph/lib)

file(GLOB S3MBToolkit_INCS ${PROJECT_SOURCE_DIR}/S3MBToolkit/Include/*.h)
file(GLOB S3MBToolkit_INCS_UTILS ${PROJECT_SOURCE_DIR}/S3MBToolkit/Include/Utils/*.h)
source_group("Header Files" FILES ${S3MBToolkit_INCS})
source_group("Header Files/Utils" FILES ${S3MBToolkit_INCS_UTILS})

file(GLOB S3MBToolkit_SRCS ${PROJECT_SOURCE_DIR}/S3MBToolkit/Src/*.cpp)
file(GLOB S3MBToolkit_SRCS_UTILS ${PROJECT_SOURCE_DIR}/S3MBToolkit/Src/Utils/*.cpp)
source_group("Source Files" FILES ${S3MBToolkit_SRCS})
source_group("Source Files/Utils" FILES ${S3MBToolkit_SRCS_UTILS})

if (WIN32)
link_directories(${s3m_LIBRARY_DIR} ${COLLADA2GLTF_LIBRARY_DIR} ${pcre_LIBRARY_DIR} ${OPENSCENEGRAPH_LIBRARY_DIR} ${tinyxml2_LIBRARY_DIR} ${zlib_LIBRARY_DIR} ${Qt5_INSTALL_DIR}/lib)
else()
link_directories(${s3m_LIBRARY_DIR} ${COLLADA2GLTF_LIBRARY_DIR} ${libuuid_LIBRARY_DIR} ${pcre_LIBRARY_DIR} ${OPENSCENEGRAPH_LIBRARY_DIR} ${tinyxml2_LIBRARY_DIR} ${zlib_LIBRARY_DIR} ${Qt5_INSTALL_DIR}/lib)
endif()

if(WIN32)
	link_directories(${THIRD_PARTY_DIR}/libxml2/lib ${OPENSCENEGRAPH_LIBRARY_DIR}/osgPlugins)
endif()

add_executable(S3MBToolkit ${S3MBToolkit_INCS} ${S3MBToolkit_INCS_UTILS} ${S3MBToolkit_SRCS} ${S3MBToolkit_SRCS_UTILS} )
if(WIN32)
	target_compile_definitions(S3MBToolkit PRIVATE -D_UNICODE -DUNICODE -DS3MB_LIB)
	target_compile_definitions(S3MBToolkit PRIVATE -DSTK_EXPORTS -DOSG_LIBRARY_STATIC)
else()
      target_compile_options(S3MBToolkit PRIVATE -D_UNICODE -DUNICODE -DS3MB_LIB -fPIC)
endif()

set_target_properties(S3MBToolkit PROPERTIES OUTPUT_NAME "S3MBToolkit")

target_include_directories(S3MBToolkit PUBLIC ${PROJECT_SOURCE_DIR}/S3MBToolkit/Include)
target_include_directories(S3MBToolkit PRIVATE 
	${PROJECT_SOURCE_DIR}/S3MBToolkit
	${THIRD_PARTY_DIR}
	${THIRD_PARTY_DIR}/rapidjson/include/rapidjson
	if (WIN32)
	${THIRD_PARTY_DIR}/libuuid/include
	endif()
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/GLTF/dependencies/draco/src
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/GLTF/include
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/JSON/
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/assetModifiers/
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/dependencies/OpenCOLLADA/OpenCOLLADA/COLLADASaxFrameworkLoader/include
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/dependencies/OpenCOLLADA/OpenCOLLADA/COLLADABaseUtils/include
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/dependencies/OpenCOLLADA/OpenCOLLADA/COLLADAFramework/include
	${THIRD_PARTY_DIR}/COLLADA2GLTF/include/dependencies/OpenCOLLADA/OpenCOLLADA/GeneratedSaxParser/include
	${THIRD_PARTY_DIR}/pcre/include
	${THIRD_PARTY_DIR}/s3m/include
	${THIRD_PARTY_DIR}/openscenegraph/include
	${THIRD_PARTY_DIR}/tinyxml2/include
	${THIRD_PARTY_DIR}/zlib/include
	if (NOT WIN32)
	${CMAKE_BINARY_DIR}/S3MBToolkit_autogen/include
	endif()
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5Core_INCLUDE_DIRS}
	${Qt5_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS})

if(WIN32)
target_link_libraries(S3MBToolkit s3m jpeg libpng16 zlib minizip libxml2 pcre draco squish jsoncpp tinyxml2 osg osgDB osgGA osgText osgSim osgAnimation osgUtil OpenThreads osgdb_obj osgdb_osg osgdb_jpeg osgdb_png osgdb_deprecated_osg osgdb_deprecated_osgtext osgdb_deprecated_osganimation osgdb_serializers_osg COLLADA2GLTF gltf GLTF COLLADASaxFrameworkLoader COLLADABaseUtils GeneratedSaxParser COLLADAFramework MathMLSolver Qt5Widgets Qt5Gui Qt5Core)
else()
target_link_libraries(S3MBToolkit GL pthread s3m png16 jpeg z minizip draco squish jsoncpp uuid tinyxml2 osg osgDB osgUtil OpenThreads osgdb_obj osgdb_osg osgdb_jpeg osgdb_png osgdb_serializers_osg COLLADA2GLTF dracogltf2 GLTF COLLADASaxFrameworkLoader COLLADABaseUtils GeneratedSaxParser COLLADAFramework MathMLSolver Qt5Widgets Qt5Gui Qt5Core)
endif()

