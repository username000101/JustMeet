cmake_minimum_required(VERSION 3.30)

project(JustMeet LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIDebughRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -g")

find_package(Boost CONFIG REQUIRED COMPONENTS system)
find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(hiredis REQUIRED)
find_package(spdlog REQUIRED)
find_package(TgBot REQUIRED)

include_directories(include)

file(GLOB Database "include/Database/*.hxx" "src/Database/*.cxx")
file(GLOB Executors "include/Logic/Executors/*.hxx" "src/Logic/Executors/*.cxx")
file(GLOB Handlers "include/Logic/Handlers/*.hxx" "src/Logic/Handlers/*.cxx")
file(GLOB JustMeet "main.cpp")
file(GLOB RuntimeStorage "include/Runtime/*.hxx" "src/Runtime/*.cxx")
file(GLOB Logic "include/Logic/*.hxx" "src/Logic/*.cxx")
file(GLOB Utils "include/Utils/*.hxx" "src/Utils/*.cxx")

add_executable(JustMeet
    ${JustMeet}
    ${Database}
    ${Executors}
    ${Handlers}
    ${RuntimeStorage}
    ${Logic}
    ${Utils}
)

if (NOT DEFINED TOKEN)
    message(FATAL_ERROR "You need to specify the TOKEN variable")
endif()

target_compile_definitions(JustMeet PRIVATE
    TG_BOT_TOKEN="${TOKEN}"
)

target_link_libraries(JustMeet PRIVATE
    ${Boost_LIBRARIES}
    ${ICU_LIBRARY}
    hiredis::hiredis
    spdlog::spdlog
    TgBot::TgBot
    hiredis
)
