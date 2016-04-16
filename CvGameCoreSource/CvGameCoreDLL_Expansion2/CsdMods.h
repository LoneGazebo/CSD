// CsdMods.h
#pragma once

#ifndef CSD_MODS_H
#define CSD_MODS_H

#define MOD_DLL_VERSION_NUMBER ((uint) 51)

// Comment out this line to switch off all custom mod logging
#define CUSTOMLOGDEBUG "CustomMods.log"

// Comment these lines out to remove the associated code from the DLL,
#define MOD_BUGFIX_MINOR                            (true)

// Changes for the City State Diplomacy mod by Gazebo - AFFECTS SAVE GAME DATA FORMAT
#define MOD_DIPLOMACY_CITYSTATES                    (true)
#if defined(MOD_DIPLOMACY_CITYSTATES)
#define MOD_DIPLOMACY_CITYSTATES_QUESTS             (MOD_DIPLOMACY_CITYSTATES && (true))
#define MOD_DIPLOMACY_CITYSTATES_RESOLUTIONS        (MOD_DIPLOMACY_CITYSTATES && (true))
#define MOD_DIPLOMACY_CITYSTATES_HURRY              (MOD_DIPLOMACY_CITYSTATES && (true))
#endif


// Enables the Extensions API
#define MOD_API_EXTENSIONS                          (true)
// Enables the LUA Extensions API
#define MOD_API_LUA_EXTENSIONS                      (true)

// Removes free GP (from buildings, policies, traits, etc) from the GP counters (v61)
#define MOD_GLOBAL_TRULY_FREE_GP                    (true)


// Custom mod logger
#if defined(CUSTOMLOGDEBUG)
#define	CUSTOMLOG(sFmt, ...) {															\
	CvString sMsg;																		\
	CvString::format(sMsg, sFmt, __VA_ARGS__);											\
	LOGFILEMGR.GetLog(CUSTOMLOGDEBUG, FILogFile::kDontTimeStamp)->Msg(sMsg.c_str());	\
}
#else
#define	CUSTOMLOG(sFmt, ...) __noop
#endif


// GlobalDefines wrappers
#define GD_INT_DECL(name)       int m_i##name
#define GD_INT_DEF(name)        inline int get##name() { return m_i##name; }
#define GD_INT_INIT(name, def)  m_i##name(def)
#define GD_INT_CACHE(name)      m_i##name = getDefineINT(#name); CUSTOMLOG("<Defines>: %s = %i", #name, m_i##name)
#define GD_INT_GET(name)        GC.get##name()


// LUA API wrappers
#define LUAAPIEXTN(method, type, ...) static int l##method(lua_State* L)
#define LUAAPIIMPL(object, method) int CvLua##object::l##method(lua_State* L) { return BasicLuaMethod(L, &Cv##object::##method); }
#define LUAAPIINLINE(method, hasMethod, type) inline bool method() const { return hasMethod(type); }


// Serialization wrappers
#define MOD_SERIALIZE

#if defined(MOD_SERIALIZE)
#define MOD_SERIALIZE_INIT_READ(stream) uint uiDllSaveVersion; stream >> uiDllSaveVersion
#define MOD_SERIALIZE_READ(version, stream, member, def) if (uiDllSaveVersion >= version) { stream >> member; } else { member = def; }
#define MOD_SERIALIZE_READ_AUTO(version, stream, member, size, def)   \
	if (uiDllSaveVersion >= version) {                                \
		stream >> member;                                             \
	} else {                                                          \
		for (int iI = 0; iI < size; iI++) { member.setAt(iI, def); }  \
	}
#define MOD_SERIALIZE_READ_ARRAY(version, stream, member, type, size, def)	\
	if (uiDllSaveVersion >= version) {										\
		ArrayWrapper<type> wrapper(size, member); stream >> wrapper;		\
	} else {																\
		for (int iI = 0; iI < size; iI++) { (member)[iI] = def; }			\
	}
#define MOD_SERIALIZE_READ_HASH(version, stream, member, type, size, def)		\
	if (uiDllSaveVersion >= version) {											\
		CvInfosSerializationHelper::ReadHashedDataArray(stream, member, size);	\
	} else {																	\
		for (int iI = 0; iI < size; iI++) { (member)[iI] = def; }				\
	}
#define MOD_SERIALIZE_INIT_WRITE(stream) uint uiDllSaveVersion = MOD_DLL_VERSION_NUMBER; stream << uiDllSaveVersion
#define MOD_SERIALIZE_WRITE(stream, member) CvAssert(uiDllSaveVersion == MOD_DLL_VERSION_NUMBER); stream << member
#define MOD_SERIALIZE_WRITE_AUTO(stream, member) CvAssert(uiDllSaveVersion == MOD_DLL_VERSION_NUMBER); stream << member
#define MOD_SERIALIZE_WRITE_ARRAY(stream, member, type, size) CvAssert(uiDllSaveVersion == MOD_DLL_VERSION_NUMBER); stream << ArrayWrapper<type>(size, member)
#define MOD_SERIALIZE_WRITE_CONSTARRAY(stream, member, type, size) CvAssert(uiDllSaveVersion == MOD_DLL_VERSION_NUMBER); stream << ArrayWrapperConst<type>(size, member)
#define MOD_SERIALIZE_WRITE_HASH(stream, member, type, size, obj) CvAssert(uiDllSaveVersion == MOD_DLL_VERSION_NUMBER); CvInfosSerializationHelper::WriteHashedDataArray<obj, type>(stream, member, size)
#else
#define MOD_SERIALIZE_INIT_READ(stream) __noop
#define MOD_SERIALIZE_READ(version, stream, member, def) __noop
#define MOD_SERIALIZE_READ_AUTO(version, stream, member, size, def) __noop
#define MOD_SERIALIZE_READ_ARRAY(version, stream, member, type, size, def) __noop
#define MOD_SERIALIZE_READ_HASH(version, stream, member, type, size, def) __noop
#define MOD_SERIALIZE_INIT_WRITE(stream) __noop
#define MOD_SERIALIZE_WRITE(stream, member) __noop
#define MOD_SERIALIZE_WRITE_AUTO(stream, member) __noop
#define MOD_SERIALIZE_WRITE_ARRAY(stream, member, type, size) __noop
#define MOD_SERIALIZE_WRITE_ARRAYCONST(stream, member, type, size) __noop
#define MOD_SERIALIZE_WRITE_HASH(stream, member, type, size) __noop
#endif

#endif