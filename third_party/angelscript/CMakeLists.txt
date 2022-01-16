cmake_minimum_required(VERSION 3.15)
project(angelscript)
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        # Most important header
        ${CMAKE_CURRENT_LIST_DIR}/include/angelscript.h

        # Core headers
        ${CMAKE_CURRENT_LIST_DIR}/source/as_array.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_builder.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_bytecode.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_compiler.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_config.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_configgroup.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_context.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_criticalsection.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_datatype.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_debug.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_generic.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_map.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_memory.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_module.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_objecttype.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_outputbuffer.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_parser.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_property.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_restore.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptcode.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptengine.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptfunction.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptnode.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptobject.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_string.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_string_util.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_texts.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_thread.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_tokendef.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_tokenizer.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_typeinfo.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_variablescope.h
        ${CMAKE_CURRENT_LIST_DIR}/source/as_atomic.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_builder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_bytecode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc_mips.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc_x86.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc_x64_gcc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc_x64_msvc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc_x64_msvc_asm.asm
        ${CMAKE_CURRENT_LIST_DIR}/source/as_callfunc_x64_mingw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_compiler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_configgroup.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_context.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_datatype.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_gc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_generic.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_globalproperty.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_memory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_module.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_objecttype.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_outputbuffer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_parser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_restore.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptcode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptengine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptfunction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptnode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_scriptobject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_string.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_string_util.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_thread.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_tokenizer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_typeinfo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/as_variablescope.cpp

        # Addons
        ${CMAKE_CURRENT_LIST_DIR}/addons/datetime/datetime.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/datetime/datetime.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptany/scriptany.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptany/scriptany.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptarray/scriptarray.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptarray/scriptarray.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptbuilder/scriptbuilder.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptbuilder/scriptbuilder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptdictionary/scriptdictionary.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptdictionary/scriptdictionary.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptgrid/scriptgrid.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptgrid/scriptgrid.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scripthandle/scripthandle.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scripthandle/scripthandle.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptmath/scriptmath.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptmath/scriptmath.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptmath/scriptmathcomplex.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptmath/scriptmathcomplex.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptstdstring/scriptstdstring.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptstdstring/scriptstdstring.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/scriptstdstring/scriptstdstring_utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/addons/weakref/weakref.h
        ${CMAKE_CURRENT_LIST_DIR}/addons/weakref/weakref.cpp)

#if(MSVC)
#    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/source/as_callfunc_x64_msvc_asm.asm PROPERTIES COMPILE_FLAGS /c /nologo /Fo$(OutDir)\\as_callfunc_x64_msvc_asm.obj /W3 /Zi /Ta $(InputDir)\$(InputFileName))
#endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/addons)
