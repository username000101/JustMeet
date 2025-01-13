cmake_minimum_required(VERSION 3.30)

project(JustMeet LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIDebughRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_package(Boost CONFIG REQUIRED COMPONENTS system)
find_package(spdlog REQUIRED)
find_package(TgBot REQUIRED)

include_directories(include)

file(GLOB RuntimeStorage "include/Runtime/*.hxx" "src/Runtime/*.cxx")
file(GLOB Database "include/Database/*.hxx" "src/Database/*.cxx")
file(GLOB Utils "include/Utils/*.hxx" "src/Utils/*.cxx")
file(GLOB JustMeet "main.cpp")

add_executable(JustMeet
    ${JustMeet}
    ${RuntimeStorage}
    ${Database}
    ${Utils}
)

if (NOT DEFINED DB_USERNAME OR NOT DEFINED DB_NAME OR NOT DEFINED DB_PASSWORD)
    message(FATAL_ERROR "You need to specify the DB_USERNAME, DB_NAME and DB_PASSWORD variables")
endif()

target_compile_definitions(JustMeet PRIVATE
    JUSTMEET_DB_USERNAME="${DB_USERNAME}"
    JUSTMEET_DB_NAME="${DB_NAME}"
    JUSTMEET_DB_PASWORD="${DB_PASSWORD}"
)

target_link_libraries(JustMeet PRIVATE
    ${Boost_LIBRARIES}
    spdlog::spdlog
    TgBot::TgBot
    hiredis
)
