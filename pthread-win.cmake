if (WIN32)
file(DOWNLOAD
	"ftp://sourceware.org/pub/pthreads-win32/dll-latest/lib/x64/pthreadVC2.lib"
	"${EXTERNAL}/pthread-win/pthread.lib")


link_directories(${EXTERNAL}/pthread-win/)
add_library(pthread-win UNKNOWN IMPORTED)
set_target_properties(pthread-win PROPERTIES
        IMPORTED_LOCATION "${EXTERNAL}/pthread-win/pthread.lib"
)
endif (WIN32)