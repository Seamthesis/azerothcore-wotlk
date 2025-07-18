/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ACORE_DEFINE_H
#define ACORE_DEFINE_H

#include "CompilerDefs.h"
#include <cinttypes>
#include <climits>

#define ACORE_LITTLEENDIAN 0
#define ACORE_BIGENDIAN    1

#if !defined(ACORE_ENDIAN)
#  if defined (BOOST_BIG_ENDIAN)
#    define ACORE_ENDIAN ACORE_BIGENDIAN
#  else
#    define ACORE_ENDIAN ACORE_LITTLEENDIAN
#  endif
#endif

#if AC_PLATFORM == AC_PLATFORM_WINDOWS
#  define ACORE_PATH_MAX MAX_PATH
#  define _USE_MATH_DEFINES
#else //AC_PLATFORM != AC_PLATFORM_WINDOWS
#  define ACORE_PATH_MAX PATH_MAX
#endif //AC_PLATFORM

#if !defined(COREDEBUG)
#  define ACORE_INLINE inline
#else //COREDEBUG
#  if !defined(ACORE_DEBUG)
#    define ACORE_DEBUG
#  endif //ACORE_DEBUG
#  define ACORE_INLINE
#endif //!COREDEBUG

#if AC_COMPILER == AC_COMPILER_GNU
#  define ATTR_PRINTF(F, V) __attribute__ ((format (printf, F, V)))
#else //AC_COMPILER != AC_COMPILER_GNU
#  define ATTR_PRINTF(F, V)
#endif //AC_COMPILER == AC_COMPILER_GNU

#ifdef ACORE_API_USE_DYNAMIC_LINKING
#  if AC_COMPILER == AC_COMPILER_MICROSOFT
#    define AC_API_EXPORT __declspec(dllexport)
#    define AC_API_IMPORT __declspec(dllimport)
#  elif AC_COMPILER == AC_COMPILER_GNU
#    define AC_API_EXPORT __attribute__((visibility("default")))
#    define AC_API_IMPORT
#  else
#    error compiler not supported!
#  endif
#else
#  define AC_API_EXPORT
#  define AC_API_IMPORT
#endif

#ifdef ACORE_API_EXPORT_COMMON
#  define AC_COMMON_API AC_API_EXPORT
#else
#  define AC_COMMON_API AC_API_IMPORT
#endif

#ifdef ACORE_API_EXPORT_DATABASE
#  define AC_DATABASE_API AC_API_EXPORT
#else
#  define AC_DATABASE_API AC_API_IMPORT
#endif

#ifdef ACORE_API_EXPORT_SHARED
#  define AC_SHARED_API AC_API_EXPORT
#else
#  define AC_SHARED_API AC_API_IMPORT
#endif

#ifdef ACORE_API_EXPORT_GAME
#  define AC_GAME_API AC_API_EXPORT
#else
#  define AC_GAME_API AC_API_IMPORT
#endif

#define UI64LIT(N) UINT64_C(N)
#define SI64LIT(N) INT64_C(N)

#define STRING_VIEW_FMT_ARG(str) static_cast<int>((str).length()), (str).data()

using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;
using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8 = std::uint8_t;
#endif //ACORE_DEFINE_H
