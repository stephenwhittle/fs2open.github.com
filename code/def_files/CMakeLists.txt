add_library(def_files STATIC def_files/def_files.h)

target_include_directories(def_files PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if (MSVC)
target_sources(def_files PRIVATE def_files/def_files-win32.cpp)
else()
target_sources(def_files PRIVATE def_files/def_files-generic.cpp)
endif()


target_link_libraries(def_files PUBLIC core)

macro(add_default_files)
    list (APPEND default_files ${ARGV})
    set (default_files "${default_files}" PARENT_SCOPE)
endmacro()


set(default_files def_files/ai_profiles.tbl)

add_subdirectory(def_files/data)


IF(MSVC)
	# Windows specific version using resource files

	set(RES_CONTENT)
	set(ARRAY_ELEMENTS)

	FOREACH(file ${default_files})
		set(INPUT_NAME "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
		SET(OUTPUT "${GENERATED_SOURCE_DIR}/code/${file}")
		file(RELATIVE_PATH RELATIVE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/def_files" "${INPUT_NAME}")


		# Retrieve the path type name from the relative path
		GET_FILENAME_COMPONENT(PATH_TYPE "${RELATIVE_FILE_PATH}" DIRECTORY)
		file(TO_NATIVE_PATH "${PATH_TYPE}" PATH_TYPE)
		string(REPLACE "\\" "\\\\" PATH_TYPE "${PATH_TYPE}")
		GET_FILENAME_COMPONENT(FILE_NAME "${RELATIVE_FILE_PATH}" NAME)

		string(MAKE_C_IDENTIFIER "${RELATIVE_FILE_PATH}" FIELD_NAME)

		set(FIELD_NAME "Default_${FIELD_NAME}")
		string(TOUPPER "${FIELD_NAME}" FIELD_NAME)

		set(ARRAY_ELEMENTS "${ARRAY_ELEMENTS}\n\t{ \"${PATH_TYPE}\", \"${FILE_NAME}\" , TEXT(\"${FIELD_NAME}\") },")

		set(RES_CONTENT "${RES_CONTENT}\n${FIELD_NAME} RCDATA \"${INPUT_NAME}\"")
	ENDFOREACH(file)

	file(WRITE "${GENERATED_SOURCE_DIR}/code/default_files.rc" "${RES_CONTENT}")

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/def_files/generated_def_files-win32.inc.in" "${GENERATED_SOURCE_DIR}/code/def_files/generated_def_files-win32.inc")
	target_sources(def_files PUBLIC "${GENERATED_SOURCE_DIR}/code/def_files/generated_def_files-win32.inc")
	target_include_directories(def_files PUBLIC "${GENERATED_SOURCE_DIR}/code")
	target_sources(def_files INTERFACE "${GENERATED_SOURCE_DIR}/code/default_files.rc")
else()
	# Generic version using embedfile
	SET(DEF_OUT_FILES)

	set(INCLUDE_LIST)
	set(ARRAY_ELEMENTS)

	FOREACH(file ${default_files})
		set(INPUT_NAME "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
		SET(OUTPUT "${GENERATED_SOURCE_DIR}/code/${file}")
		file(RELATIVE_PATH RELATIVE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/def_files" "${INPUT_NAME}")

		# For some reason this is needed...
		GET_FILENAME_COMPONENT(DIRECTORY_PATH ${OUTPUT} DIRECTORY)
		FILE(MAKE_DIRECTORY ${DIRECTORY_PATH})

		# Retrieve the path type name from the relative path
		GET_FILENAME_COMPONENT(PATH_TYPE "${RELATIVE_FILE_PATH}" DIRECTORY)
		file(TO_NATIVE_PATH "${PATH_TYPE}" PATH_TYPE)
		if (MINGW)
			# There is a bug in CMake where it thinks MinGW uses forward slashes for paths but we need backslashes for Windows builds
			string(REPLACE "/" "\\" PATH_TYPE "${PATH_TYPE}")
		endif()
		# Properly escape back slashes in strings
		string(REPLACE "\\" "\\\\" PATH_TYPE "${PATH_TYPE}")

		GET_FILENAME_COMPONENT(FILE_NAME "${RELATIVE_FILE_PATH}" NAME)

		string(MAKE_C_IDENTIFIER "${RELATIVE_FILE_PATH}" FIELD_NAME)

		set(HEADER_FILE "${OUTPUT}.h")
		set(SOURCE_FILE "${OUTPUT}.cpp")

		set_source_files_properties("${HEADER_FILE}" "${SOURCE_FILE}" PROPERTIES GENERATED TRUE)

		set(ALL_OUTPUTS "${HEADER_FILE}" "${SOURCE_FILE}")
		set(FIELD_NAME "Default_${FIELD_NAME}")

		set(INCLUDE_LIST "${INCLUDE_LIST}\n#include \"${file}.h\"")
		set(ARRAY_ELEMENTS "${ARRAY_ELEMENTS}\n\t{ \"${PATH_TYPE}\", \"${FILE_NAME}\" , ${FIELD_NAME} , ${FIELD_NAME}_size },")

		ADD_CUSTOM_COMMAND(
			OUTPUT ${ALL_OUTPUTS}
			COMMAND embedfile "${INPUT_NAME}" "${OUTPUT}" "${FIELD_NAME}"
			MAIN_DEPENDENCY "${INPUT_NAME}"
			DEPENDS embedfile
			COMMENT "Generating string file for ${INPUT_NAME}"
			)

		LIST(APPEND DEF_OUT_FILES ${ALL_OUTPUTS})
	ENDFOREACH(file)

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/def_files/generated_def_files-generic.inc.in" "${GENERATED_SOURCE_DIR}/code/def_files/generated_def_files-generic.inc")
	target_sources(def_files PRIVATE ${DEF_OUT_FILES})
	target_sources(def_files public ${GENERATED_SOURCE_DIR}/code/def_files/generated_def_files-generic.inc)
	target_include_directories(def_files public ${GENERATED_SOURCE_DIR}/code)
	SOURCE_GROUP("Generated Files\\Default Files" FILES ${DEF_OUT_FILES})

endif()