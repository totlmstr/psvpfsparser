cmake_minimum_required(VERSION 3.2)
project(psvpfsparser)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_EXAMPLES "Build project examples" ON)

FILE(GLOB F00D_FILES "IF00DKeyEncryptor.h"
                     "F00DFileKeyEncryptor.h"
                     "F00DFileKeyEncryptor.cpp"
                     "F00DKeyEncryptorFactory.h"
                     "F00DKeyEncryptorFactory.cpp"
                     "F00DNativeKeyEncryptor.h"
                     "F00DNativeKeyEncryptor.cpp"
                     )
source_group("F00D Files" FILES ${F00D_FILES})

FILE(GLOB CRYPTO_FILES "ICryptoOperations.h"
                       "LibTomCryptCryptoOperations.h"
                       "LibTomCryptCryptoOperations.cpp"
                       "CryptoOperationsFactory.h"
                       "CryptoOperationsFactory.cpp"
                       )
source_group("Crypto Files" FILES ${CRYPTO_FILES})

FILE(GLOB HEADER_FILES "FilesDbParser.h"
                       "HashTree.h"
                       "NodeIcvCalculator.h"
                       "PfsCryptEngine.h"
                       "PfsCryptEngineBase.h"
                       "PfsCryptEngineSelectors.h"
                       "PfsKeyGenerator.h"
                       "PfsKeys.h"
                       "PsvPfsParserConfig.h"
                       "SceKernelUtilsForDriver.h"
                       "SceSblSsMgrForDriver.h"
                       "SecretGenerator.h"
                       "UnicvDbParser.h"
                       "UnicvDbTypes.h"
                       "UnicvDbUtils.h"
                       "Utils.h"
                       "LocalKeyGenerator.h"
                       "LocalKeys.h"
                       "MerkleTree.hpp"
                       "IcvPrimitives.h"
                       "FlagOperations.h"
                       "PfsPageMapper.h"
                       "PfsFilesystem.h"
                       "PfsFile.h"
                       "rif2zrif.h"
                       )
source_group("Header Files" FILES ${HEADER_FILES})

FILE(GLOB SOURCE_FILES "FilesDbParser.cpp"
                       "HashTree.cpp"
                       "NodeIcvCalculator.cpp"
                       "PfsCryptEngine.cpp"
                       "PfsCryptEngineBase.cpp"
                       "PfsCryptEngineSelectors.cpp"
                       "PfsKeyGenerator.cpp"
                       "PfsKeys.cpp"
                       "PsvPfsParserConfig.cpp"
                       "SceKernelUtilsForDriver.cpp"
                       "SceSblSsMgrForDriver.cpp"
                       "SecretGenerator.cpp"
                       "UnicvDbParser.cpp"
                       "UnicvDbTypes.cpp"
                       "UnicvDbUtils.cpp"
                       "Utils.cpp"
                       "LocalKeyGenerator.cpp"
                       "LocalKeys.cpp"
                       "IcvPrimitives.cpp"
                       "FlagOperations.cpp"
                       "PfsPageMapper.cpp"
                       "PfsFilesystem.cpp"
                       "PfsFile.cpp"
                       "rif2zrif.cpp"
                       )
source_group("Source Files" FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES} ${F00D_FILES} ${CRYPTO_FILES})

include("${PROJECT_SOURCE_DIR}/cmake/GetStandard.cmake")
get_standard_for_build()
if(${COMPILE_MODERN_CPP})
     if(WIN32)
          if(MSVC)
               add_definitions("/Zc:__cplusplus")
          endif()
          add_definitions("${STANDARD_FLAG}")
          add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
     else()
          list(APPEND CMAKE_CXX_FLAGS "${STANDARD_FLAG}")
          target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
     endif()

     if(${CPP17_SUPPORTED})
          set(FS_INCLUDE_HEADER "filesystem")
          set(FS_NAMESPACE "std")
     else()
          set(FS_INCLUDE_HEADER "experimental/filesystem")
          set(FS_NAMESPACE "std::experimental")
     endif()
else()
     if(${BUILD_EXAMPLES})
          find_package(Boost 1.60 REQUIRED COMPONENTS filesystem program_options)
     else()
          find_package(Boost 1.60 REQUIRED COMPONENTS filesystem)
     endif()

     set(FS_INCLUDE_HEADER "boost/filesystem.hpp")
     set(FS_NAMESPACE "boost")

     target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} libzRIF libtomcrypt)
endif()

configure_file(LocalFilesystem.h.in LocalFilesystem.h @ONLY)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

if(${BUILD_EXAMPLES} AND (${BOOST_FOUND}))
     add_executable(psvpfsparserconfig MACOSX_BUNDLE "examples/psvpfsparser.cpp")
     target_include_directories(psvpfsparserconfig PUBLIC "${PROJECT_SOURCE_DIR}/examples")
     target_link_libraries(psvpfsparserconfig PRIVATE psvpfsparser)
endif()
