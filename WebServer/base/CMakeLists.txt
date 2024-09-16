set(LIB_SRC
    AsyncLogging.cpp
    LogFile.cpp
    AppendFile.cpp
    LogStream.cpp
    Logger.cpp
    Thread.cpp
    CountDownLatch.cpp
)

add_library(libserver_base ${LIB_SRC})
target_link_libraries(libserver_base pthread rt)

set_target_properties(libserver_base PROPERTIES OUTPUT_NAME "server_base")

# add_subdirectory(test)