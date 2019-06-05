cmake_minimum_required (VERSION 3.5)
##############
# demo app
#

project(cronejobs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE  
 cronejob.h
 scheduler.h
 scheduler.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)

target_link_libraries(${PROJECT_NAME}
 pthread
)

################
# unit tests
#

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

function (add_unit_test testName)

set(TEST_SOURCE test/${testName}.cpp)
set (PROJECT_TEST_NAME ${testName})

add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCE} ${SOURCE})

target_link_libraries(${PROJECT_TEST_NAME}
    pthread
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
add_test(${PROJECT_TEST_NAME} ${PROJECT_TEST_NAME})

endfunction()

add_unit_test(test_cronejob)
add_unit_test(test_scheduler)