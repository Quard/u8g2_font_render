cmake_minimum_required(VERSION 3.5)
project(u8g2_font_render LANGUAGES C VERSION 0.1)

include(GNUInstallDirs)

add_library(u8g2_font_render u8g2_font_render.c u8g2_font_render.h)
target_include_directories(u8g2_font_render PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR>
)

# Install via make install / cmake --install
install(TARGETS u8g2_font_render EXPORT u8g2_font_render_config
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT u8g2_font_render_config
	DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/u8g2_font_render"
)
