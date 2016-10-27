MESSAGE(STATUS "\n Checking ${CMAKE_CURRENT_SOURCE_DIR}\n")
SET(BINARY_NAME "TivaTest")

#------------------------------
# Resolve Dependencies
#------------------------------

find_package(TivaWare COMPONENTS driverlib drivers utils
  CONFIG REQUIRED)
find_package(FreeRTOS CONFIG REQUIRED)


SET(MODULE_DEPS "")


#------------------------------
# sources
#------------------------------
set(SRCS
  main.c
  tm4c129encpdt_startup_gcc.c
  )

#------------------------------
# Output executable
#------------------------------
add_executable(${BINARY_NAME}  ${SRCS} )

# Link the binary
target_link_libraries( ${BINARY_NAME}
#  TivaWare::driverlib
  TivaWare::drivers
  TivaWare::utils
  freertos
  )

set_target_properties(${BINARY_NAME}
  PROPERTIES
  COMPILE_FLAGS " -nostartfiles -fno-rtti"
  COMPILE_OPTIONS "-fexceptions" 
  LINK_FLAGS
	" -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/tm4c129encpdt.lds \
        -mcpu=cortex-m4 -mthumb -Wl,-M=symbols.map,--gc-sections"
  )




MESSAGE(STATUS "** end CMakeLists.txt **\n")
