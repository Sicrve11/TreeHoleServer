
add_executable(StressTest StressTest.cpp ../SkipList.cpp ../TreeHole.cpp)
target_link_libraries(StressTest libserver_base)
