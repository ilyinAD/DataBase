cmake_minimum_required(VERSION 3.13)
include(FetchContent)
project(BigHM)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(MyDataBase)

add_executable(BigHM main.cpp
        MyDataBase/dataBase/DataBase.h
        MyDataBase/utils/utils.h
        MyDataBase/utils/utils.cpp
        MyDataBase/dataBase/DataBase.cpp
        MyDataBase/conditionParser/ExprChecker.h
        MyDataBase/conditionParser/ExprChecker.cpp
        MyDataBase/domain/DataBaseType.h
        MyDataBase/domain/ColTable.h
        MyDataBase/domain/DataBaseType.cpp
        MyDataBase/table/Table.h
        MyDataBase/domain/Element.h
        MyDataBase/table/Table.cpp
)

#set(SOURCE_FILES main.cpp
#        MyDataBase/dataBase/DataBase.h
#        MyDataBase/utils/utils.h
#        MyDataBase/utils/utils.cpp
#        MyDataBase/dataBase/DataBase.cpp
#        MyDataBase/conditionParser/ExprChecker.h
#        MyDataBase/conditionParser/ExprChecker.cpp
#        MyDataBase/domain/DataBaseType.h
#        MyDataBase/domain/ColTable.h
#        MyDataBase/domain/DataBaseType.h
#        MyDataBase/domain/DataBaseType.cpp
#        MyDataBase/table/Table.h
#        MyDataBase/domain/Element.h
#        MyDataBase/table/Table.cpp
#)


target_link_libraries(BigHM PUBLIC MyDataBase_1)

#add_subdirectory(googletest)
enable_testing()

# FetchContent for googletest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/googletest
)
FetchContent_MakeAvailable(googletest)


add_executable(my_tests test/conditionParser_test.cpp test/DataBase_test.cpp)
target_link_libraries(my_tests gtest gtest_main MyDataBase_1)

target_include_directories(my_tests PRIVATE ${CMAKE_SOURCE_DIR})
#target_sources(my_tests PRIVATE
#        MyDataBase/conditionParser/ExprChecker.cpp
#        MyDataBase/utils/utils.cpp
#)
#add_executable(BigHM ${SOURCE_FILES})