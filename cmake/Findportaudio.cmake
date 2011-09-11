# - Try to find ImagePortaudio
# Once done, this will define
#
#  Portaudio_FOUND - system has Pulseaudio
#  Portaudio_INCLUDE_DIRS - the Pulseaudio include directories
#  Portaudio_LIBRARIES - link these to use Pulseaudio

include(LibFindMacros)

# Dependencies
#libfind_package(Portaudio Magick)

# Use pkg-config to get hints about paths
#libfind_pkg_check_modules(Portaudio_PKGCONF ImagePulseaudio)

# Include dir
find_path(Portaudio_INCLUDE_DIR
  NAMES portaudio.h
  PATH_SUFFIXES include
  PATHS 
  ../extern/portaudio
)

# Finally the library itself
find_library(Portaudio_LIBRARY
  NAMES portaudio portaudio_x86
  PATH_SUFFIXES
  lib
  build/msvc/Win32/Debug
  build/msvc/Win32/Release
  PATHS 
  ../extern/portaudio/
)

if(Portaudio_LIBRARY)
  get_filename_component(Portaudio_LIBRARY_DIR 
    ${Portaudio_LIBRARY} 
    PATH
  )
endif(Portaudio_LIBRARY)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Portaudio_PROCESS_INCLUDES Pulseaudio_INCLUDE_DIR)
set(Portaudio_PROCESS_LIBS Pulseaudio_LIBRARY)
libfind_process(Portaudio)
