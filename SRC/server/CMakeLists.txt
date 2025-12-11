set(
    SERVER_SRC
    listen.cpp
    server.hpp
    server.cpp
)
add_executable(server ${SERVER_SRC})
target_link_libraries(server event)
