add_library(mpcserver MpcServer.cpp)
add_executable(server run_service.cpp)
target_link_libraries(server mpcserver mysocket journal timer pthread)