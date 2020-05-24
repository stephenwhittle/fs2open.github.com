add_library(utils STATIC utils/unicode.h
                            utils/unicode.cpp
                            utils/tuples.h
                            utils/strings.h
                            utils/string_utils.h
                            utils/string_utils.cpp
                            utils/id.h
                            utils/HeapAllocator.h
                            utils/HeapAllocator.cpp
                            utils/event.h
                            utils/encoding.h
                            utils/encoding.cpp
                            utils/boost/hash_combine.h
                            utils/boost/syncboundedqueue.h
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(utils PUBLIC core utfcpp )