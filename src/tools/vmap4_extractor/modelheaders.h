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

#ifndef MODELHEADERS_H
#define MODELHEADERS_H

/* typedef unsigned char uint8;
using int8 = char;
using uint16 = unsigned short;
using int16 = short;
using uint32 = unsigned int;
typedef int int32; */

#pragma pack(push,1)

struct ModelHeader
{
    char id[4];
    uint8 version[4];
    uint32 nameLength;
    uint32 nameOfs;
    uint32 type;
    uint32 nGlobalSequences;
    uint32 ofsGlobalSequences;
    uint32 nAnimations;
    uint32 ofsAnimations;
    uint32 nAnimationLookup;
    uint32 ofsAnimationLookup;
    uint32 nBones;
    uint32 ofsBones;
    uint32 nKeyBoneLookup;
    uint32 ofsKeyBoneLookup;
    uint32 nVertices;
    uint32 ofsVertices;
    uint32 nViews;
    uint32 nColors;
    uint32 ofsColors;
    uint32 nTextures;
    uint32 ofsTextures;
    uint32 nTransparency;
    uint32 ofsTransparency;
    uint32 nTextureanimations;
    uint32 ofsTextureanimations;
    uint32 nTexReplace;
    uint32 ofsTexReplace;
    uint32 nRenderFlags;
    uint32 ofsRenderFlags;
    uint32 nBoneLookupTable;
    uint32 ofsBoneLookupTable;
    uint32 nTexLookup;
    uint32 ofsTexLookup;
    uint32 nTexUnits;
    uint32 ofsTexUnits;
    uint32 nTransLookup;
    uint32 ofsTransLookup;
    uint32 nTexAnimLookup;
    uint32 ofsTexAnimLookup;
    float floats[14];
    uint32 nBoundingTriangles;
    uint32 ofsBoundingTriangles;
    uint32 nBoundingVertices;
    uint32 ofsBoundingVertices;
    uint32 nBoundingNormals;
    uint32 ofsBoundingNormals;
    uint32 nAttachments;
    uint32 ofsAttachments;
    uint32 nAttachLookup;
    uint32 ofsAttachLookup;
    uint32 nAttachments_2;
    uint32 ofsAttachments_2;
    uint32 nLights;
    uint32 ofsLights;
    uint32 nCameras;
    uint32 ofsCameras;
    uint32 nCameraLookup;
    uint32 ofsCameraLookup;
    uint32 nRibbonEmitters;
    uint32 ofsRibbonEmitters;
    uint32 nParticleEmitters;
    uint32 ofsParticleEmitters;
};

#pragma pack(pop)
#endif
