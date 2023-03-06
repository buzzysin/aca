function(capitalise result str)
  string(SUBSTRING ${str} 0 1 first)
  string(TOUPPER ${first} first)
  string(SUBSTRING ${str} 1 -1 rest)
  set(${result} "${first}${rest}" PARENT_SCOPE)
endfunction()

if (MODULE_NAME)
  message(STATUS "config.cmake :: ${MODULE_NAME} :: Entering ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src)
    set(MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    file(GLOB MODULE_SRC_LOCAL ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
  else()
    unset(MODULE_SRC)
    unset(MODULE_SRC_LOCAL)
  endif()

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(MODULE_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  else()
    unset(MODULE_INC)
  endif()

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set(MODULE_TEST ${CMAKE_CURRENT_SOURCE_DIR}/test)
  else()
    unset(MODULE_TEST)
  endif()

  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/apps)
    set(MODULE_APP ${CMAKE_CURRENT_SOURCE_DIR}/apps)
  else()
    unset(MODULE_APP)
  endif()

  message(STATUS "config.cmake :: ${MODULE_NAME} :: Sources ${MODULE_SRC}")
  message(STATUS "config.cmake :: ${MODULE_NAME} :: Includes ${MODULE_INC}")
  message(STATUS "config.cmake :: ${MODULE_NAME} :: Tests ${MODULE_TEST}")
  message(STATUS "config.cmake :: ${MODULE_NAME} :: Applications ${MODULE_APP}")

  if (MODULE_SRC AND MODULE_SUBDIRS)
    message(STATUS "config.cmake :: ${MODULE_NAME} :: Initialising subdirs ...")

    foreach(SUBDIR ${MODULE_SUBDIRS})
      capitalise(SUBDIR_MODULE_NAME ${SUBDIR})
      
      if (EXISTS ${MODULE_SRC}/${SUBDIR}/CMakeLists.txt)
        message(STATUS "config.cmake :: ${MODULE_NAME} :: Got subdirectory ${SUBDIR_MODULE_NAME}")
        add_subdirectory(src/${SUBDIR})
        list(APPEND MODULE_OBJECTS $<TARGET_OBJECTS:${SUBDIR_MODULE_NAME}>)
      else()
        message(STATUS "\t\tWARNING :: Subdirectory ${MODULE_SRC}/${SUBDIR_MODULE_NAME} does not have a CMakeLists.txt file")
      endif()

    endforeach()

    message(STATUS "config.cmake :: ${MODULE_NAME} :: Adding sources ${MODULE_OBJECTS}")
    add_library(${MODULE_NAME} INTERFACE ${MODULE_OBJECTS})

    message(STATUS "config.cmake :: ${MODULE_NAME} :: Creating alias ${PROJECT_NAME}::${MODULE_NAME}")
    add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

    foreach(MODULE_DEP ${MODULE_DEPS})
      message(STATUS "config.cmake :: ${MODULE_NAME} :: Adding dependency ${MODULE_DEP} to module ${MODULE_NAME}")
      target_link_libraries(${MODULE_NAME} INTERFACE ${MODULE_DEP})
    endforeach()
    
  elseif(MODULE_SRC)
    message(STATUS "config.cmake :: ${MODULE_NAME} :: Configuring library module ${MODULE_NAME} sources")

    message(STATUS "config.cmake :: ${MODULE_NAME} :: Adding sources ${MODULE_SRC_LOCAL}")
    add_library(${MODULE_NAME} OBJECT ${MODULE_SRC_LOCAL})

    message(STATUS "config.cmake :: ${MODULE_NAME} :: Creating alias ${PROJECT_NAME}::${MODULE_NAME}")
    add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})
  endif()
    
  if(MODULE_APP AND MODULE_APPS)
    message(STATUS "config.cmake :: ${MODULE_NAME} :: Configuring application module ${MODULE_NAME} sources")
    
    foreach(APP ${MODULE_APPS})
      set(APP_NAME ${APP})
      message(STATUS "config.cmake :: ${MODULE_NAME} :: Configuring application ${APP_NAME}")
      add_executable(${APP_NAME} ${MODULE_APP}/${APP}/main.cpp)
      target_link_libraries(${APP_NAME} ${MODULE_NAME})
      target_include_directories(${APP_NAME} PRIVATE ${MODULE_INC})
    endforeach()
  endif()
else()
  message(FATAL_ERROR "MODULE_NAME is not defined")
endif()