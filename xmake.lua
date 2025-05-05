add_rules("mode.debug", "mode.release")
set_encodings("utf-8")
set_warnings("all")
set_languages("cxx17")

-- if target:is_plat("linux", "macosx") and switches.THREADS == "1" then
--         target:add("syslinks", "pthread")
-- end
target("test")
    set_kind("static")
    add_defines(string.format("__XMAKE__=\"%s\"", os.scriptdir():gsub("\\", "/")))
    add_files("learn/test.cpp")

target("learn")
    set_kind("binary")
    add_deps("test")
    add_files("learn/learn.cpp")

target("summary")
    set_kind("binary")
    add_deps("test")
    add_files("learn/summary.cpp")
    add_syslinks("pthread")

