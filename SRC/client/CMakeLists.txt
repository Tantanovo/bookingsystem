set(CLIENT_SRC
    client.cpp
    client.hpp
    connect.cpp
)
add_executable(client ${CLIENT_SRC})
target_link_libraries(client event)
