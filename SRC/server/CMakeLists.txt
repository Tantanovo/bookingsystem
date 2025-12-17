set(
    SERVER_SRC
    listen.cpp
    server.cpp
    sermysql.cpp
)
find_package(jsoncpp REQUIRED)
add_executable(server ${SERVER_SRC})
target_link_libraries(server
    jsoncpp
    event
    mysqlclient
    pthread
)
