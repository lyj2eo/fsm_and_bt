# add_executable(bt_06_hcr_contact_autonomy main.cpp)
add_executable(bt_06_hcr_contact_autonomy
    main.cpp
    observer/observer.cpp
    identifier/contact_identifier.cpp
    supervisor/contact_supervisor.cpp
)
target_link_libraries(bt_06_hcr_contact_autonomy PRIVATE btlib)
