add_executable(${PROJECT_NAME}  
    main.cxx
    
    server.cxx server.hxx
    ping.cxx
    check.cxx)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OSTREE
    DBUS
    GLIB)