cmake_minimum_required (VERSION 3.1)

project (gen)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

SET(OpenCV_DIR /home/daniel/OpenCV/installation/OpenCV-master/lib/cmake/opencv4)
find_package( OpenCV REQUIRED )

#dd_compile_options(-std=c++11)
#add_compile_options(-w)
#add_compile_options(-Wno-int-in-bool-context)

#add_compile_options(-fopenmp)

# Used to resolve absolute path names
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_VERBOSE_MAKEFILE false)

# Now set the RW/RWSIM root
SET(RWUser0 "/home/daniel/RobWork/RobWork") #various alternatives are tested first
SET(RWUser1 "/home/smathiesen/RobWork/trunk/RobWork") #various alternatives are tested first
IF(EXISTS ${RWUser0})
    SET(RW_ROOT ${RWUser0})
ELSEIF(EXISTS ${RWUser1})
    SET(RW_ROOT ${RWUser1})
ENDIF()

SET(RWSIM_ROOT "${RW_ROOT}/../RobWorkSim")

# We use the settings that robwork studio uses
IF( NOT DEFINED CMAKE_BUILD_TYPE )
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

#Use RobWork and RobWorkSim
SET(RobWork_DIR ${RW_ROOT}/cmake)
SET(RobWorkSim_DIR ${RWSIM_ROOT}/cmake)
FIND_PACKAGE(RobWork REQUIRED)
FIND_PACKAGE(RobWorkSim REQUIRED)

INCLUDE_DIRECTORIES( ${ROBWORK_INCLUDE_DIRS} ${ROBWORKSIM_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${ROBWORK_LIBRARY_DIRS} ${ROBWORKSIM_LIBRARY_DIRS} )


#Include RobWorkSim settings

MESSAGE(" ${RWSIM_ROOT} --")
SET(CMAKE_MODULE_PATH ${RW_ROOT}/build ${RWSIM_ROOT}/build ${CMAKE_MODULE_PATH})

# Set the output dir for generated libraries and binaries
IF(MSVC)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin" CACHE PATH "Runtime directory" FORCE)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/libs" CACHE PATH "Library directory" FORCE)
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ROOT}/libs" CACHE PATH "Archive directory" FORCE)
ELSE()
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin/${CMAKE_BUILD_TYPE}" CACHE PATH "Runtime directory" FORCE)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/libs/${CMAKE_BUILD_TYPE}" CACHE PATH "Library directory" FORCE)
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ROOT}/libs/${CMAKE_BUILD_TYPE}" CACHE PATH "Archive directory" FORCE)
ENDIF()

# openmp is used to parallize some for-loops in some code
FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()


#LINK_DIRECTORIES( ${BulletLinkDir} ${OdeLinkDir} )
#INCLUDE_DIRECTORIES( ${BulletIncludes} ${OdeIncludes} )
#INCLUDE_DIRECTORIES("${RW_ROOT}/example/simplegui")

# prevent Cxx from optimizing parammeters oute
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")


add_executable(design src/main.cpp src/PartDesigner.cpp src/PartDesigner.h src/FeederDesigner.cpp src/FeederDesigner.h src/ImageHandle.cpp src/ImageHandle.h)
target_link_libraries( design ${OpenCV_LIBS} ${ROBWORK_LIBRARIES} ${ROBWORKSIM_LIBRARIES})
