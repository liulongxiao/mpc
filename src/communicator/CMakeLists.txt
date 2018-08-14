set(COMMUNICATOR_SOURCES Reader.cpp Writer.cpp communicator.cpp )
add_library(communicator SHARED ${COMMUNICATOR_SOURCES})
target_link_libraries(communicator journal timer)