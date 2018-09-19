CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES)
  
list(APPEND LOCAL_SOURCES
	"main_moq.cpp"
	"main_moq.h"
	"FreezeDetector.cpp"
	"FreezeDetector.h"
	"MoqAsyncAcceptor.cpp"
	"MoqAsyncAcceptor.h"
	"MoqWorldSocketMgr.cpp"
	"MoqWorldSocketMgr.h"
	"MoqSocketMgr.h")
list(APPEND EXTERNAL_SOURCES
	"../worldserver/CommandLine/CliRunnable.cpp"
	"../worldserver/CommandLine/CliRunnable.h"
	"../worldserver/RemoteAccess/RASession.cpp"
	"../worldserver/RemoteAccess/RASession.h"
	"../worldserver/TCSoap/TCSoap.cpp"
	"../worldserver/TCSoap/TCSoap.h")
list(APPEND PRIVATE_SOURCES ${EXTERNAL_SOURCES})

if( WIN32 )
  list(APPEND PRIVATE_SOURCES ${sources_windows})
endif()

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER PrecompiledHeaders/worldPCH.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} )

add_library(worldserverMoq 
	${PRVATE_SOURCES}
	${LOCAL_SOURCES}
	${EXTERNAL_SOURCES}
)
	
CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES)
target_include_directories(worldserverMoq
  PUBLIC
    # Provide the binary dir for all child targets
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(worldserverMoq
    trinity-core-interface
    scripts
	shared
    game
    gsoap
    readline)
