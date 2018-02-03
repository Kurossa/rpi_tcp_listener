project (pugixml)

add_library (pugixml 
	pugixml/pugixml.cpp
	pugixml/pugixml.hpp
	pugixml/pugiconfig.hpp)

target_include_directories (pugixml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (pugixml)
