//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2015-2018 Fabian Greffrath
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//        [crispy] extended BSP tree formats
//

#include "m_bbox.h"
#include "p_local.h"
#include "i_swap.h"
#include "i_system.h"
#include "p_mapformat.h"
#include "w_wad.h"
#include "z_zone.h"

// [crispy] support maps with compressed ZDBSP nodes
#include "config.h"
#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

fixed_t GetOffset(vertex_t *v1, vertex_t *v2);

// [crispy] support maps with DeePBSP nodes
void P_LoadSegs_DeePBSPV4 (int lump)
{
    int i;
    mapseg_deepbspv4_t *data;

    numsegs = W_LumpLength(lump) / sizeof(mapseg_deepbspv4_t);
    segs = Z_Malloc(numsegs * sizeof(seg_t), PU_LEVEL, 0);
    data = (mapseg_deepbspv4_t *)W_CacheLumpNum(lump, PU_STATIC);

    for (i = 0; i < numsegs; i++)
    {
        seg_t *li = segs + i;
        mapseg_deepbspv4_t *ml = data + i;
        int side, linedef;
        line_t *ldef;
        int vn1, vn2;

        // [MB] 2020-04-30: Fix endianess for DeePBSP V4 nodes
        vn1 = LONG(ml->v1);
        vn2 = LONG(ml->v2);

        li->v1 = &vertexes[vn1];
        li->v2 = &vertexes[vn2];

        li->angle = (SHORT(ml->angle))<<FRACBITS;

    //  li->offset = (SHORT(ml->offset))<<FRACBITS; // [crispy] recalculated below
        linedef = (unsigned short)SHORT(ml->linedef);
        ldef = &lines[linedef];
        li->linedef = ldef;
        side = SHORT(ml->side);

        // e6y: check for wrong indexes
        if ((unsigned)linedef >= (unsigned)numlines)
        {
            I_Error("P_LoadSegs: seg %d references a non-existent linedef %d",
                i, (unsigned)linedef);
        }
        if ((unsigned)ldef->sidenum[side] >= (unsigned)numsides)
        {
            I_Error("P_LoadSegs: linedef %d for seg %d references a non-existent sidedef %d",
                linedef, i, (unsigned)ldef->sidenum[side]);
        }

        li->sidedef = &sides[ldef->sidenum[side]];
        li->frontsector = sides[ldef->sidenum[side]].sector;
        // [crispy] recalculate
        li->offset = GetOffset(li->v1, (ml->side ? ldef->v2 : ldef->v1));

        if (ldef->flags & ML_TWOSIDED)
            li->backsector = sides[ldef->sidenum[side ^ 1]].sector;
        else
            li->backsector = 0;
    }

    W_ReleaseLumpNum(lump);
}

// [crispy] support maps with DeePBSP nodes
// adapted from prboom-plus/src/p_setup.c:843-863
void P_LoadSubsectors_DeePBSPV4 (int lump)
{
    mapsubsector_deepbspv4_t *data;
    int i;

    numsubsectors = W_LumpLength(lump) / sizeof(mapsubsector_deepbspv4_t);
    subsectors = Z_Malloc(numsubsectors * sizeof(subsector_t), PU_LEVEL, 0);
    data = (mapsubsector_deepbspv4_t *)W_CacheLumpNum(lump, PU_STATIC);

    // [crispy] fail on missing subsectors
    if (!data || !numsubsectors)
        I_Error("P_LoadSubsectors: No subsectors in map!");

    for (i = 0; i < numsubsectors; i++)
    {
        // [MB] 2020-04-30: Fix endianess for DeePBSP V4 nodes
        subsectors[i].numlines = (unsigned short)SHORT(data[i].numsegs);
        subsectors[i].firstline = LONG(data[i].firstseg);
    }

    W_ReleaseLumpNum(lump);
}
// [crispy] support maps with DeePBSP nodes
// adapted from prboom-plus/src/p_setup.c:995-1038
void P_LoadNodes_DeePBSPV4 (int lump)
{
    const byte *data;
    int i;

    numnodes = (W_LumpLength (lump) - 8) / sizeof(mapnode_deepbspv4_t);
    nodes = Z_Malloc(numnodes * sizeof(node_t), PU_LEVEL, 0);
    data = W_CacheLumpNum (lump, PU_STATIC);

    // [crispy] warn about missing nodes
    if (!data || !numnodes)
    {
        if (numsubsectors == 1)
            fprintf(stderr, "P_LoadNodes_DeePBSPV4: No nodes in map, but only one subsector.\n");
        else
            I_Error("P_LoadNodes_DeePBSPV4: No nodes in map!");
    }

    // skip header
    data += 8;

    for (i = 0; i < numnodes; i++)
    {
        node_t *no = nodes + i;
        const mapnode_deepbspv4_t *mn = (const mapnode_deepbspv4_t *) data + i;
        int j;

        no->x = SHORT(mn->x)<<FRACBITS;
        no->y = SHORT(mn->y)<<FRACBITS;
        no->dx = SHORT(mn->dx)<<FRACBITS;
        no->dy = SHORT(mn->dy)<<FRACBITS;

        for (j = 0; j < 2; j++)
        {
            int k;
            // [MB] 2020-04-30: Fix endianess for DeePBSP V4 nodes
            no->children[j] = LONG(mn->children[j]);

            for (k = 0; k < 4; k++)
                no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
        }
    }

  W_ReleaseLumpNum(lump);
}

static void P_LoadSegs_XNOD(byte *data)
{
    for (int i = 0; i < numsegs; i++)
    {
        line_t *ldef;
        unsigned int linedef;
        unsigned char side;
        seg_t *li = segs + i;
        mapseg_xnod_t *ml = (mapseg_xnod_t *)data + i;
        unsigned int v1, v2;

        v1 = LONG(ml->v1);
        v2 = LONG(ml->v2);
        li->v1 = &vertexes[v1];
        li->v2 = &vertexes[v2];

        linedef = (unsigned short)SHORT(ml->linedef);
        ldef = &lines[linedef];
        li->linedef = ldef;
        side = ml->side ? 1 : 0;

        // e6y: check for wrong indexes
        if ((unsigned)linedef >= (unsigned)numlines)
        {
                I_Error("P_LoadSegs_XNOD: seg %d references a non-existent linedef %d",
                        i, (unsigned)linedef);
        }
        if ((unsigned)ldef->sidenum[side] >= (unsigned)numsides)
        {
                I_Error("P_LoadSegs_XNOD: linedef %d for seg %d references a non-existent sidedef %d",
                        linedef, i, (unsigned)ldef->sidenum[side]);
        }

        li->sidedef = &sides[ldef->sidenum[side]];
        li->frontsector = sides[ldef->sidenum[side]].sector;

        // seg angle and offset are not included
        li->angle = R_PointToAngle2(segs[i].v1->x, segs[i].v1->y, segs[i].v2->x, segs[i].v2->y);
        li->offset = GetOffset(li->v1, (ml->side ? ldef->v2 : ldef->v1));

        if (ldef->flags & ML_TWOSIDED)
            li->backsector = sides[ldef->sidenum[side ^ 1]].sector;
        else
            li->backsector = 0;
    }
}

static void P_LoadSegs_XGL(byte *data, bspformat_t format)
{
    int i, j;
    const mapseg_xgln_t *mln = (const mapseg_xgln_t *)data;
    const mapseg_xgl2_t *ml2 = (const mapseg_xgl2_t *)data;

    for (i = 0; i < numsubsectors; ++i)
    {
        for (j = 0; j < subsectors[i].numlines; ++j)
        {
            unsigned int v1;
            // unsigned int partner;
            unsigned int line;
            unsigned char side;
            seg_t *seg;

            if (format == NFMT_XGLN)
            {
                v1 = LONG(mln->vertex);
                // partner = LONG(mln->partner);
                line = (unsigned short)SHORT(mln->linedef);
                FIX_NO_INDEX(line);
                side = mln->side;
                mln++;
            }
            else
            {
                v1 = LONG(ml2->vertex);
                // partner = LONG(ml2->partner);
                line = (unsigned int)LONG(ml2->linedef);
                side = ml2->side;
                ml2++;
            }

            seg = &segs[subsectors[i].firstline + j];

            seg->v1 = &vertexes[v1];
            if (j == 0)
            {
                seg[subsectors[i].numlines - 1].v2 = seg->v1;
            }
            else
            {
                seg[-1].v2 = seg->v1;
            }

            if (line != 0xffffffff)
            {
                line_t *ldef;

                if ((unsigned int)line >= (unsigned int)numlines)
                {
                    I_Error("P_LoadSegs_XGL: seg %d, %d references a non-existent linedef %d",
                            i, j, (unsigned int)line);
                }

                ldef = &lines[line];
                seg->linedef = ldef;

                if (side != 0 && side != 1)
                {
                    I_Error("P_LoadSegs_XGL: seg %d, %d references a non-existent side %d",
                            i, j, (unsigned int)side);
                }

                if ((unsigned)ldef->sidenum[side] >= (unsigned)numsides)
                {
                    I_Error("P_LoadSegs_XGL: linedef %d for seg %d, %d references a non-existent sidedef %d",
                            line, i, j, (unsigned)ldef->sidenum[side]);
                }

                seg->sidedef = &sides[ldef->sidenum[side]];

                /* cph 2006/09/30 - our frontsector can be the second side of
                 * the linedef, so must check for NO_INDEX in case we are
                 * incorrectly referencing the back of a 1S line */
                if (ldef->sidenum[side] != NO_INDEX)
                {
                    seg->frontsector = sides[ldef->sidenum[side]].sector;
                }
                else
                {
                    seg->frontsector = 0;
                    fprintf(stderr, "P_LoadSegs_XGL: front of seg %d, %d has no sidedef", i, j);
                }

                if ((ldef->flags & ML_TWOSIDED) && (ldef->sidenum[side ^ 1] != NO_INDEX))
                {
                    seg->backsector = sides[ldef->sidenum[side ^ 1]].sector;
                }
                else
                {
                    seg->backsector = 0;
                }

                seg->offset = GetOffset(seg->v1, (side ? ldef->v2 : ldef->v1));
            }
            else
            {
                seg->angle = 0;
                seg->offset = 0;
                seg->linedef = NULL;
                seg->sidedef = NULL;
                seg->frontsector = segs[subsectors[i].firstline].frontsector;
                seg->backsector = seg->frontsector;
            }
        }

        // Need all vertices to be defined before setting angles
        for (j = 0; j < subsectors[i].numlines; ++j)
        {
            seg_t *seg = &segs[subsectors[i].firstline + j];
            if (seg->linedef)
            {
                seg->angle = R_PointToAngle2(seg->v1->x, seg->v1->y, seg->v2->x, seg->v2->y);
            }
        }
    }
}

void P_LoadZNodes_XNOD(mapnode_xnod_t *data)
{
    for (int i = 0; i < numnodes; i++)
    {
        node_t *no = nodes + i;
        mapnode_xnod_t *mn = data + i;

        no->x = SHORT(mn->x)<<FRACBITS;
        no->y = SHORT(mn->y)<<FRACBITS;
        no->dx = SHORT(mn->dx)<<FRACBITS;
        no->dy = SHORT(mn->dy)<<FRACBITS;

        for (int j = 0; j < 2; j++)
        {
            no->children[j] = LONG(mn->children[j]);
            for (int k = 0; k < 4; k++)
            {
                no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
            }
        }
    }
}

void P_LoadZNodes_XGL3(mapnode_xgl3_t *data)
{
    for (int i = 0; i < numnodes; i++)
    {
        node_t *no = nodes + i;
        mapnode_xgl3_t *mn = data + i;

        no->x = LONG(mn->x);
        no->y = LONG(mn->y);
        no->dx = LONG(mn->dx);
        no->dy = LONG(mn->dy);

        for (int j = 0; j < 2; j++)
        {
            no->children[j] = LONG(mn->children[j]);
            for (int k = 0; k < 4; k++)
            {
                no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
            }
        }
    }
}

// [crispy] support maps with compressed or uncompressed ZDBSP nodes
// adapted from prboom-plus/src/p_setup.c:1040-1331
// heavily modified, condensed and simplyfied
// - removed most paranoid checks, brought in line with Vanilla P_LoadNodes()
// - removed const type punning pointers
// - added support for compressed ZDBSP nodes
// - added support for flipped levels
// [MB] 2020-04-30: Fix endianess for ZDoom extended nodes
void P_LoadNodes_ZDBSP (int lump, mapformat_t format)
{
    byte *data;
    unsigned int i;
#ifdef HAVE_LIBZ
    byte *output = NULL;
#endif

    unsigned int orgVerts, newVerts;
    unsigned int numSubs, currSeg;
    unsigned int numSegs;
    unsigned int numNodes;
    vertex_t *newvertarray = NULL;

    data = W_CacheLumpNum(lump, PU_LEVEL);

    // 0. Uncompress nodes lump (or simply skip header)

    if (format.compressed)
    {
#ifdef HAVE_LIBZ
        const int len =  W_LumpLength(lump);
        int outlen, err;
        z_stream *zstream;

        // first estimate for compression rate:
        // output buffer size == 2.5 * input size
        outlen = 2.5 * len;
        output = Z_Malloc(outlen, PU_STATIC, 0);

        // initialize stream state for decompression
        zstream = malloc(sizeof(*zstream));
        memset(zstream, 0, sizeof(*zstream));
        zstream->next_in = data + 4;
        zstream->avail_in = len - 4;
        zstream->next_out = output;
        zstream->avail_out = outlen;

        if (inflateInit(zstream) != Z_OK)
            I_Error("P_LoadNodes: Error during ZDBSP nodes decompression initialization!");

        // resize if output buffer runs full
        while ((err = inflate(zstream, Z_SYNC_FLUSH)) == Z_OK)
        {
            int outlen_old = outlen;
            outlen = 2 * outlen_old;
            output = I_Realloc(output, outlen);
            zstream->next_out = output + outlen_old;
            zstream->avail_out = outlen - outlen_old;
        }

        if (err != Z_STREAM_END)
            I_Error("P_LoadNodes: Error during ZDBSP nodes decompression!");

        fprintf(stderr, "P_LoadNodes: ZDBSP nodes compression ratio %.3f\n",
                (float)zstream->total_out/zstream->total_in);

        data = output;

        if (inflateEnd(zstream) != Z_OK)
            I_Error("P_LoadNodes: Error during ZDBSP nodes decompression shut-down!");

        // release the original data lump
        W_ReleaseLumpNum(lump);
        free(zstream);
#else
        I_Error("P_LoadNodes: Compressed ZDBSP nodes are not supported!");
#endif
    }
    else
    {
        // skip header
        data += 4;
    }

    // 1. Load new vertices added during node building

    orgVerts = LONG(*((unsigned int*)data));
    data += sizeof(orgVerts);

    newVerts = LONG(*((unsigned int*)data));
    data += sizeof(newVerts);

    if (orgVerts + newVerts == (unsigned int)numvertexes)
    {
        newvertarray = vertexes;
    }
    else
    {
        newvertarray = Z_Malloc((orgVerts + newVerts) * sizeof(vertex_t), PU_LEVEL, 0);
        memcpy(newvertarray, vertexes, orgVerts * sizeof(vertex_t));
        memset(newvertarray + orgVerts, 0, newVerts * sizeof(vertex_t));
    }

    for (i = 0; i < newVerts; i++)
    {
        newvertarray[i + orgVerts].r_x =
        newvertarray[i + orgVerts].x = LONG(*((unsigned int*)data));
        data += sizeof(newvertarray[0].x);

        newvertarray[i + orgVerts].r_y =
        newvertarray[i + orgVerts].y = LONG(*((unsigned int*)data));
        data += sizeof(newvertarray[0].y);
    }

    if (vertexes != newvertarray)
    {
        for (i = 0; i < (unsigned int)numlines; i++)
        {
            lines[i].v1 = lines[i].v1 - vertexes + newvertarray;
            lines[i].v2 = lines[i].v2 - vertexes + newvertarray;
        }

        Z_Free(vertexes);
        vertexes = newvertarray;
        numvertexes = orgVerts + newVerts;
    }

    // 2. Load subsectors

    numSubs = LONG(*((unsigned int*)data));
    data += sizeof(numSubs);

    if (numSubs < 1)
        I_Error("P_LoadNodes: No subsectors in map!");

    numsubsectors = numSubs;
    subsectors = Z_Malloc(numsubsectors * sizeof(subsector_t), PU_LEVEL, 0);

    for (i = currSeg = 0; i < numsubsectors; i++)
    {
        mapsubsector_xnod_t *mseg = (mapsubsector_xnod_t*) data + i;

        subsectors[i].firstline = currSeg;
        subsectors[i].numlines = LONG(mseg->numsegs);
        currSeg += LONG(mseg->numsegs);
    }

    data += numsubsectors * sizeof(mapsubsector_xnod_t);

    // 3. Load segs

    numSegs = LONG(*((unsigned int*)data));
    data += sizeof(numSegs);

    // The number of stored segs should match the number of segs used by subsectors
    if (numSegs != currSeg)
    {
        I_Error("P_LoadNodes: Incorrect number of segs in ZDBSP nodes!");
    }

    numsegs = numSegs;
    segs = Z_Malloc(numsegs * sizeof(seg_t), PU_LEVEL, 0);
    memset(segs, 0, sizeof(seg_t) * numSegs);

    if (format.bsp == NFMT_XNOD)
    {
        P_LoadSegs_XNOD(data);
        data += numsegs * sizeof(mapseg_xnod_t);
    }
    else if (format.bsp == NFMT_XGLN)
    {
        P_LoadSegs_XGL(data, format.bsp);
        data += numsegs * sizeof(mapseg_xgln_t);
    }
    else if (format.bsp == NFMT_XGL2 || format.bsp == NFMT_XGL3)
    {
        P_LoadSegs_XGL(data, format.bsp);
        data += numsegs * sizeof(mapseg_xgl2_t);
    }

    // 4. Load nodes

    numNodes = LONG(*((unsigned int*)data));
    data += sizeof(numNodes);

    numnodes = numNodes;
    nodes = Z_Malloc(numnodes * sizeof(node_t), PU_LEVEL, 0);

    if (format.bsp == NFMT_XGL3)
    {
        P_LoadZNodes_XGL3((mapnode_xgl3_t *)data);
    }
    else
    {
        P_LoadZNodes_XNOD((mapnode_xnod_t *)data);
    }

#ifdef HAVE_LIBZ
    if (format.compressed && output)
    {
        Z_Free(output);
    }
    else
#endif
    {
        W_ReleaseLumpNum(lump);
    }
}
