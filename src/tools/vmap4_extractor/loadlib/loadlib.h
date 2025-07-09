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

#ifndef LOAD_LIB_H
#define LOAD_LIB_H

#include <string>

#ifdef WIN32
using int64 = __int64;
using int32 = __int32;
using int16 = __int16;
using int8 = __int8;
using uint64 = unsigned __int64;
using uint32 = unsigned __int32;
using uint16 = unsigned __int16;
using uint8 = unsigned __int8;
#else
#include <cstdint>
#ifndef uint64_t
#ifdef __linux__
#include <linux/types.h>
#endif
#endif
using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;
using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;
#endif

#define FILE_FORMAT_VERSION    18

//
// File version chunk
//
struct file_MVER
{
    union
    {
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
    uint32 ver;
};

class FileLoader
{
    uint8*  data;
    uint32  data_size;
public:
    virtual bool prepareLoadedData();
    uint8* GetData()     {return data;}
    uint32 GetDataSize() {return data_size;}

    file_MVER* version;
    FileLoader();
    ~FileLoader();
    bool loadFile(std::string const& filename, bool log = true);
    virtual void free();
};
#endif
