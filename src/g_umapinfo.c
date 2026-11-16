//
//  Copyright(C) 2017 Christoph Oelckers
//  Copyright(C) 2021 Roman Fomin
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "g_umapinfo.h"

#include "d_mode.h"
#include "doomtype.h"
#include "i_system.h"
#include "m_array.h"
#include "m_misc.h"
#include "m_scanner.h"
#include "w_wad.h"
#include "z_zone.h"

boolean mapinfo_partimes = false;
boolean mapinfo_mapxy = false;

static mapentry_t *mapinfo = NULL;

static const char *const *actor_names = NULL;
static int actor_names_length = 0;

static const char *const actor_names_doom[250] = {
    "DoomPlayer",
    "ZombieMan",
    "ShotgunGuy",
    "Archvile",
    "ArchvileFire",
    "Revenant",
    "RevenantTracer",
    "RevenantTracerSmoke",
    "Fatso",
    "FatShot",
    "ChaingunGuy",
    "DoomImp",
    "Demon",
    "Spectre",
    "Cacodemon",
    "BaronOfHell",
    "BaronBall",
    "HellKnight",
    "LostSoul",
    "SpiderMastermind",
    "Arachnotron",
    "Cyberdemon",
    "PainElemental",
    "WolfensteinSS",
    "CommanderKeen",
    "BossBrain",
    "BossEye",
    "BossTarget",
    "SpawnShot",
    "SpawnFire",
    "ExplosiveBarrel",
    "DoomImpBall",
    "CacodemonBall",
    "Rocket",
    "PlasmaBall",
    "BFGBall",
    "ArachnotronPlasma",
    "BulletPuff",
    "Blood",
    "TeleportFog",
    "ItemFog",
    "TeleportDest",
    "BFGExtra",
    "GreenArmor",
    "BlueArmor",
    "HealthBonus",
    "ArmorBonus",
    "BlueCard",
    "RedCard",
    "YellowCard",
    "YellowSkull",
    "RedSkull",
    "BlueSkull",
    "Stimpack",
    "Medikit",
    "Soulsphere",
    "InvulnerabilitySphere",
    "Berserk",
    "BlurSphere",
    "RadSuit",
    "Allmap",
    "Infrared",
    "Megasphere",
    "Clip",
    "ClipBox",
    "RocketAmmo",
    "RocketBox",
    "Cell",
    "CellPack",
    "Shell",
    "ShellBox",
    "Backpack",
    "BFG9000",
    "Chaingun",
    "Chainsaw",
    "RocketLauncher",
    "PlasmaRifle",
    "Shotgun",
    "SuperShotgun",
    "TechLamp",
    "TechLamp2",
    "Column",
    "TallGreenColumn",
    "ShortGreenColumn",
    "TallRedColumn",
    "ShortRedColumn",
    "SkullColumn",
    "HeartColumn",
    "EvilEye",
    "FloatingSkull",
    "TorchTree",
    "BlueTorch",
    "GreenTorch",
    "RedTorch",
    "ShortBlueTorch",
    "ShortGreenTorch",
    "ShortRedTorch",
    "Stalagtite",
    "TechPillar",
    "CandleStick",
    "Candelabra",
    "BloodyTwitch",
    "Meat2",
    "Meat3",
    "Meat4",
    "Meat5",
    "NonsolidMeat2",
    "NonsolidMeat4",
    "NonsolidMeat3",
    "NonsolidMeat5",
    "NonsolidTwitch",
    "DeadCacodemon",
    "DeadMarine",
    "DeadZombieMan",
    "DeadDemon",
    "DeadLostSoul",
    "DeadDoomImp",
    "DeadShotgunGuy",
    "GibbedMarine",
    "GibbedMarineExtra",
    "HeadsOnAStick",
    "Gibs",
    "HeadOnAStick",
    "HeadCandles",
    "DeadStick",
    "LiveStick",
    "BigTree",
    "BurningBarrel",
    "HangNoGuts",
    "HangBNoBrain",
    "HangTLookingDown",
    "HangTSkull",
    "HangTLookingUp",
    "HangTNoBrain",
    "ColonGibs",
    "SmallBloodPool",
    "BrainStem",
    // Boom
    "PointPusher",
    "PointPuller",
    // MBF
    "MBFHelperDog",
    // MBF beta emulation
    "PlasmaBall1",
    "PlasmaBall2",
    "EvilSceptre",
    "UnholyBible",
    // Risen3D, later PrBoom+
    "MusicChanger",
    // Port-exlusive
    "Deh_Actor_145",
    "Deh_Actor_146",
    "Deh_Actor_147",
    "Deh_Actor_148",
    "Deh_Actor_149",
    // DEHEXTRA
    "Deh_Actor_150",
    "Deh_Actor_151",
    "Deh_Actor_152",
    "Deh_Actor_153",
    "Deh_Actor_154",
    "Deh_Actor_155",
    "Deh_Actor_156",
    "Deh_Actor_157",
    "Deh_Actor_158",
    "Deh_Actor_159",
    "Deh_Actor_160",
    "Deh_Actor_161",
    "Deh_Actor_162",
    "Deh_Actor_163",
    "Deh_Actor_164",
    "Deh_Actor_165",
    "Deh_Actor_166",
    "Deh_Actor_167",
    "Deh_Actor_168",
    "Deh_Actor_169",
    "Deh_Actor_170",
    "Deh_Actor_171",
    "Deh_Actor_172",
    "Deh_Actor_173",
    "Deh_Actor_174",
    "Deh_Actor_175",
    "Deh_Actor_176",
    "Deh_Actor_177",
    "Deh_Actor_178",
    "Deh_Actor_179",
    "Deh_Actor_180",
    "Deh_Actor_181",
    "Deh_Actor_182",
    "Deh_Actor_183",
    "Deh_Actor_184",
    "Deh_Actor_185",
    "Deh_Actor_186",
    "Deh_Actor_187",
    "Deh_Actor_188",
    "Deh_Actor_189",
    "Deh_Actor_190",
    "Deh_Actor_191",
    "Deh_Actor_192",
    "Deh_Actor_193",
    "Deh_Actor_194",
    "Deh_Actor_195",
    "Deh_Actor_196",
    "Deh_Actor_197",
    "Deh_Actor_198",
    "Deh_Actor_199",
    "Deh_Actor_200",
    "Deh_Actor_201",
    "Deh_Actor_202",
    "Deh_Actor_203",
    "Deh_Actor_204",
    "Deh_Actor_205",
    "Deh_Actor_206",
    "Deh_Actor_207",
    "Deh_Actor_208",
    "Deh_Actor_209",
    "Deh_Actor_210",
    "Deh_Actor_211",
    "Deh_Actor_212",
    "Deh_Actor_213",
    "Deh_Actor_214",
    "Deh_Actor_215",
    "Deh_Actor_216",
    "Deh_Actor_217",
    "Deh_Actor_218",
    "Deh_Actor_219",
    "Deh_Actor_220",
    "Deh_Actor_221",
    "Deh_Actor_222",
    "Deh_Actor_223",
    "Deh_Actor_224",
    "Deh_Actor_225",
    "Deh_Actor_226",
    "Deh_Actor_227",
    "Deh_Actor_228",
    "Deh_Actor_229",
    "Deh_Actor_230",
    "Deh_Actor_231",
    "Deh_Actor_232",
    "Deh_Actor_233",
    "Deh_Actor_234",
    "Deh_Actor_235",
    "Deh_Actor_236",
    "Deh_Actor_237",
    "Deh_Actor_238",
    "Deh_Actor_239",
    "Deh_Actor_240",
    "Deh_Actor_241",
    "Deh_Actor_242",
    "Deh_Actor_243",
    "Deh_Actor_244",
    "Deh_Actor_245",
    "Deh_Actor_246",
    "Deh_Actor_247",
    "Deh_Actor_248",
    "Deh_Actor_249",
};

static const char *const actor_names_heretic[161] = {
    "CrystalVial",
    "SilverShield",
    "EnchantedShield",
    "BagOfHolding",
    "SuperMap",
    "ArtiInvisibility",
    "ArtiHealth",
    "ArtiFly",
    "ArtiInvulnerability",
    "ArtiTomeOfPower",
    "ArtiEgg",
    "EggFX",
    "ArtiSuperHealth",
    "ArtiTorch",
    "ArtiTimeBomb",
    "ActivatedTimeBomb",
    "ArtiTeleport",
    "Pod",
    "PodGoo",
    "PodGenerator",
    "WaterSplash",
    "WaterSplashBase",
    "LavaSplash",
    "LavaSmoke",
    "SludgeChunk",
    "SludgeSplash",
    "SkullHang70",
    "SkullHang60",
    "SkullHang45",
    "SkullHang35",
    "Chandelier",
    "SerpentTorch",
    "SmallPillar",
    "StalagmiteSmall",
    "StalagmiteLarge",
    "StalactiteSmall",
    "StalactiteLarge",
    "FireBrazier",
    "Barrel",
    "BrownPillar",
    "Moss1",
    "Moss2",
    "WallTorch",
    "HangingCorpse",
    "KeyGizmoBlue",
    "KeyGizmoGreen",
    "KeyGizmoYellow",
    "KeyGizmoFloat",
    "Volcano",
    "VolcanoBlast",
    "VolcanoTBlast",
    "TeleGlitterGenerator1",
    "TeleGlitterGenerator2",
    "TeleGlitter1",
    "TeleGlitter2",
    "TeleportFog",
    "TeleportDest",
    "StaffPuff",
    "StaffPuff2",
    "BeakPuff",
    "Gauntlets",
    "GauntletPuff1",
    "GauntletPuff2",
    "Blaster",
    "BlasterFX1",
    "BlasterSmoke",
    "Ripper",
    "BlasterPuff1",
    "BlasterPuff2",
    "MaceSpawner",
    "MaceFX1",
    "MaceFX2",
    "MaceFX3",
    "MaceFX4",
    "SkullRod",
    "HornRodFX1",
    "HornRodFX2",
    "RainPillar1",
    "RainPillar2",
    "RainPillar3",
    "RainPillar4",
    "GoldWandFX1",
    "GoldWandFX2",
    "GoldWandPuff1",
    "GoldWandPuff2",
    "PhoenixRod",
    "PhoenixFX1",
    "PhoenixFXUnknown",
    "PhoenixPuff",
    "PhoenixFX2",
    "Crossbow",
    "CrossbowFX1",
    "CrossbowFX2",
    "CrossbowFX3",
    "CrossbowFX4",
    "Blood",
    "BloodSplatter",
    "HereticPlayer",
    "BloodySkull",
    "ChickenPlayer",
    "Chicken",
    "Feather",
    "Mummy",
    "MummyLeader",
    "MummyGhost",
    "MummyLeaderGhost",
    "MummySoul",
    "MummyFX1",
    "Beast",
    "BeastBall",
    "BurnBall",
    "BurnBallFB",
    "Puffy",
    "Snake",
    "SnakeProjA",
    "SnakeProjB",
    "Ironlich",
    "HeadFX1",
    "HeadFX2",
    "HeadFX3",
    "Whirlwind",
    "Clink",
    "Wizard",
    "WizardFX1",
    "HereticImp",
    "HereticImpLeader",
    "HereticImpChunk1",
    "HereticImpChunk2",
    "HereticImpBall",
    "Knight",
    "KnightGhost",
    "KnightAxe",
    "RedAxe",
    "Sorcerer1",
    "SorcererFX1",
    "Sorcerer2",
    "Sorcerer2FX1",
    "Sorcerer2FXSpark",
    "Sorcerer2FX2",
    "Sorcerer2Telefade",
    "Minotaur",
    "MinotaurFX1",
    "MinotaurFX2",
    "MinotaurFX3",
    "KeyGreen",
    "KeyBlue",
    "KeyYellow",
    "GoldWandAmmo",
    "GoldWandHefty",
    "MaceAmmo",
    "MaceHefty",
    "CrossbowAmmo",
    "CrossbowHefty",
    "SkullRodAmmo",
    "SkullRodHefty",
    "PhoenixRodAmmo",
    "PhoenixRodHefty",
    "BlasterAmmo",
    "BlasterHefty",
    "SoundWind",
    "SoundWaterfall",
};

static void ReplaceString(char **to, const char *from)
{
    if (*to != NULL)
    {
        free(*to);
    }
    *to = M_StringDuplicate(from);
}

static void FreeMapEntry(mapentry_t *mape)
{
    if (mape->levelname)
    {
        free(mape->levelname);
    }
    if (mape->label)
    {
        free(mape->label);
    }
    if (mape->intertext)
    {
        free(mape->intertext);
    }
    if (mape->intertextsecret)
    {
        free(mape->intertextsecret);
    }
    if (mape->author)
    {
        free(mape->author);
    }
    array_free(mape->bossactions);
    memset(mape, 0, sizeof(*mape));
}

// Parses a set of string and concatenates them
// Returns a pointer to the string (must be freed)

static char *ParseMultiString(scanner_t *s)
{
    char *build = NULL;

    do
    {
        SCN_MustGetToken(s, TK_StringConst);
        if (build == NULL)
        {
            build = M_StringDuplicate(SCN_GetString(s));
        }
        else
        {
            char *tmp = build;
            build = M_StringJoin(tmp, "\n", SCN_GetString(s), NULL);
            free(tmp);
        }
    } while (SCN_CheckToken(s, ','));

    return build;
}

// Parses a lump name. The buffer must be at least 9 characters.

static void ParseLumpName(scanner_t *s, char *buffer)
{
    SCN_MustGetToken(s, TK_StringConst);
    if (strlen(SCN_GetString(s)) > 8)
    {
        SCN_Error(s, "String too long. Maximum size is 8 characters.");
    }
    strncpy(buffer, SCN_GetString(s), 8);
    buffer[8] = 0;
    M_ForceUppercase(buffer);
}

// Parses a standard property that is already known
// These do not get stored in the property list
// but in dedicated struct member variables.

static void ParseStandardProperty(scanner_t *s, mapentry_t *mape,
                                  AddEpisodeFunc AddEpisode,
                                  ClearEpisodeFunc ClearEpisodes)
{
    char *prop;
    SCN_MustGetToken(s, TK_Identifier);
    prop = M_StringDuplicate(SCN_GetString(s));

    SCN_MustGetToken(s, '=');
    if (!strcasecmp(prop, "levelname"))
    {
        SCN_MustGetToken(s, TK_StringConst);
        ReplaceString(&mape->levelname, SCN_GetString(s));
    }
    else if (!strcasecmp(prop, "label"))
    {
        if (SCN_CheckToken(s, TK_Identifier))
        {
            if (!strcasecmp(SCN_GetString(s), "clear"))
            {
                mape->flags |= MapInfo_LabelClear;
            }
            else
            {
                SCN_Error(s, "Either 'clear' or string constant expected");
            }
        }
        else
        {
            mape->flags &= ~MapInfo_LabelClear;
            SCN_MustGetToken(s, TK_StringConst);
            ReplaceString(&mape->label, SCN_GetString(s));
        }
    }
    else if (!strcasecmp(prop, "author"))
    {
        SCN_MustGetToken(s, TK_StringConst);
        ReplaceString(&mape->author, SCN_GetString(s));
    }
    else if (!strcasecmp(prop, "episode"))
    {
        if (SCN_CheckToken(s, TK_Identifier))
        {
            if (!strcasecmp(SCN_GetString(s), "clear"))
            {
                ClearEpisodes();
            }
            else
            {
                SCN_Error(s, "Either 'clear' or string constant expected");
            }
        }
        else
        {
            char lumpname[9] = {0};
            char *alttext = NULL;
            char key = 0;

            ParseLumpName(s, lumpname);
            if (SCN_CheckToken(s, ','))
            {
                SCN_MustGetToken(s, TK_StringConst);
                alttext = M_StringDuplicate(SCN_GetString(s));
                if (SCN_CheckToken(s, ','))
                {
                    const char *tmp;
                    SCN_MustGetToken(s, TK_StringConst);
                    tmp = SCN_GetString(s);
                    key = M_ToLower(tmp[0]);
                }
            }

            AddEpisode(mape->lumpname, lumpname, alttext, key);

            if (alttext)
            {
                free(alttext);
            }
        }
    }
    else if (!strcasecmp(prop, "next"))
    {
        ParseLumpName(s, mape->nextmap);
        if (!G_ValidateMapName(mape->nextmap, NULL, NULL))
        {
            SCN_Error(s, "Invalid map name %s.", mape->nextmap);
        }
    }
    else if (!strcasecmp(prop, "nextsecret"))
    {
        ParseLumpName(s, mape->nextsecret);
        if (!G_ValidateMapName(mape->nextsecret, NULL, NULL))
        {
            SCN_Error(s, "Invalid map name %s", mape->nextsecret);
        }
    }
    else if (!strcasecmp(prop, "levelpic"))
    {
        ParseLumpName(s, mape->levelpic);
    }
    else if (!strcasecmp(prop, "skytexture"))
    {
        ParseLumpName(s, mape->skytexture);
    }
    else if (!strcasecmp(prop, "music"))
    {
        ParseLumpName(s, mape->music);
    }
    else if (!strcasecmp(prop, "endpic"))
    {
        mape->flags |= MapInfo_EndGameArt;
        ParseLumpName(s, mape->endpic);
    }
    else if (!strcasecmp(prop, "endcast"))
    {
        SCN_MustGetToken(s, TK_BoolConst);
        if (SCN_GetBoolean(s))
        {
            mape->flags |= MapInfo_EndGameCast;
        }
        else
        {
            mape->flags &= ~MapInfo_EndGameCast;
            mape->flags |= MapInfo_EndGameClear;
        }
    }
    else if (!strcasecmp(prop, "endbunny"))
    {
        SCN_MustGetToken(s, TK_BoolConst);
        if (SCN_GetBoolean(s))
        {
            mape->flags |= MapInfo_EndGameBunny;
        }
        else
        {
            mape->flags &= ~MapInfo_EndGameBunny;
            mape->flags |= MapInfo_EndGameClear;
        }
    }
    else if (!strcasecmp(prop, "endgame"))
    {
        SCN_MustGetToken(s, TK_BoolConst);
        if (SCN_GetBoolean(s))
        {
            mape->flags |= MapInfo_EndGameStandard;
        }
        else
        {
            mape->flags &= ~MapInfo_EndGameStandard;
            mape->flags |= MapInfo_EndGameClear;
        }
    }
    else if (!strcasecmp(prop, "exitpic"))
    {
        ParseLumpName(s, mape->exitpic);
    }
    else if (!strcasecmp(prop, "enterpic"))
    {
        ParseLumpName(s, mape->enterpic);
    }
    else if (!strcasecmp(prop, "nointermission"))
    {
        SCN_MustGetToken(s, TK_BoolConst);
        if (SCN_GetBoolean(s))
        {
            mape->flags |= MapInfo_NoIntermission;
        }
        else
        {
            mape->flags &= ~MapInfo_NoIntermission;
        }
    }
    else if (!strcasecmp(prop, "partime"))
    {
        SCN_MustGetToken(s, TK_IntConst);
        mape->partime = SCN_GetNumber(s);
    }
    else if (!strcasecmp(prop, "intertext"))
    {
        if (SCN_CheckToken(s, TK_Identifier))
        {
            if (!strcasecmp(SCN_GetString(s), "clear"))
            {
                mape->flags |= MapInfo_InterTextClear;
            }
            else
            {
                SCN_Error(s, "Either 'clear' or string constant expected");
            }
        }
        else
        {
            mape->flags &= ~MapInfo_InterTextClear;
            if (mape->intertext)
            {
                free(mape->intertext);
            }
            mape->intertext = ParseMultiString(s);
        }
    }
    else if (!strcasecmp(prop, "intertextsecret"))
    {
        if (SCN_CheckToken(s, TK_Identifier))
        {
            if (!strcasecmp(SCN_GetString(s), "clear"))
            {
                mape->flags |= MapInfo_InterTextSecretClear;
            }
            else
            {
                SCN_Error(s, "Either 'clear' or string constant expected");
            }
        }
        else
        {
            mape->flags &= ~MapInfo_InterTextSecretClear;
            if (mape->intertextsecret)
            {
                free(mape->intertextsecret);
            }
            mape->intertextsecret = ParseMultiString(s);
        }
    }
    else if (!strcasecmp(prop, "interbackdrop"))
    {
        ParseLumpName(s, mape->interbackdrop);
    }
    else if (!strcasecmp(prop, "intermusic"))
    {
        ParseLumpName(s, mape->intermusic);
    }
    else if (!strcasecmp(prop, "bossaction"))
    {
        SCN_MustGetToken(s, TK_Identifier);
        if (!strcasecmp(SCN_GetString(s), "clear"))
        {
            mape->flags |= MapInfo_BossActionClear;
            array_free(mape->bossactions);
        }
        else
        {
            int type, special, tag;
            mape->flags &= ~MapInfo_BossActionClear;
            for (type = 0; type < actor_names_length; ++type)
            {
                if (!strcasecmp(SCN_GetString(s), actor_names[type]))
                {
                    break;
                }
            }
            if (type == actor_names_length)
            {
                SCN_Error(s, "bossaction: unknown thing type '%s'",
                          SCN_GetString(s));
            }
            SCN_MustGetToken(s, ',');
            SCN_MustGetToken(s, TK_IntConst);
            special = SCN_GetNumber(s);
            SCN_MustGetToken(s, ',');
            SCN_MustGetToken(s, TK_IntConst);
            tag = SCN_GetNumber(s);
            // allow no 0-tag specials here, unless a level exit.
            if (tag != 0 || special == 11 || special == 51 || special == 52 ||
                special == 124)
            {
                bossaction_t bossaction = {type, special, tag};
                array_push(mape->bossactions, bossaction);
            }
        }
    }
    // If no known property name was given, skip all comma-separated values
    // after the = sign
    else
    {
        do
        {
            SCN_GetNextToken(s, true);
        } while (SCN_CheckToken(s, ','));
    }

    free(prop);
}

static void ParseMapEntry(scanner_t *s, mapentry_t *entry, AddEpisodeFunc add,
                          ClearEpisodeFunc clear)
{
    SCN_MustGetToken(s, TK_Identifier);
    if (strcasecmp(SCN_GetString(s), "map"))
    {
        SCN_Error(s, "Expected 'map' but got '%s' instead", SCN_GetString(s));
    }

    SCN_MustGetToken(s, TK_Identifier);
    if (!G_ValidateMapName(SCN_GetString(s), NULL, NULL))
    {
        SCN_Error(s, "Invalid map name %s", SCN_GetString(s));
    }
    ReplaceString(&entry->lumpname, SCN_GetString(s));

    SCN_MustGetToken(s, '{');
    while (!SCN_CheckToken(s, '}'))
    {
        ParseStandardProperty(s, entry, add, clear);
    }
}

void G_ParseMapInfo(int lumpnum, GameMission_t mission, GameMode_t mode,
                    AddEpisodeFunc add, ClearEpisodeFunc clear)
{
    scanner_t *s = SCN_Open("UMAPINFO", W_CacheLumpNum(lumpnum, PU_CACHE),
                            W_LumpLength(lumpnum));
    boolean pwad_help2 =
        (mode == retail) && W_IsPWADLump(lumpinfo[W_CheckNumForName("HELP2")]);

    if (mission >= doom && mission <= pack_hacx)
    {
        actor_names = actor_names_doom;
        actor_names_length = arrlen(actor_names_doom);
    }
    else if (mission == heretic)
    {
        actor_names = actor_names_heretic;
        actor_names_length = arrlen(actor_names_heretic);
    }
    else
    {
        I_Error("G_ParseMapInfo: unsupported gamemission: %d", mission);
    }

    while (SCN_TokensLeft(s))
    {
        mapentry_t parsed = {0};
        int i;
        ParseMapEntry(s, &parsed, add, clear);

        // Set default level progression here to simplify the checks elsewhere.
        // Doing this lets us skip all normal code for this if nothing has been
        // defined.
        if (parsed.flags & MapInfo_EndGame)
        {
            parsed.nextmap[0] = 0;
        }
        else if (!parsed.nextmap[0] && !(parsed.flags & MapInfo_EndGameClear))
        {
            if (!strcasecmp(parsed.lumpname, "MAP30"))
            {
                parsed.flags |= MapInfo_EndGameCast;
            }
            else if (!strcasecmp(parsed.lumpname, "E1M8"))
            {
                parsed.flags |= MapInfo_EndGameArt;
                M_CopyLumpName(parsed.endpic, pwad_help2 ? "HELP2" : "CREDIT");
            }
            else if (!strcasecmp(parsed.lumpname, "E2M8"))
            {
                parsed.flags |= MapInfo_EndGameArt;
                M_CopyLumpName(parsed.endpic, "VICTORY2");
            }
            else if (!strcasecmp(parsed.lumpname, "E3M8"))
            {
                parsed.flags |= MapInfo_EndGameBunny;
            }
            else if (!strcasecmp(parsed.lumpname, "E4M8"))
            {
                parsed.flags |= MapInfo_EndGameArt;
                M_CopyLumpName(parsed.endpic, "ENDPIC");
            }
            else
            {
                int ep, map;
                if (G_ValidateMapName(parsed.lumpname, &ep, &map))
                {
                    M_CopyLumpName(parsed.nextmap, G_MapName(ep, map + 1));
                }
            }
        }

        // Does this entry already exist? If yes, replace it.
        for (i = 0; i < array_size(mapinfo); ++i)
        {
            if (!strcmp(parsed.lumpname, mapinfo[i].lumpname))
            {
                FreeMapEntry(&mapinfo[i]);
                mapinfo[i] = parsed;
                break;
            }
        }
        // Not found so create a new one.
        if (i == array_size(mapinfo))
        {
            array_push(mapinfo, parsed);
        }
    }

    SCN_Close(s);
}

mapentry_t *G_LookupMapinfo(int episode, int map)
{
    mapentry_t *entry;
    char lumpname[9] = {0};
    M_StringCopy(lumpname, G_MapName(episode, map), sizeof(lumpname));

    array_foreach(entry, mapinfo)
    {
        if (!strcasecmp(lumpname, entry->lumpname))
        {
            return entry;
        }
    }

    return NULL;
}

// Check if the given map name can be expressed as a gameepisode/gamemap pair
// and be reconstructed from it.

boolean G_ValidateMapName(const char *mapname, int *episode, int *map)
{
    char lumpname[9], mapuname[9];
    int e = -1, m = -1;

    if (strlen(mapname) > 8)
    {
        return false;
    }

    M_StringCopy(mapuname, mapname, 8);
    mapuname[8] = 0;
    M_ForceUppercase(mapuname);

    if (!mapinfo_mapxy)
    {
        if (sscanf(mapuname, "E%dM%d", &e, &m) != 2)
        {
            return false;
        }
        M_CopyLumpName(lumpname, G_MapName(e, m));
    }
    else
    {
        if (sscanf(mapuname, "MAP%d", &m) != 1)
        {
            return false;
        }
        M_CopyLumpName(lumpname, G_MapName(e = 1, m));
    }

    if (episode)
    {
        *episode = e;
    }
    if (map)
    {
        *map = m;
    }

    return strcmp(mapuname, lumpname) == 0;
}

char *G_MapName(int e, int m)
{
    static char name[9];

    if (mapinfo_mapxy)
        M_snprintf(name, sizeof(name), "MAP%02d", m);
    else
        M_snprintf(name, sizeof(name), "E%dM%d", e, m);

    return name;
}