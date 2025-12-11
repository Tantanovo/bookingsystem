set(CLIENT_SRC
    connect.cpp
    client.hpp
    client.cpp
)

add_executable(client ${CLIENT_SRC})
target_link_libraries(client event)
