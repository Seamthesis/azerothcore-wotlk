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

#ifndef MPQ_H
#define MPQ_H

#include "libmpq/mpq.h"
#include "loadlib/loadlib.h"
#include <cstring>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

// cppcheck-suppress ctuOneDefinitionRuleViolation
class MPQArchive
{
public:
    mpq_archive_s* mpq_a;

    MPQArchive(const char* filename);
    ~MPQArchive() { close(); }
    void close();

    void GetFileListTo(vector<string>& filelist)
    {
        uint32_t filenum;
        if (libmpq__file_number(mpq_a, "(listfile)", &filenum)) return;
        libmpq__off_t size, transferred;
        libmpq__file_unpacked_size(mpq_a, filenum, &size);

        char* buffer = new char[size + 1];
        buffer[size] = '\0';

        libmpq__file_read(mpq_a, filenum, (unsigned char*)buffer, size, &transferred);

        char seps[] = "\n";
        char* token;

        token = strtok( buffer, seps );
        uint32 counter = 0;
        while ((token != nullptr) && (counter < size))
        {
            //cout << token << endl;
            token[strlen(token) - 1] = 0;
            string s = token;
            filelist.push_back(s);
            counter += strlen(token) + 2;
            token = strtok(nullptr, seps);
        }

        delete[] buffer;
    }
};
using ArchiveSet = std::deque<MPQArchive*>;
// cppcheck-suppress ctuOneDefinitionRuleViolation
class MPQFile
{
    //MPQHANDLE handle;
    bool eof;
    char* buffer;
    libmpq__off_t pointer, size;

    // disable copying
    MPQFile(const MPQFile& /*f*/) {}
    void operator=(const MPQFile& /*f*/) {}

public:
    MPQFile(const char* filename);    // filenames are not case sensitive
    ~MPQFile() { close(); }
    std::size_t read(void* dest, std::size_t bytes);
    std::size_t getSize() { return size; }
    std::size_t getPos() { return pointer; }
    char* getBuffer() { return buffer; }
    char* getPointer() { return buffer + pointer; }
    bool isEof() { return eof; }
    void seek(int offset);
    void seekRelative(int offset);
    void close();
};

inline void flipcc(char* fcc)
{
    char t;
    t = fcc[0];
    fcc[0] = fcc[3];
    fcc[3] = t;
    t = fcc[1];
    fcc[1] = fcc[2];
    fcc[2] = t;
}

#endif
