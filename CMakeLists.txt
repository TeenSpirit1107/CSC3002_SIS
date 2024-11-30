cmake_minimum_required(VERSION 3.29)
project(CSC3002_SIS)

set(CMAKE_CXX_STANDARD 14)

include_directories(sis_ws/include)

link_libraries(libeasyx.a libgdi32.a libole32.a)

add_executable(CSC3002_SIS
#        sis_ws/src/registryTest.cpp
        sis_ws/src/client.cpp
        sis_ws/src/registry.cpp
        sis_ws/src/course.cpp
        sis_ws/src/staff.cpp
        sis_ws/src/student.cpp
        sis_ws/src/staffTest.cpp
#        sis_ws/src/studentTest.cpp
        #        sis_ws/src/studentTest.cpp
)
