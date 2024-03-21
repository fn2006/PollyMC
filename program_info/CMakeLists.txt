if(UNIX)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_search_module(SCDOC scdoc)
        if(SCDOC_FOUND)
            pkg_get_variable(SCDOC_SCDOC scdoc scdoc)
        endif()
    endif()
endif()

set(Launcher_CommonName "PollyMC")
set(Launcher_DisplayName "PollyMC")

set(Launcher_Name "${Launcher_CommonName}" PARENT_SCOPE)
set(Launcher_DisplayName "${Launcher_DisplayName}" PARENT_SCOPE)

set(Launcher_Copyright "© 2022-2024 PollyMC Contributors\\n© 2022-2024 Prism Launcher Contributors\\n© 2021-2022 PolyMC Contributors\\n© 2012-2021 MultiMC Contributors")
set(Launcher_Copyright_Mac "© 2022-2024 PollyMC Contributors\\n© 2022-2024 Prism Launcher Contributors, © 2021-2022 PolyMC Contributors and © 2012-2021 MultiMC Contributors" PARENT_SCOPE)
set(Launcher_Copyright "${Launcher_Copyright}" PARENT_SCOPE)
set(Launcher_Domain "" PARENT_SCOPE)
set(Launcher_UserAgent "PrismLauncher/${Launcher_RELEASE_VERSION_NAME}" PARENT_SCOPE)
set(Launcher_ConfigFile "pollymc.cfg" PARENT_SCOPE)
set(Launcher_Git "https://github.com/fn2006/PollyMC" PARENT_SCOPE)
set(Launcher_DesktopFileName "org.fn2006.PollyMC.desktop" PARENT_SCOPE)
set(Launcher_SVGFileName "org.fn2006.PollyMC.svg" PARENT_SCOPE)

set(Launcher_Desktop "program_info/org.fn2006.PollyMC.desktop" PARENT_SCOPE)
set(Launcher_mrpack_MIMEInfo "program_info/modrinth-mrpack-mime.xml" PARENT_SCOPE)
set(Launcher_MetaInfo "program_info/org.fn2006.PollyMC.metainfo.xml" PARENT_SCOPE)
set(Launcher_SVG "program_info/org.fn2006.PollyMC.svg" PARENT_SCOPE)
set(Launcher_Branding_ICNS "program_info/prismlauncher.icns" PARENT_SCOPE)
set(Launcher_Branding_ICO "program_info/pollymc.ico")
set(Launcher_Branding_ICO "${Launcher_Branding_ICO}" PARENT_SCOPE)
set(Launcher_Branding_WindowsRC "program_info/pollymc.rc" PARENT_SCOPE)
set(Launcher_Branding_LogoQRC "program_info/prismlauncher.qrc" PARENT_SCOPE)

set(Launcher_Portable_File "program_info/portable.txt" PARENT_SCOPE)

configure_file(org.fn2006.PollyMC.desktop.in org.fn2006.PollyMC.desktop)
configure_file(org.fn2006.PollyMC.metainfo.xml.in org.fn2006.PollyMC.metainfo.xml)
configure_file(pollymc.rc.in pollymc.rc @ONLY)
configure_file(pollymc.manifest.in pollymc.manifest @ONLY)
configure_file(pollymc.ico pollymc.ico COPYONLY)
configure_file(win_install.nsi.in win_install.nsi @ONLY)

if(SCDOC_FOUND)
    set(in_scd "${CMAKE_CURRENT_SOURCE_DIR}/pollymc.6.scd")
    set(out_man "${CMAKE_CURRENT_BINARY_DIR}/pollymc.6")
    add_custom_command(
        DEPENDS "${in_scd}"
        OUTPUT "${out_man}"
        COMMAND ${SCDOC_SCDOC} < "${in_scd}" > "${out_man}"
    )
    add_custom_target(man ALL DEPENDS ${out_man})
    set(Launcher_ManPage "program_info/pollymc.6" PARENT_SCOPE)
endif()
