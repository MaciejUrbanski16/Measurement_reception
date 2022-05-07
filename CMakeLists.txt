cmake_minimum_required(VERSION 3.15)
project(server_for_tracker)

set(CMAKE_CXX_STANDARD 20)

set(SRC_FILES
        main.cpp mathplot/mathplot.h mathplot/mathplot.cpp
        RemoteDataInterpreter.cpp RemoteDataInterpreter.h PseudoServerContext.cpp PseudoServerContext.h RemoteDataHandler.cpp RemoteDataHandler.h VelocityCalculator.cpp VelocityCalculator.h RelativePositionCalculator.cpp RelativePositionCalculator.h TimeFormatter.cpp TimeFormatter.h ManualMeasurements.h CsvWriter.cpp CsvWriter.h CsvReader.cpp CsvReader.h)

include_directories(
        includes
        D:/wxWidgets-3.1.5_installation
        D:/wxWidgets-3.1.5_installation/include
        D:/wxWidgets-3.1.5_installation/lib/gcc_dll/mswu
)

link_directories(
        D:/wxWidgets-3.1.5_installation/lib/gcc_dll
        D:/wxWidgets-3.1.5_installation/lib/gcc_dll/mswu
)

set(
        WX_LIBS
        libwxmsw31u_propgrid.a
        libwxmsw31u_ribbon.a
        libwxmsw31u_stc.a
        libwxmsw31u_webview.a
        libwxmsw31u_gl.a
        libwxmsw31u_richtext.a
        libwxmsw31u_xrc.a
        libwxmsw31u_aui.a
        libwxmsw31u_html.a
        libwxmsw31u_adv.a
        libwxmsw31u_core.a
        libwxbase31u_xml.a
        libwxbase31u_net.a
        libwxbase31u.a
        libwxscintilla.a
        libwxtiff.a
        libwxjpeg.a
        libwxpng.a
        libwxzlib.a
        libwxregexu.a
        libwxexpat.a
        libwxmsw31u_media.a
)

find_package(Boost 1.77.0 REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(
        ${PROJECT_NAME}
        ${WX_LIBS}
        ws2_32
        wsock32
)
