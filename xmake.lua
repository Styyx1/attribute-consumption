includes("lib/commonlibsse")

set_project("styyx-adjustable-attributes")
set_version("2.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

add_rules("mode.debug", "mode.releasedbg")

set_config("commonlib_toml", true)

target("styyx-adjustable-attributes")
    add_rules("commonlibsse.plugin", {
        name = "styyx-adjustable-attributes",
        author = "styyx",
        description = "Change how attributes are consumed"
    })
    add_files("src/main.cpp")
    set_pcxxheader("src/pch.h")
