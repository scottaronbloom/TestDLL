set(qtproject_SRCS
    main.cpp    
    wrapper.cpp
)

set(qtproject_H
)

set(project_H
    wrapper.h
)

IF( WIN32 )
    set(qtproject_SRCS ${qtproject_SRCS} wrapper_win.cpp)
    set(project_H ${project_H} wrapper_win.h)
ELSE()
    set(qtproject_SRCS ${qtproject_SRCS} wrapper_linux.cpp)
    set(project_H ${project_H} wrapper_linux.h)
ENDIF()

set(qtproject_UIS
)


set(qtproject_QRC
)
