ADD_SUBDIRECTORY(backgrounds)
ADD_SUBDIRECTORY(locale)
ADD_SUBDIRECTORY(presets)
ADD_SUBDIRECTORY(projects)
ADD_SUBDIRECTORY(samples)
ADD_SUBDIRECTORY(themes)

IF(LMMS_BUILD_LINUX)
   INSTALL(FILES themes/default/icon.png DESTINATION "${DATA_DIR}/pixmaps" RENAME lmms.png)
   INSTALL(FILES lmms DESTINATION "${DATA_DIR}/menu")
   INSTALL(FILES lmms.desktop DESTINATION "${DATA_DIR}/applications")
   INSTALL(FILES lmms.xml DESTINATION "${DATA_DIR}/mime/packages")
ENDIF(LMMS_BUILD_LINUX)

IF(LMMS_BUILD_WIN32)
   FILE(GLOB RAWWAVES "${CMAKE_INSTALL_PREFIX}/share/stk/rawwaves/*.raw")
   INSTALL(FILES ${RAWWAVES} DESTINATION "${DATA_DIR}/stk/rawwaves")
ENDIF(LMMS_BUILD_WIN32)

