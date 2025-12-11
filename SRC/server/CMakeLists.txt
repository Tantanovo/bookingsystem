set(SERVER_SRC
    listen.cpp
    server.cpp
    server.hpp
)

add_executable(server ${SERVER_SRC})
target_link_libraries(server event)