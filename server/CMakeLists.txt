add_executable(unid_server)

target_sources(
  unid_server
  PRIVATE main.cpp
          network/udp_server.cpp
)

target_include_directories(unid_server PUBLIC include)

target_link_libraries(
  unid_server
  PUBLIC asio::asio
         Microsoft.GSL::GSL
         quill::quill
  PRIVATE common
)

message(STATUS "Quill library Search paths: ${quill_INCLUDE_DIRS}")
