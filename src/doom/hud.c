//////////////////////////////////////
//
//  HUD.C: Hud text, menus, cursor
//
//////////////////////////////////////

#include "hud.h"
#include "bot.h"  
#include "dj.h"
#include "pkemeter.h"

//  The info readout allows us to watch variables on-screen during gameplay
void UpdateInfoReadout()
{	
	if (!Marshmallow_InfoReadout) 
	{
		EraseInfoReadout();
	}
	else
	{
        //AddToInfoReadout("MonsterHitpointsScale: ", MonsterHitpointsScale, 5);
        //AddToInfoReadout("marshmallow_rndindex: ", marshmallow_rndindex, 7);
        //AddToInfoReadout("prndindex: ", prndindex, 8);

		DJ_ShowInfo();
		
		//PKE_ShowInfo();
	}
}


void SetMarshmallowColors()
{

    CrispyReplaceColor( PKEHINT_SMALL, CR_BLUE, PKEHINT_SMALL);
    CrispyReplaceColor( PKEHINT_MEDIUM, CR_GREEN, PKEHINT_MEDIUM);
    CrispyReplaceColor( PKEHINT_LARGE, CR_GOLD, PKEHINT_LARGE);

    CrispyReplaceColor( PKERADIUS_STRING1, CR_BLUE, "SMALL");
    CrispyReplaceColor( PKERADIUS_STRING2, CR_GREEN, "MEDIUM");
    CrispyReplaceColor( PKERADIUS_STRING3, CR_GOLD, "LARGE");

	CrispyReplaceColor( "BOT 1: ", CR_GRAY, "BOT 1: ");
	CrispyReplaceColor( "BOT 2: ", CR_GOLD, "BOT 2: ");
	CrispyReplaceColor( "BOT 3: ", CR_DARK, "BOT 3: ");

	CrispyReplaceColor( MENU_CURSOR, CR_GREEN, MENU_CURSOR);

    CrispyReplaceColor( "    Gameplay options...", CR_GRAY, "...");
    CrispyReplaceColor( "    Music options...", CR_GRAY, "...");
    CrispyReplaceColor( "    MESSAGES options...", CR_GRAY, "...");

	CrispyReplaceColor( MENU_TITLE1, CR_GRAY, MENU_TITLE1);
	CrispyReplaceColor( MENU_TITLE2, CR_GOLD, MENU_TITLE2);

	CrispyReplaceColor( OPTION_ON, CR_GREEN, OPTION_ON);
	CrispyReplaceColor( OPTION_OFF, CR_DARK, OPTION_OFF);
	
	CrispyReplaceColor( DYNAMICMUSIC_ON, CR_GRAY, "MARSHMALLOW");
	CrispyReplaceColor( DYNAMICMUSIC_OFF, CR_GOLD, DYNAMICMUSIC_OFF);

	CrispyReplaceColor( CYBERSPOT, CR_GOLD, "CYBERDEMON");
	CrispyReplaceColor( VILESPOT, CR_GOLD, "ARCHVILE");
	CrispyReplaceColor( SPIDERSPOT, CR_GOLD, "SPIDERDEMON");
	CrispyReplaceColor( PLAYERSHOT, CR_GREEN, "PLAYER");
	CrispyReplaceColor( SHOTPLAYER, CR_GREEN, "PLAYER");

	// IDEA: try assigning different colors to different states (ex. ATTACKING will be red)
	
	CrispyReplaceColor( SEEK, CR_GOLD, SEEK);
	CrispyReplaceColor( PATROL, CR_BLUE, PATROL);
	CrispyReplaceColor( NONE, CR_DARK, NONE);

	CrispyReplaceColor( FOLLOWPATH, CR_GOLD, FOLLOWPATH);
	
	CrispyReplaceColor( FOLLOW, CR_GREEN, FOLLOW);
	CrispyReplaceColor( COVER, CR_GREEN, COVER);
	CrispyReplaceColor( USING, CR_BLUE, USING);

	CrispyReplaceColor( REGROUP, CR_DARK, REGROUP);
	CrispyReplaceColor( REGROUPSQUAD, CR_GREEN, "ORDER: ");

	CrispyReplaceColor( TAKEPOINT, CR_GOLD, TAKEPOINT);

	CrispyReplaceColor( STAY, CR_BLUE, STAY);
	CrispyReplaceColor( ATTACK, CR_RED, ATTACK);
	CrispyReplaceColor( WAIT, CR_BLUE, WAIT);
	
	CrispyReplaceColor( NAZIKILL, CR_GOLD, "STORMTROOPER");
	CrispyReplaceColor( ZOMKILL, CR_GOLD, "ZOMBIEMAN");
	CrispyReplaceColor( SGKILL, CR_GOLD, "SARGEANT");
	CrispyReplaceColor( CGKILL, CR_GOLD, "CHAINGUNNER");
	CrispyReplaceColor( IMPKILL, CR_GOLD, "IMP");
	CrispyReplaceColor( DEMONKILL, CR_GOLD, "DEMON");
	CrispyReplaceColor( SPKILL, CR_GOLD, "SPECTRE");
	CrispyReplaceColor( FATKILL, CR_GOLD, "MANCUBUS");
	CrispyReplaceColor( VILEKILL, CR_GOLD, "ARCHVILE");
	CrispyReplaceColor( CYBERKILL, CR_GOLD, "CYBERDEMON");
	CrispyReplaceColor( BOSSKILL, CR_GOLD, "SPIDERDEMON");
	CrispyReplaceColor( SKELKILL, CR_GOLD, "REVENANT");
	CrispyReplaceColor( BARONKILL, CR_GOLD, "BARON OF HELL");
	CrispyReplaceColor( KNIGHTKILL, CR_GOLD, "HELL KNIGHT");
	CrispyReplaceColor( BABYKILL, CR_GOLD, "ARACHNOTRON");

	CrispyReplaceColor( CONFIRMSUICIDE, CR_GRAY, "SUICIDE");  // gray makes it pretty noticeable, right?
	CrispyReplaceColor( YOUSEPPUKU, CR_DARK, "SEPPUKU");  // which color best represents SUICIDE?!?!?

	CrispyReplaceColor( VILEDEAD, CR_GREEN, "ARCHVILE");
	CrispyReplaceColor( SPIDERDEAD, CR_GREEN, "SPIDERDEMON");
	CrispyReplaceColor( CYBERDEAD, CR_GREEN, "CYBERDEMON");

	CrispyReplaceColor( PROMPTRADSUIT, CR_GREEN, "'R'");
	//CrispyReplaceColor( PROMPTRADSUIT, CR_GRAY, "RAD-SUIT");  // can't use two different colors in the same line
	CrispyReplaceColor( PROMPTMEDKIT, CR_GREEN, "'R'");
	//CrispyReplaceColor( PROMPTMEDKIT, CR_GRAY, "MEDKIT");  // can't use two different colors in the same line

	CrispyReplaceColor( ADDEDRADSUIT, CR_GRAY, "RAD-SUIT");
	CrispyReplaceColor( ADDEDMEDKIT, CR_GREEN, "MEDKIT");
	//CrispyReplaceColor( ADDEDINVIS, CR_DARK, "INVISIBILITY");  // not working for some reason

	CrispyReplaceColor( USEDMEDICAL, CR_GRAY, "MEDICAL");

	CrispyReplaceColor( HAVEMEDKIT, CR_GREEN, "MEDKIT");
	CrispyReplaceColor( HAVEINVIS, CR_DARK, "INVISIBILITY");
	CrispyReplaceColor( HAVERADSUIT, CR_GRAY, "RAD-SUIT");

	CrispyReplaceColor( USINGMEDKIT, CR_GREEN, "MEDKIT");
	CrispyReplaceColor( USINGINVIS, CR_DARK, "INVISIBILITY");
	CrispyReplaceColor( USINGRADSUIT, CR_GRAY, "RAD-SUIT");

	CrispyReplaceColor( INVRADSUIT, CR_GRAY, "RAD-SUIT");
	//CrispyReplaceColor( INVINVUL, CR_RED, "INVULNERABILITY");  // too long to color 
	CrispyReplaceColor( INVINVIS, CR_DARK, "INVISIBILITY");  
	CrispyReplaceColor( INVMEDKIT, CR_GREEN, "MEDKIT");
	CrispyReplaceColor( INVAUTOMAP, CR_GOLD, "AUTO-MAP");

	CrispyReplaceColor( MISSILELOCK1, CR_GOLD, MISSILELOCK1);  // been playing with different colors on this one
	CrispyReplaceColor( MISSILELOCK2, CR_GREEN, MISSILELOCK2);
	
	CrispyReplaceColor( DANGERSYMBOL1, CR_GREEN, DANGERSYMBOL1);
	CrispyReplaceColor( DANGERSYMBOL2, CR_BLUE, DANGERSYMBOL2);
	CrispyReplaceColor( DANGERSYMBOL3, CR_DARK, DANGERSYMBOL3);
	CrispyReplaceColor( DANGERSYMBOL4, CR_GOLD, DANGERSYMBOL4);
	CrispyReplaceColor( DANGERSYMBOL5, CR_RED, DANGERSYMBOL5);

	CrispyReplaceColor( PLAYLISTMODE1, CR_GOLD, PLAYLISTMODE1);
	CrispyReplaceColor( PLAYLISTMODE2, CR_BLUE, PLAYLISTMODE2);
	CrispyReplaceColor( PLAYLISTMODE3, CR_GRAY, PLAYLISTMODE3);
	CrispyReplaceColor( PLAYLISTMODE4, CR_GREEN, PLAYLISTMODE4);
	CrispyReplaceColor( PLAYLISTMODE5, CR_DARK, PLAYLISTMODE5);

	CrispyReplaceColor( SHORTER, CR_GRAY, "SHORTER");
	CrispyReplaceColor( LONGER, CR_GOLD, "LONGER");

	CrispyReplaceColor( THINGLABEL, CR_GRAY, "Thing:");

	CrispyReplaceColor( DOOMLABEL, CR_GREEN, "DOOM");
	CrispyReplaceColor( DOOM2LABEL, CR_GOLD, "DOOM 2");
	CrispyReplaceColor( BOTHLABEL, CR_GRAY, "BOTH");
	
	CrispyReplaceColor( PLASMALOW, CR_GRAY, "WARNING:");

	CrispyReplaceColor( TARGETHP, CR_GRAY, "TARGET HP:");

	CrispyReplaceColor( GOBUTTON, CR_GREEN, "GO!");
	
	CrispyReplaceColor( WHICHMAP, CR_GRAY, "Choose Map:");

	CrispyReplaceColor( "TELEFRAG!", CR_GREEN, "TELEFRAG!");
	CrispyReplaceColor( FIGHT, CR_GOLD, "FIGHT!");

	CrispyReplaceColor( SANDBOXDONE, CR_GREEN, "COMPLETED!");

	//CrispyReplaceColor( "PISTOL", CR_GRAY, "PISTOL");
	CrispyReplaceColor( "PROGRESSIVE", CR_GREEN, "PROGRESSIVE");

	CrispyReplaceColor( HELPTITLE, CR_GOLD, "KEYBOARD");
	CrispyReplaceColor( HELPLINE1, CR_GREEN, "'M'");
	CrispyReplaceColor( HELPLINE2, CR_GREEN, "'V'");
	CrispyReplaceColor( HELPLINE3, CR_GREEN, "'P'");
	CrispyReplaceColor( HELPLINE4, CR_GREEN, "'F'");
	CrispyReplaceColor( HELPLINE5, CR_GREEN, "'T'");
    CrispyReplaceColor( HELPLINE_RANGE, CR_GREEN, "'R'");

    CrispyReplaceColor( WAITDIPSHIT, CR_DARK, "WAIT");
    CrispyReplaceColor( SPAWNINGOBJECT, CR_GREEN, "SPAWNING");

	CrispyReplaceColor( TIP, CR_GRAY, "TIP:");

	CrispyReplaceColor( HELPLINE6, CR_GREEN, "Hold USE");
	CrispyReplaceColor( HELPLINE8, CR_GREEN, "Hold SHIFT");
	//CrispyReplaceColor( HELPLINE7, CR_GREEN, "'G'");
	CrispyReplaceColor( HELPLINE9, CR_GREEN, "'H'");
	//CrispyReplaceColor( HELPLINE_BRACKETS, CR_GREEN, "'[' or ']'");
	CrispyReplaceColor( HELPLINE_PGUPDN, CR_GREEN, "PGUP, PGDN");
	//CrispyReplaceColor( HELPLINE_SLASH, CR_GREEN, "'\\'");
	CrispyReplaceColor( HELPLINE_INSERT, CR_GREEN, "INSERT");
	CrispyReplaceColor( HELPLINE_M, CR_GREEN, "'M'");

	CrispyReplaceColor( AMBIENTSONGS, CR_GREEN, "DOOM DJ:");
	CrispyReplaceColor( INTENSESONGS, CR_GREEN,"DOOM DJ:");
	CrispyReplaceColor( SUSPENSEFULSONGS, CR_GREEN, "DOOM DJ:");
	CrispyReplaceColor( BOSSSONGS, CR_GREEN, "DOOM DJ:");
	CrispyReplaceColor( VICTORYSONGS, CR_GREEN, "DOOM DJ:");
	CrispyReplaceColor( ALLSONGS, CR_GREEN, "DOOM DJ:");
	CrispyReplaceColor( NEXTSONG, CR_GREEN, "DOOM DJ:");

	CrispyReplaceColor( RANDOMLEVEL, CR_DARK, "Random");

	CrispyReplaceColor( INVENTORYTITLE, CR_GRAY, "INVENTORY");
	CrispyReplaceColor( INVEMPTY, CR_DARK, "NO ITEMS" );

	CrispyReplaceColor( "MISSILE", CR_GRAY, "MISSILE");
	CrispyReplaceColor( "PLASMA", CR_BLUE, "PLASMA");
	CrispyReplaceColor( "GREEN PLASMA", CR_GREEN, "GREEN");
	CrispyReplaceColor( "BFG9000", CR_GREEN, "BFG9000");
	CrispyReplaceColor( "NOT PLAYING", CR_DARK, "NOT PLAYING");
	
	CrispyReplaceColor( TREASURE_REDSKULL, CR_DARK, "TREASURE!");
	CrispyReplaceColor( TREASURE_YELLOWSKULL, CR_GOLD, "TREASURE!");
	CrispyReplaceColor( TREASURE_BLUESKULL, CR_BLUE, "TREASURE!");
	CrispyReplaceColor( SCORELABEL, CR_GOLD, "TREASURE");
	CrispyReplaceColor( REMAINLABEL, CR_GOLD, "REMAINING");
	CrispyReplaceColor( ALLTREASURE, CR_GREEN, "COLLECTED!");
	CrispyReplaceColor( NOTREASURE, CR_DARK, "NO");
	CrispyReplaceColor( TREASURECONGRATS, CR_GREEN, "TREASURE");

	CrispyReplaceColor( CHAINGUNLABEL, CR_GRAY, "CHAINGUN");
	CrispyReplaceColor( SHOTGUNLABEL, CR_GOLD, "SHOTGUN");

	CrispyReplaceColor( TARGETHPALL, CR_GOLD, "ALL");
	CrispyReplaceColor( TARGETHPBOSSES, CR_GRAY, "BOSSES");

	CrispyReplaceColor( ROAM, CR_GOLD, "ROAMING");

	CrispyReplaceColor( TREASURENORMAL, CR_GRAY, "SCARCE");
	CrispyReplaceColor( TREASUREFULL, CR_GOLD, "ABUNDANT");

	CrispyReplaceColor( MODECOOP, CR_GREEN, "COOPERATIVE");
	CrispyReplaceColor( MODEDM, CR_GOLD, "DEATHMATCH");
	CrispyReplaceColor( MODESP, CR_DARK, "SOLO PLAY");
	CrispyReplaceColor( MODESBOX, CR_GRAY, "SANDBOX");

    CrispyReplaceColor( "OFF", CR_DARK, "OFF");

	CrispyReplaceColor( BOT1LABEL, CR_GRAY, "BOT1:");
	CrispyReplaceColor( BOT2LABEL, CR_GOLD, "BOT2:");
	CrispyReplaceColor( BOT3LABEL, CR_RED, "BOT3:");

	CrispyReplaceColor( PLAYERSHOT, CR_GREEN, "PLAYER");
	CrispyReplaceColor( SHOTPLAYER, CR_GREEN, "PLAYER");
	CrispyReplaceColor( WORLDDAMAGE, CR_GOLD, "ENVIRONMENT");
	CrispyReplaceColor( MONSTERTRAP, CR_GOLD, "MONSTER");
	CrispyReplaceColor( MONSTERFIGHT, CR_DARK, "INFIGHTING");
	CrispyReplaceColor( HURTSELF, CR_GREEN, "YOURSELF");
	CrispyReplaceColor( MONSTERDIED, CR_GOLD, "MONSTER");
	CrispyReplaceColor( ENVKILL, CR_GREEN, "PLAYER");
	CrispyReplaceColor( KILLENV, CR_GOLD, "DANGER");
	CrispyReplaceColor( SOMEONEBLEW, CR_GREEN, "PLAYER");
	CrispyReplaceColor( YOUBLEW, CR_GREEN, "YOURSELF");
	CrispyReplaceColor( KILLBYPLAYER, CR_GOLD, "KILLED");
	CrispyReplaceColor( KILLPLAYER, CR_GREEN, "PLAYER");

	CrispyReplaceColor( DEMONSPHEREHEALS, CR_BLUE, "DEMONSPHERE");
	CrispyReplaceColor( FOUNDBACKPACK, CR_BLUE, "BACKPACK");
	CrispyReplaceColor( SOULSPHEREHEALS, CR_BLUE, "SOULSPHERE");
	CrispyReplaceColor( MEGAHEALS, CR_GOLD, "MEGASPHERE");

	CrispyReplaceColor( REDSCREENOFF, CR_DARK, "OFF");
	CrispyReplaceColor( REDSCREENSHORT, CR_GRAY, "SHORT");
	CrispyReplaceColor( REDSCREENFULL, CR_GOLD, "FULL");   // redundant since we already colored "FULL" above

	CrispyReplaceColor( "  Weapon Tweaks...", CR_GRAY, "...");
	CrispyReplaceColor( "  Monster Tweaks...", CR_GRAY, "...");

	CrispyReplaceColor( MENUBLACKLIST, CR_GRAY, "...");

	CrispyReplaceColor( SONG_ENABLED, CR_GREEN, SONG_ENABLED);
	CrispyReplaceColor( SONG_DISABLED, CR_GOLD, SONG_DISABLED);
	
	CrispyReplaceColor( SANDBOXTIP, CR_GOLD, "'B'");

	CrispyReplaceColor( ALLMONSTERS, CR_GREEN, "ELIMINATED");

	CrispyReplaceColor( "RANDOM (NO BFG)", CR_DARK, "(NO BFG)");
	CrispyReplaceColor( "RANDOM (BFG)", CR_GREEN, "(BFG)");
	CrispyReplaceColor( "AUTO", CR_GREEN, "AUTO");

	CrispyReplaceColor( "      Bot 1 Weapon: ", CR_GRAY, "Bot 1");
	CrispyReplaceColor( "      Bot 2 Weapon: ", CR_GOLD, "Bot 2");
	CrispyReplaceColor( "      Bot 3 Weapon: ", CR_DARK, "Bot 3");

	CrispyReplaceColor( "    SLOW [PSX]", CR_GRAY, "SLOW [PSX]");
	CrispyReplaceColor( "    FAST [DOS]", CR_GOLD, "FAST [DOS]");

	CrispyReplaceColor( CHANGESBOX, CR_GOLD, "NEXT MAP");
	CrispyReplaceColor( CHANGEFFECT, CR_GOLD, "NEXT MAP");
	CrispyReplaceColor( NM2NEXT, CR_GOLD, "NEXT MAP");

	CrispyReplaceColor( NOWADSTEALING, CR_GOLD, "DOOM 2");

	CrispyReplaceColor( NOMONSTERS, CR_DARK, "NO MONSTERS");
	
	CrispyReplaceColor( GIVEALL, CR_GREEN, "ALL ITEMS");
	 
	CrispyReplaceColor( EKGON, CR_DARK, "LUDICROUS");
	CrispyReplaceColor( EKGOFF, CR_DARK, "DISABLED");

	CrispyReplaceColor( NM2AL, CR_GREEN, "ENABLED");

	CrispyReplaceColor( SKILL1, CR_GOLD, "NEXT MAP");
	CrispyReplaceColor( SKILL2, CR_GOLD, "NEXT MAP");
	CrispyReplaceColor( SKILL3, CR_GOLD, "NEXT MAP");
	CrispyReplaceColor( SKILL4, CR_GOLD, "NEXT MAP");
	CrispyReplaceColor( SKILL5, CR_GOLD, "NEXT MAP");

	CrispyReplaceColor( SKILL_ITYTD, CR_GREEN, "ITYTD");
	CrispyReplaceColor( SKILL_HNTR, CR_GRAY, "HNTR");
	CrispyReplaceColor( SKILL_HMP, CR_BLUE, "HMP");
	CrispyReplaceColor( SKILL_UV, CR_GOLD, "UV");
	CrispyReplaceColor( SKILL_NM, CR_DARK, "NM");

	CrispyReplaceColor( AU_ON, CR_GREEN, "ENABLED");
	CrispyReplaceColor( AU_OFF, CR_DARK, "DISABLED");

	CrispyReplaceColor( WWWCHEAT, CR_GREEN, "ON!");

	CrispyReplaceColor( USEDMEDICAL, CR_GREEN, "ON");

	CrispyReplaceColor( YOUBERSERK, CR_DARK, "BERSERK");

	CrispyReplaceColor( ALREADYFULL, CR_GRAY, "FULL HEALTH");

	CrispyReplaceColor( SKILL_ITYTD, CR_GREEN, "ITYTD");
	CrispyReplaceColor( SKILL_HNTR, CR_GRAY, "HNTR");
	CrispyReplaceColor( SKILL_HMP, CR_BLUE, "HMP");
	CrispyReplaceColor( SKILL_UV, CR_GOLD, "UV");
	CrispyReplaceColor( SKILL_NM, CR_DARK, "NM");
	CrispyReplaceColor( SKILL_ITYTD, CR_GREEN, "ITYTD");
	CrispyReplaceColor( SKILL_HNTR, CR_GRAY, "HNTR");
	CrispyReplaceColor( SKILL_HMP, CR_BLUE, "HMP");
	CrispyReplaceColor( SKILL_UV, CR_GOLD, "UV");
	CrispyReplaceColor( SKILL_NM, CR_DARK, "NM");
	CrispyReplaceColor( SKILL_ITYTD, CR_GREEN, "ITYTD");
	CrispyReplaceColor( SKILL_HNTR, CR_GRAY, "HNTR");
	CrispyReplaceColor( SKILL_HMP, CR_BLUE, "HMP");
	CrispyReplaceColor( SKILL_UV, CR_GOLD, "UV");
	CrispyReplaceColor( SKILL_NM, CR_DARK, "NM");

	CrispyReplaceColor( MIRRORON, CR_GOLD, "ON");
	CrispyReplaceColor( STOPFF, CR_GOLD, "TEAMMATES");

	CrispyReplaceColor( FOUNDBULL, CR_DARK, "BULLETS");
	CrispyReplaceColor( FOUNDBUCK, CR_GOLD, "SHELLS");
	CrispyReplaceColor( FOUNDROCK, CR_GRAY, "ROCKETS");
	CrispyReplaceColor( FOUNDCELL, CR_GREEN, "CELLS");

	CrispyReplaceColor( FFON, CR_GOLD, "ENABLED");
	CrispyReplaceColor( FFOFF, CR_GREEN, "DISABLED");

	CrispyReplaceColor( RESTORESANDBOX, CR_GREEN, "SAVED");

	CrispyReplaceColor( SEARCHING, CR_GRAY, SEARCHING);

	// later on we could colorize these based on bot
	CrispyReplaceColor( ORDERMSGPATROL, CR_GREEN, "ORDER:");
	CrispyReplaceColor( ORDERMSGFOLLOW, CR_GREEN, "ORDER:");
	CrispyReplaceColor( ORDERMSGHOLD, CR_GREEN, "ORDER:");
	CrispyReplaceColor( ORDERSMSGTAKE, CR_GREEN, "ORDER:");
	CrispyReplaceColor( ORDERSMSGASSIGN, CR_GREEN, "ORDER:");

	CrispyReplaceColor( "  Player Name:  ", CR_GREEN, "Player Name");

	CrispyReplaceColor( STATSOK, CR_GREEN, "SAVED");
	CrispyReplaceColor( STATSHINT, CR_GREEN, "PROFILE");

	CrispyReplaceColor( "NORMAL", CR_GRAY, "NORMAL");
	CrispyReplaceColor( "ABRUPT", CR_GOLD, "ABRUPT");

	CrispyReplaceColor( INVRECOVERED, CR_GREEN, "RECOVERED");

	CrispyReplaceColor( "EXTREME", CR_GOLD, "EXTREME");

	CrispyReplaceColor( PHYSON, CR_GREEN, "ENABLED");
	CrispyReplaceColor( PHYSOFF, CR_DARK, "DISABLED");

	CrispyReplaceColor( PHYSMODEOFF, CR_DARK, PHYSMODEOFF);
	CrispyReplaceColor( PHYSMODEONE, CR_GRAY, PHYSMODEONE);
	CrispyReplaceColor( PHYSMODETWO, CR_GOLD, PHYSMODETWO);

	CrispyReplaceColor( LIGHTINGCRT, CR_GRAY, "VINTAGE CRT");
	CrispyReplaceColor( LIGHTINGDOOM3, CR_GOLD, "DOOM 3");

	CrispyReplaceColor( VANILLA, CR_GOLD, "DOOM");
	CrispyReplaceColor( LUDICROUS, CR_GREEN, "LUDICROUS");
	CrispyReplaceColor( GROOVY, CR_GRAY, "GROOVY");
	CrispyReplaceColor( BRUTAL, CR_DARK, "BRUTAL");

	CrispyReplaceColor( "    FAST", CR_GOLD, "FAST");
	CrispyReplaceColor( "    MODERATE", CR_GREEN, "MODERATE");
	CrispyReplaceColor( "    CAREFUL", CR_GRAY, "CAREFUL");

    CrispyReplaceColor( "FULL RANDOM", CR_GOLD, "FULL RANDOM");
    CrispyReplaceColor( "BY TYPE", CR_GRAY, "BY TYPE");
}


static void DrawInventoryItemIcon()
{
	switch (invmenu_selection)
	{
	case RADSUIT_SELECTED:

		V_DrawPatchDirect(INV_ICON_X, INV_ICON_Y+10, W_CacheLumpName(DEH_String("SUITA0"), PU_CACHE));
		break;

	case MEDKIT_SELECTED:

        V_DrawPatchDirect(INV_ICON_X, INV_ICON_Y, W_CacheLumpName(DEH_String("PSTRA0"), PU_CACHE));
		break;

	case INVUL_SELECTED:

        V_DrawPatchDirect(INV_ICON_X, INV_ICON_Y, W_CacheLumpName(DEH_String("PINVA0"), PU_CACHE));

		// TODO: animate it

		break;

	case INVIS_SELECTED:

        V_DrawPatchDirect(INV_ICON_X, INV_ICON_Y+10, W_CacheLumpName(DEH_String("PINSA0"), PU_CACHE));

		// TODO: animate it

		break;

	case VISOR_SELECTED:

        V_DrawPatchDirect(INV_ICON_X, INV_ICON_Y, W_CacheLumpName(DEH_String("PVISA0"), PU_CACHE));

		// TODO: animate it

		break;

	//case AUTOMAP_SELECTED:

	//	V_DrawPatchShadow2(INV_ICON_X, INV_ICON_Y, W_CacheLumpName(DEH_String("PMAPC0"), PU_CACHE));

	//	// TODO: animate it

	//	break;

	default:

		// TODO: erase rect

		break;		
	}
}


// Still a work in progress... getting junk data when returning buf
char* ShowIntAsString(int val)
{
	char buf[8];
	sprintf(buf, "%d", val);

	return DEH_String(buf);
}


void TreasureReadout()  
{
	char show_number[8];
	
	if (Marshmallow_TreasureMode && (invmenu_on || treasure_on))  
	{
		sprintf( show_number, "%d", treasure_bag.score );
		//if (treasure_bag.score > 0) { CrispyReplaceColor(show_number, CR_GREEN, show_number); }  // already being colored zero; probaby by PKE being updated every tic
		HUlib_addMessageToSText(&treasureinfo_line1, DEH_String(SCORELABEL), show_number);

		if (treasure_bag.total_in_level == 0)
		{
			HUlib_addMessageToSText(&treasureinfo_line2, 0, DEH_String(NOTREASURE));
		}
		else
		{
			if (treasure_bag.remaining_in_level == 0 && treasure_bag.total_in_level > 0)
			{
				HUlib_addMessageToSText(&treasureinfo_line2, 0, DEH_String( ALLTREASURE ));
				//HUlib_addMessageToSText(&treasureinfo_line2, 0, DEH_String( " " ));
			}
			else
			{
				sprintf(show_number, "%d", treasure_bag.remaining_in_level);
				HUlib_addMessageToSText(&treasureinfo_line2, DEH_String(REMAINLABEL), show_number );
			}
		}
	}
	else
	{  
	HUlib_addMessageToSText(&treasureinfo_line1, 0, "");
	HUlib_addMessageToSText(&treasureinfo_line2, 0, "");
	}
}


void EraseInfoReadout()  
{
	AddStringsToInfoReadout("", " ", 1);
	AddStringsToInfoReadout("", " ", 2);
	AddStringsToInfoReadout("", " ", 3);
	AddStringsToInfoReadout("", " ", 4);
	AddStringsToInfoReadout("", " ", 5);
	AddStringsToInfoReadout("", " ", 6);
	AddStringsToInfoReadout("", " ", 7);
	AddStringsToInfoReadout("", " ", 8);
}


void SandboxReadout()  
{
	if (!sandbox.design_mode)
		return;

	HUlib_addMessageToSText(&help_sandbox_scroll, 0, DEH_String(HELPLINE_PGUPDN));  // was HELPLINE_BRACKETS
	HUlib_addMessageToSText(&help_sandbox_select, 0, DEH_String(HELPLINE_INSERT));  // was HELPLINE_SLASH
	HUlib_addMessageToSText(&help_sandbox_battle, 0, DEH_String(SANDBOXTIP));
	HUlib_addMessageToSText(&help_sandbox_object, DEH_String(THINGLABEL), ShowThingName());
}


void BotCommandMenu()
{
	CrispyReplaceColor(BOTCOMMANDTITLE, CR_GREEN, "Squad");
	AddStringsToInfoReadout("", DEH_String(BOTCOMMANDTITLE), 1);

	CrispyReplaceColor(FOLLOWME, CR_GOLD, "'F'");
	AddStringsToInfoReadout("", DEH_String(FOLLOWME), 3);

	CrispyReplaceColor(SCATTER, CR_GOLD, "'P'");
	AddStringsToInfoReadout("", DEH_String(SCATTER), 4);

	CrispyReplaceColor(HOLDPOSITION, CR_GOLD, "'H'");
	AddStringsToInfoReadout("", DEH_String(HOLDPOSITION), 5);	
}

void ScoreboardReadout()
{
	//AddToInfoReadout("Player1: ", players[0].cmd.chatchar, 3);
	//AddToInfoReadout("Player2: ", players[1].cmd.chatchar, 4);
	//AddToInfoReadout("Player3: ", players[2].cmd.chatchar, 5);
	//AddToInfoReadout("Player4: ", players[3].cmd.chatchar, 6);	

	//AddToInfoReadout("netgamesignal: ", netgamesignal, 8);
	
	if (menus_on || profilescreen_on || !BotsInGame || botcommandmenu_on)
		return;

	if (deathmatch)  
	{
		// TODO: we need our own hu_stext objects for these three score lines

		if (BotsInGame >= 1)
			AddToInfoReadout(DEH_String(BOT1LABEL), ShowScore(BOT_1), 6);
		//else
		//	AddStringsToInfoReadout("", "", 6);

		if (BotsInGame >= 2)
			AddToInfoReadout(DEH_String(BOT2LABEL), ShowScore(BOT_2), 7);
		//else
		//	AddStringsToInfoReadout("", "", 7);

		if (BotsInGame >= 3)
			AddToInfoReadout(DEH_String(BOT3LABEL), ShowScore(BOT_3), 8);
		//else
		//	AddStringsToInfoReadout("", "", 8);
	}	
	else
	{
		if (!ShowBotStates)
		{
			AddStringsToInfoReadout("", "", 6);
			AddStringsToInfoReadout("", "", 7);
			AddStringsToInfoReadout("", "", 8);

			return;
		}

		{

		/*
		if (BotsInGame >= 1 && Bots[BOT_1].player->mo)
			AddToInfoReadout("BOT_1 node: ", Bots[BOT_1].player->mo->current_node, 1);   // TEMP
		
		if (BotsInGame >= 2 && Bots[BOT_2].player->mo)
			AddToInfoReadout("BOT_2 node: ", Bots[BOT_2].player->mo->current_node, 2);   // TEMP
		
		if (BotsInGame >= 3 && Bots[BOT_3].player->mo)
			AddToInfoReadout("BOT_3 node: ", Bots[BOT_3].player->mo->current_node, 3);   // TEMP
		*/
		//AddToInfoReadout("total_nodes: ", main_path.total_nodes, 4);   // TEMP
		
		//AddToInfoReadout("targetingTimeout: ", Bots[BOT_1].targetingTimeout, 3);
		//AddToInfoReadout("stuck_timeout: ", Bots[BOT_1].stuck_timeout, 4);


		}

		//if (BotsInGame >= 2 && Bots[BOT_2].player->mo)
		//AddToInfoReadout("BOT_2 node: ", Bots[BOT_2].player->mo->current_node, 2);   // TEMP

		//if (BotsInGame >= 3 && Bots[BOT_3].player->mo)
		//AddToInfoReadout("BOT_3 node: ", Bots[BOT_3].player->mo->current_node, 3);   // TEMP

		

		if (BotsInGame >= 1)
		{
			if (gametic % BOTSTATES_UPDATE_INTERVAL == 0)
			botstate[BOT_1] = ShowBotState(BOT_1);
			
			AddStringsToInfoReadout(DEH_String("BOT 1: "), botstate[BOT_1], 6);
		}

		if (BotsInGame >= 2)
		{
			if (gametic % BOTSTATES_UPDATE_INTERVAL == 0)
			botstate[BOT_2] = ShowBotState(BOT_2);
			
			AddStringsToInfoReadout(DEH_String("BOT 2: "), botstate[BOT_2], 7);
		}
		
		if (BotsInGame >= 3)
		{
			if (gametic % BOTSTATES_UPDATE_INTERVAL == 0)
			botstate[BOT_3] = ShowBotState(BOT_3);
			
			AddStringsToInfoReadout(DEH_String("BOT 3: "), botstate[BOT_3], 8);
		}
	}
}




void InitHUDMenuText()							
{
	HUD_InitHelp();
	HUD_InitEnemyMenu();
	HUD_InitWeaponMenu();
	HUD_InitSandboxMenu();
	HUD_InitSkipLevelMenu();
	HUD_InitMusicMenu();
	HUD_InitMainMenu();
	HUD_InitMessagesMenu();
	HUD_InitSkillMenu();
	HUD_InitInventoryMenu();
	HUD_InitBotMenu();
	HUD_InitGameplayMenu();
	HUD_InitMiscText();
	HUD_InitBlacklistMenu();
	HUD_InitVanillaMusicMenu();
	HUD_InitProfileScreen();
    HUD_InitShortcutMenu();

	// So the cursor isn't stuck on zero at first
	mainmenu_selection = FIRST_MENU_ITEM;
}



extern void PKE_HUDisplay();   // Oh?
extern void Draw_Datapad();
extern void Draw_Wallpaper(char* tile);

void DrawHUDMenu()
{
	//if ( gamestate != GS_LEVEL )  // TESTING to prevent datapad crash
	//	return;
	
	if ( profilescreen_on )
		DrawProfile();

	PKE_HUDisplay();  

	if (pkereadout_on)
	{
	//if (Marshmallow_DrawPKEGraphics)  // TODO: Marshmallow_DrawPKEGraphics option
        Draw_Datapad();

	HUlib_drawSText(&pkeline1);
	HUlib_drawSText(&pkeline2);
	HUlib_drawSText(&pkeline3);
	HUlib_drawSText(&pkeline4);
	HUlib_drawSText(&pkeline8);
	}

	if (mainmenu_on)
	{
        Draw_Datapad();  // first so anything below draws over it
	}
	else if (menus_on)
	{
		int game = GetGameType();

		if (game != DOOM2)
			Draw_Wallpaper(MENU_WALLPAPER);  // blue techbase texture
	}

	if (menus_on)  
	{
		if (invmenu_on)
		{ 
			if (invmenu_selection != NO_SELECTION
				&& !realnetgame)
					HUlib_drawSText(&menu_cursor);		// handle cursor for inventory menu
		}
		else
		{
			HUlib_drawSText(&menu_cursor);			// handle cursor for all other menus
		}
	}

	if (invmenu_on)  
	{
		DrawInventoryItemIcon();
	}

	if (botcommandmenu_on)
		BotCommandMenu();

	if (missilelock_on)
	{
		HUlib_drawSText(&missilelock_text);
	}

	if (Marshmallow_TreasureMode)
	{
		if (treasure_bag.total_in_level > 0)  // only show if level actually has treasure to find
			HUlib_drawSText(&treasureinfo_line1);

		HUlib_drawSText(&treasureinfo_line2);
	}

	if (targethp_on
		&& Marshmallow_ShowTargetHP
		&& MAIN_PLAYER.mo->player->victim 
		&& !invmenu_on)  
	{
		if (Marshmallow_ShowTargetHP == 2)	 // 2 = bosses only mode
		{
			if (MAIN_PLAYER.mo->target)
			{
			if (MAIN_PLAYER.mo->target->type == MT_CYBORG
				|| MAIN_PLAYER.mo->target->type == MT_SPIDER)  // any battle with Cyberdemon or Spiderboss will show targethp
					HUlib_drawSText(&targethp_readout);

			if (MAIN_PLAYER.mo->target->type == MT_BRUISER
				&& gamemode != commercial
				&& E1M8)										// targethp for barons will only work in E1M8
					HUlib_drawSText(&targethp_readout);
			}
		}
		else
			HUlib_drawSText(&targethp_readout);
	}
	
	if (viewingbot && !menus_on && !sandbox.design_mode && !profilescreen_on && !deathmatch)
	{
		HUlib_drawSText(&botreadout_health);
		HUlib_drawSText(&botreadout_state);

		if (Bots[viewingbot].msgtimeout)
			HUlib_drawSText(&botreadout_chat);
	}
	
	if (miscreadout_on || Marshmallow_InfoReadout) 
	{
		int i;

        // Don't show when main menu is up
		if (menuactive)
		    return;

		for (i = 0;i<MAX_INFOLINES;i++)
			HUlib_drawSText(&miscreadout_output[i]); 
	}

	if (ShowBotStates && !miscreadout_on && !menus_on)
	{
		if (BotsInGame >= 1)
		HUlib_drawSText(&miscreadout_output[5]);

		if (BotsInGame >= 2)
		HUlib_drawSText(&miscreadout_output[6]);

		if (BotsInGame >= 3)
		HUlib_drawSText(&miscreadout_output[7]);
	}

	if (/*menus_on || */help_on)
	{
		HUlib_drawSText(&help_title); 
		HUlib_drawSText(&help_mainmenu); 
		HUlib_drawSText(&help_inventory); 
		HUlib_drawSText(&help_pkemeter);
        HUlib_drawSText(&help_pkerange);
        HUlib_drawSText(&help_botfollow);
		HUlib_drawSText(&help_botassign); 
		HUlib_drawSText(&help_botusekey); 
		HUlib_drawSText(&help_botshiftkey); 
		//HUlib_drawSText(&help_dropgift); 
		//HUlib_drawSText(&help_togglehelp); 
			
		//if (!realnetgame)
		//{
		//HUlib_drawSText(&help_menu); 
		//HUlib_drawSText(&help_scroll); 
		//HUlib_drawSText(&help_select); 
		//}
	}

	if (sandboxhelp_on && !menus_on && !profilescreen_on && !help_on)
	{
		HUlib_drawSText(&help_sandbox_object); 
		HUlib_drawSText(&help_sandbox_scroll); 
		HUlib_drawSText(&help_sandbox_select); 

		if (consoleplayer == 0)
		HUlib_drawSText(&help_sandbox_battle); 
	}

	if (enemymenu_on)
	{
		HUlib_drawSText(&enemymenu_fast); 
		
		HUlib_drawSText(&enemymenu_lostsoul); 
		HUlib_drawSText(&enemymenu_caco); 
		HUlib_drawSText(&enemymenu_demon); 
		HUlib_drawSText(&enemymenu_cacosound); 

		if (gamemode == commercial || Marshmallow_WadStealing)
		{
		HUlib_drawSText(&enemymenu_cguysnd); 
		HUlib_drawSText(&enemymenu_skelmissile); 
		HUlib_drawSText(&enemymenu_skel);
		HUlib_drawSText(&enemymenu_paindeath); 
		HUlib_drawSText(&enemymenu_vilepain); 
		HUlib_drawSText(&enemymenu_vilez); 
		}
	}

	if (weaponmenu_on)
	{
		HUlib_drawSText(&weaponmenu_truerandom); 

		HUlib_drawSText(&weaponmenu_bersauto); 
		HUlib_drawSText(&weaponmenu_bersred); 
		HUlib_drawSText(&weaponmenu_csaw); 
		HUlib_drawSText(&weaponmenu_fist); 
		HUlib_drawSText(&weaponmenu_bullet); 
		HUlib_drawSText(&weaponmenu_infammo); 
		HUlib_drawSText(&weaponmenu_shotgun); 
		//HUlib_drawSText(&weaponmenu_plasma);
		//HUlib_drawSText(&weaponmenu_bfg);
	}

	if (sandboxmenu_on)
	{
		HUlib_drawSText(&sandboxmenu_map); 

		//if (skip_to_level)
		HUlib_drawSText(&sandboxmenu_go); 

		//if (sandbox.count)
		HUlib_drawSText(&sandboxmenu_reset); 
		//HUlib_drawSText(&sandboxmenu_cancel); 

	}

	if (mainmenu_on)					
	{
        // TODO: draw "Send Message" at bottom

		HUlib_drawSText(&mainmenu_profile); 
		HUlib_drawSText(&mainmenu_options); 
		HUlib_drawSText(&mainmenu_botmenu); 
		HUlib_drawSText(&mainmenu_nextmap);
		HUlib_drawSText(&mainmenu_sandbox); 
		HUlib_drawSText(&mainmenu_music); 
		HUlib_drawSText(&mainmenu_messages);
		//HUlib_drawSText(&mainmenu_suicide);   // No room for this at the moment
	}

	if (skipmenu_on)
	{
		HUlib_drawSText(&skipmenu_next); 
		HUlib_drawSText(&skipmenu_redo); 
		HUlib_drawSText(&skipmenu_random); 
		HUlib_drawSText(&skipmenu_jump); 
		HUlib_drawSText(&skipmenu_skill); 
		HUlib_drawSText(&skipmenu_weapons); 
		HUlib_drawSText(&skipmenu_upgrade); 
		HUlib_drawSText(&skipmenu_scalehp); 
		HUlib_drawSText(&skipmenu_itemspawns);

        //	if (skip_to_level)
			HUlib_drawSText(&skipmenu_go);

        if (upgrade_chance > 0)
            HUlib_drawSText(&skipmenu_percent);

        if (MonsterHitpointsScale > 1)
            HUlib_drawSText(&skipmenu_multiplier);
	}

	if (vanillamusicmenu_on)  // Displayed when dynamic music is disabled
	{
		HUlib_drawSText(&musicmode_vanilla); 
		HUlib_drawSText(&songblacklist_vanilla); 	
	}
	
	if (musicmenu_on)   // Displayed when dynamic music is enabled
	{
		if (Marshmallow_DynamicMusic)
		{
			HUlib_drawSText(&musicmenu_mode); 
			HUlib_drawSText(&musicmenu_skip);
			HUlib_drawSText(&musicmenu_playlist); 	
			HUlib_drawSText(&musicmenu_msgs); 
			HUlib_drawSText(&musicmenu_length); 
			HUlib_drawSText(&musicmenu_fastchanges); 

			if (Marshmallow_WadStealing)
				HUlib_drawSText(&musicmenu_game);

			HUlib_drawSText(&songblacklist_marshmallow); 
		}
		else
		{
			HUlib_drawSText(&musicmode_vanilla); 
			HUlib_drawSText(&songblacklist_vanilla); 
		}
	}

	if (blacklistmenu_on)
	{
		HUlib_drawSText(&songblacklist_runnin); 
		HUlib_drawSText(&songblacklist_e1m1); 
		HUlib_drawSText(&songblacklist_d1inter); 
		HUlib_drawSText(&songblacklist_d2inter); 
	}

	if (botmenu_on)
	{
		HUlib_drawSText(&botmenu_title); 
		HUlib_drawSText(&botmenu_blank); 
		HUlib_drawSText(&botmenu_spawn); 
		HUlib_drawSText(&botmenu_spawnall); 
		HUlib_drawSText(&botmenu_kill); 
		//HUlib_drawSText(&botmenu_states); 
		HUlib_drawSText(&botmenu_remove); 	
		HUlib_drawSText(&botmenu_bot1weapon); 	
		HUlib_drawSText(&botmenu_bot2weapon); 	
		HUlib_drawSText(&botmenu_bot3weapon); 	

		if (!deathmatch)
		{
		HUlib_drawSText(&botmenu_friendlyfire); 	
		HUlib_drawSText(&botmenu_speed); 	
		HUlib_drawSText(&botmenu_statereadout); 
		}

		//HUlib_drawSText(&botmenu_collision); 	
	}

	if (msgsmenu_on)
	{
		HUlib_drawSText(&messagesmenu_title); 
		HUlib_drawSText(&messagesmenu_blank); 
		HUlib_drawSText(&messagesmenu_danger); 
		HUlib_drawSText(&messagesmenu_targethp); 
		HUlib_drawSText(&messagesmenu_missile); 
		//HUlib_drawSText(&messagesmenu_extraline);
		HUlib_drawSText(&messagesmenu_pickupmsg); 
		HUlib_drawSText(&messagesmenu_deathmsg); 		
		//HUlib_drawSText(&messagesmenu_damagemsg); 
		HUlib_drawSText(&messagesmenu_extendedmsg);
		HUlib_drawSText(&messagesmenu_infightmsg); 
		HUlib_drawSText(&messagesmenu_bossmsg); 
	}

	if (optionsmenu_on)
	{
		HUlib_drawSText(&optionsmenu_title); 
		HUlib_drawSText(&optionsmenu_blank); 

		HUlib_drawSText(&optionsmenu_conserve); 
		HUlib_drawSText(&optionsmenu_keepitems); 
		HUlib_drawSText(&optionsmenu_goodies);
		HUlib_drawSText(&optionsmenu_treasure); 
		HUlib_drawSText(&optionsmenu_ludicrous); 
		HUlib_drawSText(&optionsmenu_lighting); 
		HUlib_drawSText(&optionsmenu_physics); 

		HUlib_drawSText(&optionsmenu_weapons); 
		HUlib_drawSText(&optionsmenu_monsters);

		HUlib_drawSText(&optionsmenu_autouse); 
		HUlib_drawSText(&optionsmenu_selfdmg); 
		HUlib_drawSText(&optionsmenu_keepweapons); 

		if (netgame)
		{
		HUlib_drawSText(&optionsmenu_keepkeys); 
		HUlib_drawSText(&optionsmenu_weaponstay); 
		}
	}

	if (shortcutmenu_on)
    {
        HUlib_drawSText(&shortcutmenu_gameplay);
        HUlib_drawSText(&shortcutmenu_music);
        HUlib_drawSText(&shortcutmenu_messages);
    }

	if (skillmenu_on)
	{
		//HUlib_drawSText(&skillmenu_title); 
		HUlib_drawSText(&skillmenu_blank); 
		HUlib_drawSText(&skillmenu_baby); 
		HUlib_drawSText(&skillmenu_easy); 
		HUlib_drawSText(&skillmenu_medium); 
		HUlib_drawSText(&skillmenu_hard); 
		HUlib_drawSText(&skillmenu_nightmare); 
		HUlib_drawSText(&skillmenu_nightmare2); 

		if (gameskill == sk_nightmare)
		HUlib_drawSText(&skillmenu_nmrespawn); 
	}

	if (hellscape_on && !menuactive && !deathmatch)
	{
		HUlib_drawSText(&hellscape_text); 
	}

	if (extra_textline_on)				
		HUlib_drawSText(&extra_textline);   

	if (invmenu_on)				
	{
		int i;
		boolean nopowers = true;

		//HUlib_drawSText(&inv_menu_title); 
		//HUlib_drawSText(&inv_menu_blank); 

		for (i=0;i<=MAX_INV_ITEMS;i++)  
		{
			if (MAIN_PLAYER.extra_powers[i])
			{
				nopowers = false;
				break;
			}
		}

		if (nopowers)
			HUlib_drawSText(&inv_menu_empty); 

		if (MAIN_PLAYER.extra_powers[ITEM_RADSUIT])
			HUlib_drawSText(&inv_menu_radsuit); 

		if (MAIN_PLAYER.extra_powers[ITEM_INVUL])
			HUlib_drawSText(&inv_menu_invul); 

		if (MAIN_PLAYER.extra_powers[ITEM_INVIS])
			HUlib_drawSText(&inv_menu_invis); 

		if (MAIN_PLAYER.extra_powers[ITEM_MEDKIT])
			HUlib_drawSText(&inv_menu_medkit); 

		if (MAIN_PLAYER.extra_powers[ITEM_VISOR])
			HUlib_drawSText(&inv_menu_visor); 

		//if (MAIN_PLAYER.extra_powers[ITEM_AUTOMAP])
		//	HUlib_drawSText(&inv_menu_automap); 
	}
}


void HUDMenuTicker()
{
	char string1[4];
	char string2[4];
	int upgrade = upgrade_chance;
	int scale = MonsterHitpointsScale;

	if (mainmenu_on)
	{
		cursor_x = CURSOR_X_DATAPAD;
	}
	else
	{
		cursor_x = CURSOR_X_MENUS;
	}

//  pke readout

	if (pkereadout_on)
		PKE_Readout();

//	turn cursor on or off

	StartMenuCursor();

// bot stats readout

	UpdateBotReadouts();

// info readouts

	miscreadout_on = true; // temporary

	UpdateInfoReadout();

	TreasureReadout();
	SandboxReadout();
	ScoreboardReadout();

// missile lock notification

	HUlib_addMessageToSText(&missilelock_text, DEH_String(MISSILELOCK1), DEH_String(MISSILELOCK2));  

// cursor
	CalculateCursorPosition();

	HUlib_addMessageToSText(&menu_cursor, 0, DEH_String(MENU_CURSOR));

	
	HUlib_addMessageToSText(&mainmenu_profile, 0, "Profile");
	HUlib_addMessageToSText(&mainmenu_options, 0, DEH_String("GAMEPLAY") /*, DisplayOnOff(Marshmallow_DynamicMusic)*/);
	HUlib_addMessageToSText(&mainmenu_botmenu, 0, DEH_String(MENU_BOTS));
	HUlib_addMessageToSText(&mainmenu_sandbox, 0, DEH_String("Sandbox"));
	HUlib_addMessageToSText(&mainmenu_nextmap, 0, DEH_String(MENU_SKIP));
	
	HUlib_addMessageToSText(&mainmenu_music, 0, DEH_String(MENU_MUSIC) /*, DisplayOnOff(Marshmallow_DynamicMusic)*/);
	HUlib_addMessageToSText(&mainmenu_messages, 0, DEH_String(MENU_MSGS));
	HUlib_addMessageToSText(&mainmenu_suicide, 0, DEH_String(MENU_SEPPUKU));

// sandbox menu

	HUlib_addMessageToSText(&sandboxmenu_map, DEH_String(WHICHMAP), ShowMapSelection() );
	HUlib_addMessageToSText(&sandboxmenu_go, 0, DEH_String(GOBUTTON) );
	HUlib_addMessageToSText(&sandboxmenu_reset, 0, DEH_String(RESETBUTTON) );
	
// skip level menu

	HUlib_addMessageToSText(&skipmenu_next, 0, "    Skip to Next Level");
	HUlib_addMessageToSText(&skipmenu_redo, 0, "    Restart This Level");
	HUlib_addMessageToSText(&skipmenu_random, 0, DEH_String(RANDOMLEVEL));

	if ( skipmenu_on || sandboxmenu_on) 
		ColorizeMapNames();

	HUlib_addMessageToSText(&skipmenu_jump, "    Skip To Level:  ", ShowMapSelection() );

	HUlib_addMessageToSText(&skipmenu_weapons, "    Starting Arsenal: ", ShowMapWeapons() );

	HUlib_addMessageToSText(&skipmenu_skill, "    Skill Level: ", ShowSkillLevel() );

	upgrade = upgrade_chance;

    CrispyReplaceColor("x", CR_GREEN, "x");
    CrispyReplaceColor("%", CR_GREEN, "%");

    HUlib_addMessageToSText(&skipmenu_multiplier, 0, DEH_String( "x" ) );
    HUlib_addMessageToSText(&skipmenu_percent, 0, DEH_String("%") );
	
	sprintf( string1, "%d", upgrade );
	sprintf( string2, "%d", scale );

	// Color the numbers green
	CrispyReplaceColor(string1, CR_GREEN, string1);
	CrispyReplaceColor(string2, CR_GREEN, string2);

	if (upgrade > 0)
	    HUlib_addMessageToSText(&skipmenu_upgrade, DEH_String("    Monster Upgrade Chance: "), DEH_String(string1) );
    else
        HUlib_addMessageToSText(&skipmenu_upgrade, DEH_String("    Monster Upgrade Chance: "), DEH_String("OFF") );

	if (scale > 1)
	    HUlib_addMessageToSText(&skipmenu_scalehp, DEH_String("    Monster Hitpoints Multiplier: "), DEH_String(string2) );
    else
        HUlib_addMessageToSText(&skipmenu_scalehp, DEH_String("    Monster Hitpoints Multiplier: "), DEH_String("OFF") );

	HUlib_addMessageToSText(&skipmenu_itemspawns, "    Random Item Pickups:   ", ShowRandomItemsMode());

	HUlib_addMessageToSText(&skipmenu_go, 0, DEH_String(GOBUTTON));


// bot menu

	//HUlib_addMessageToSText(&botmenu_title, 0, "  Marshmallow Bot Menu");
	//HUlib_addMessageToSText(&botmenu_blank, 0, " ");  // so... what's the point of this again?

	HUlib_addMessageToSText(&botmenu_spawn, 0, "    Add A Bot");
	HUlib_addMessageToSText(&botmenu_spawnall, 0, "    Spawn All Bots");
	HUlib_addMessageToSText(&botmenu_remove, 0, "    Remove All Bots");
	HUlib_addMessageToSText(&botmenu_kill, 0, "    Kill All Bots");

	//HUlib_addMessageToSText(&botmenu_states, "    Show Bot Readout", DisplayOnOff(Marshmallow_ShowBotReadout));

	HUlib_addMessageToSText(&botmenu_bot1weapon, DEH_String("      Bot 1 Weapon: "), ShowBotWeapon(1) );
	HUlib_addMessageToSText(&botmenu_bot2weapon, DEH_String("      Bot 2 Weapon: "), ShowBotWeapon(2) );
	HUlib_addMessageToSText(&botmenu_bot3weapon, DEH_String("      Bot 3 Weapon: "), ShowBotWeapon(3) );

	HUlib_addMessageToSText(&botmenu_friendlyfire, "    Friendly Fire: ", DisplayOnOff(Marshmallow_FriendlyFire) );

	HUlib_addMessageToSText(&botmenu_speed, "    Bot Walk Speed: ", DisplayBotSpeed() );

	HUlib_addMessageToSText(&botmenu_statereadout, "    Bot States Widget: ", DisplayOnOff(ShowBotStates) );

	//HUlib_addMessageToSText(&botmenu_collision, "    Team Collision: ", DisplayOnOff(Marshmallow_PlayerCollision) );

	//HUlib_addMessageToSText(&botmenu_gamemode, "    AI Mode: ", ShowGameMode() );


// options menu

	// title ?

	HUlib_addMessageToSText(&optionsmenu_conserve, "    Conserve Health/Ammo Pickups", DisplayOnOff(Marshmallow_ConservePowerups));
	HUlib_addMessageToSText(&optionsmenu_keepitems, "    Keep Special Items", DisplayOnOff(Marshmallow_SaveItems));
	HUlib_addMessageToSText(&optionsmenu_goodies, "    Monsters Drop Goodies", DisplayOnOff(Marshmallow_DropGoodies));
	HUlib_addMessageToSText(&optionsmenu_treasure, "    Treasure Items", DisplayTreasureMode());
	HUlib_addMessageToSText(&optionsmenu_ludicrous, "    Blood and Gibs", DisplayGibMode());
	HUlib_addMessageToSText(&optionsmenu_physics, "    Enhanced Physics", DisplayPhysicsMode());
	HUlib_addMessageToSText(&optionsmenu_lighting, "    Darkened Lighting", DisplayLightingMode());
	
	HUlib_addMessageToSText(&optionsmenu_weapons, 0, DEH_String("  Weapon Tweaks..."));
	HUlib_addMessageToSText(&optionsmenu_monsters, 0, DEH_String("  Monster Tweaks..."));

	HUlib_addMessageToSText(&optionsmenu_autouse, "    Auto-Use", DisplayOnOff(Marshmallow_AutoUse));
	HUlib_addMessageToSText(&optionsmenu_selfdmg, "    Self-Damage", DisplayOnOff(Marshmallow_SelfDamage));
	HUlib_addMessageToSText(&optionsmenu_keepweapons, "    Keep Weapons from Last Level", DisplayOnOff(Marshmallow_KeepWeapons));
	HUlib_addMessageToSText(&optionsmenu_keepkeys, "    Keep Keys On Death", DisplayOnOff(Marshmallow_KeepKeys));

	HUlib_addMessageToSText(&optionsmenu_weaponstay, "    Weapons Stay", DisplayOnOff(Marshmallow_WeaponsStay));

// shortcut menu

    HUlib_addMessageToSText(&shortcutmenu_gameplay, NULL, DEH_String("    Gameplay options..."));
    HUlib_addMessageToSText(&shortcutmenu_music, NULL, DEH_String("    Music options..."));
    HUlib_addMessageToSText(&shortcutmenu_messages, NULL, DEH_String("    MESSAGES options..."));


// skill menu

	HUlib_addMessageToSText(&skillmenu_title, 0, "    Select New Skill Level");
	HUlib_addMessageToSText(&skillmenu_blank, 0, " ");  // so... what's the point of this again?

	HUlib_addMessageToSText(&skillmenu_baby, 0, "    I'm Too Young To Die");
	HUlib_addMessageToSText(&skillmenu_easy, 0, "    Hey Not Too Rough");
	HUlib_addMessageToSText(&skillmenu_medium, 0, "    Hurt Me Plenty");
	HUlib_addMessageToSText(&skillmenu_hard, 0, "    Ultra Violence");
	HUlib_addMessageToSText(&skillmenu_nightmare, 0, "    Nightmare");
	HUlib_addMessageToSText(&skillmenu_nightmare2, 0, "    Nightmare 2.0");   // on/off display?

	HUlib_addMessageToSText(&skillmenu_nmrespawn, "      Respawning Monsters", DisplayOnOff(Marshmallow_RespawnInNightmare));   


// inventory menu

	HUlib_addMessageToSText(&inv_menu_title, 0, DEH_String(INVENTORYTITLE));
	HUlib_addMessageToSText(&inv_menu_blank, 0, " ");  // so... what's the point of this again?

	if (!realnetgame)  
	HUlib_addMessageToSText(&inv_menu_empty, 0, DEH_String(INVEMPTY));

	HUlib_addMessageToSText(&inv_menu_radsuit, 0, DEH_String(INVRADSUIT) );
	HUlib_addMessageToSText(&inv_menu_invul, 0, DEH_String(INVINVUL) );
	HUlib_addMessageToSText(&inv_menu_invis, 0, DEH_String(INVINVIS) );
	HUlib_addMessageToSText(&inv_menu_medkit, DEH_String(INVMEDKIT), BLANK_LINE);
	HUlib_addMessageToSText(&inv_menu_visor, 0, DEH_String(INVGOGGLES) );
	//HUlib_addMessageToSText(&inv_menu_automap, 0, DEH_String(INVAUTOMAP) );

	//HUlib_addMessageToSText(&inv_menu_treasure, DEH_String("   TREASURE SCORE: "), DisplayValue(MAIN_PLAYER.score));  // should no longer be needed


// messages menu

	// no title?

	HUlib_addMessageToSText(&messagesmenu_danger, DEH_String(MENU_HELLSCAPE), DisplayOnOff(hellscape_on)); 
	HUlib_addMessageToSText(&messagesmenu_missile, "    MISSILE-LOCK ALARM", DisplayOnOff(Marshmallow_MissileAlert));
	HUlib_addMessageToSText(&messagesmenu_pickupmsg, "    ITEM PICKUP MESSAGES", DisplayOnOff(Marshmallow_PickupMessages));
	HUlib_addMessageToSText(&messagesmenu_targethp, "    SHOW TARGET HITPOINTS", DisplayTargetHPSetting(Marshmallow_ShowTargetHP)); 
	HUlib_addMessageToSText(&messagesmenu_deathmsg, "    DEATH MESSAGES", DisplayOnOff(Marshmallow_DeathMessages));
	//HUlib_addMessageToSText(&messagesmenu_damagemsg, "    DAMAGE MESSAGES", DisplayOnOff(Marshmallow_DamageMessages));
	HUlib_addMessageToSText(&messagesmenu_extendedmsg, "    POINTLESS MESSAGES", DisplayOnOff(Marshmallow_ExtendedMessages));
	HUlib_addMessageToSText(&messagesmenu_extraline, "    EXTRA TEXT LINE", DisplayOnOff(Marshmallow_ExtraTextLines));
	HUlib_addMessageToSText(&messagesmenu_bossmsg, "    BOSS ALERTS", DisplayOnOff(Marshmallow_BossAlert));
	HUlib_addMessageToSText(&messagesmenu_infightmsg, "    MONSTER INFIGHTING ALERT", DisplayOnOff(Marshmallow_InfightAlert));

// music menu

	// no title?

	//HUlib_addMessageToSText(&musicmenu_title, 0 , "    MUSIC OPTIONS");  
	
	HUlib_addMessageToSText(&musicmenu_mode, "    MUSIC MODE", DisplayMusicMode() );   

//	if (modifiedgame)  // wrong song names are shown in TNT and Plutonia
		HUlib_addMessageToSText(&musicmenu_skip, "    SONG              ", songnames[Doom_DJ.current_song] );

	//HUlib_addMessageToSText(&musicmenu_shuffle, "    SHUFFLE       ", DisplayOnOff(Doom_DJ.shuffle));
	HUlib_addMessageToSText(&musicmenu_msgs, "    MESSAGES     ", DisplayOnOff(Marshmallow_DJMessages));
	HUlib_addMessageToSText(&musicmenu_length, "    SONG LENGTH ",  ShowSongLength() );  
	HUlib_addMessageToSText(&musicmenu_fastchanges, "    MUSIC CHANGES  ",  ShowSongChanges() );  

	HUlib_addMessageToSText(&musicmenu_playlist, "    MOOD                 ", DisplayPlaylistMode() );
	
	if (Marshmallow_WadStealing)
		HUlib_addMessageToSText(&musicmenu_game, "    GAME          " , ShowGameType() );	

	HUlib_addMessageToSText(&musicmode_vanilla, "    MUSIC MODE", DisplayMusicMode() );  

	HUlib_addMessageToSText(&songblacklist_vanilla, 0,  DEH_String(MENUBLACKLIST) );  
	HUlib_addMessageToSText(&songblacklist_marshmallow, 0,  DEH_String(MENUBLACKLIST) );  

// blacklist menu

	HUlib_addMessageToSText(&songblacklist_runnin, "    RUNNING FROM EVIL [MAP01]  ", DisplaySongStatus( song_blacklist[BLACKLIST_RUNNIN] ) );
	HUlib_addMessageToSText(&songblacklist_e1m1, "    AT DOOM'S GATE [E1M1]          ", DisplaySongStatus( song_blacklist[BLACKLIST_E1M1] ) );
	HUlib_addMessageToSText(&songblacklist_d1inter, "    DOOM INTERMISSION THEME      ", DisplaySongStatus( song_blacklist[BLACKLIST_D1INTER] ) );
	HUlib_addMessageToSText(&songblacklist_d2inter, "    DOOM2 INTERMISSION THEME    ",  DisplaySongStatus( song_blacklist[BLACKLIST_D2INTER] ) );  

// weapon menu

	HUlib_addMessageToSText(&weaponmenu_truerandom, "    TRUE RANDOM DAMAGE", DisplayOnOff(Marshmallow_TrueRandomDamage));
	
	HUlib_addMessageToSText(&weaponmenu_bersauto, "    BERSERK AUTO-SWITCH", DisplayOnOff(Marshmallow_BerserkAutoSwitch));
	HUlib_addMessageToSText(&weaponmenu_bersred, "    BERSERK REDSCREEN", DisplayRedScreenMode() ); 
	HUlib_addMessageToSText(&weaponmenu_csaw, "    BOOSTED CHAINSAW", DisplayOnOff(Marshmallow_BoostedChainsaw));
	HUlib_addMessageToSText(&weaponmenu_fist, "    BOOSTED FIST", DisplayOnOff(Marshmallow_BoostedFist));
	HUlib_addMessageToSText(&weaponmenu_bullet, "    BOOSTED GUNSHOT", DisplayOnOff(Marshmallow_BoostedBullets));
	HUlib_addMessageToSText(&weaponmenu_infammo, "    INFINITE PISTOL AMMO", DisplayOnOff(Marshmallow_InfinitePistol));
	HUlib_addMessageToSText(&weaponmenu_shotgun, "    BALANCE SHOTGUNS", DisplayOnOff(Marshmallow_BalanceShotguns));
	HUlib_addMessageToSText(&weaponmenu_plasma, "    PLASMA SPUTTER", DisplayOnOff(Marshmallow_PlasmaSlowdown));
	HUlib_addMessageToSText(&weaponmenu_bfg, "    BFG SHOCKWAVE", DisplayOnOff(Marshmallow_BFGBlastWave));

// enemy menu

	HUlib_addMessageToSText(&enemymenu_fast, "    FAST MONSTERS", DisplayOnOff(fastparm));
	
	HUlib_addMessageToSText(&enemymenu_lostsoul, "    LOST SOULS HAVE LESS HEALTH", DisplayOnOff(Marshmallow_NerfHP_LostSoul));
	HUlib_addMessageToSText(&enemymenu_caco, "    CACODEMONS HAVE LESS HEALTH", DisplayOnOff(Marshmallow_NerfHP_Cacodemon));
	HUlib_addMessageToSText(&enemymenu_demon, "    DEMONS HAVE LESS HEALTH", DisplayOnOff(Marshmallow_NerfHP_Demons));
	HUlib_addMessageToSText(&enemymenu_cacosound, "    CACODEMON MELEE ATTACK SOUND", DisplayOnOff(Marshmallow_CacoMeleeSound));

	HUlib_addMessageToSText(&enemymenu_cguysnd, "    CHAINGUNGUY ATTACK SOUND", DisplayCGuySnd(Marshmallow_FixChainguySound));
	HUlib_addMessageToSText(&enemymenu_skelmissile, "    REVENANT MISSILE SPEED", DisplayRevMissileSpeed());
	HUlib_addMessageToSText(&enemymenu_skel, "    REVENANTS WALK SLOWER", DisplayOnOff(Marshmallow_NerfSpeed_Undead));
	HUlib_addMessageToSText(&enemymenu_paindeath, "    NO PAIN ELEMENTAL DEATH SPAWNS", DisplayOnOff(Marshmallow_AltPainDeath));
	HUlib_addMessageToSText(&enemymenu_vilepain, "    ARCHVILE PAINCHANCE NERF", DisplayOnOff(Marshmallow_NerfPC_Archvile));     
	HUlib_addMessageToSText(&enemymenu_vilez, "    ARCHVILE Z SCOPE LIMIT", DisplayOnOff(Marshmallow_VileZScopeLimit));     

// help

	HUlib_addMessageToSText(&help_title, 0, DEH_String(HELPTITLE));

	HUlib_addMessageToSText(&help_mainmenu, 0, DEH_String(HELPLINE1));
	HUlib_addMessageToSText(&help_pkemeter, 0, DEH_String(HELPLINE3));
    HUlib_addMessageToSText(&help_pkerange, 0, DEH_String(HELPLINE_RANGE));
	HUlib_addMessageToSText(&help_inventory, 0, DEH_String(HELPLINE2));
	
	HUlib_addMessageToSText(&help_botfollow, 0, DEH_String(HELPLINE4));
	HUlib_addMessageToSText(&help_botassign, 0, DEH_String(HELPLINE5));
	HUlib_addMessageToSText(&help_botusekey, DEH_String(TIP), DEH_String(HELPLINE6));
	HUlib_addMessageToSText(&help_botshiftkey, DEH_String(TIP), DEH_String(HELPLINE8));

	//HUlib_addMessageToSText(&help_dropgift, 0, DEH_String(HELPLINE7));

	//HUlib_addMessageToSText(&help_togglehelp, 0, DEH_String(HELPLINE9));
	

// additional HUD reminders/functions:

	ShowTargetHP();
	LowAmmoWarning();   
	LowHealthWarning(); 
	BerserkReminder();  
}



void CalculateCursorPosition()
{
	/*
	TODO: loop?

	for (i=0;i<MAX_MAINMENU_ITEMS;i++)
	{
		if (i != mainmenu_selection)
			continue;
		// else...
	}
	*/

	if (shortcutmenu_on)
    {
        switch (shortcutmenu_selection)
        {
            case 1:
                cursor_y = INV_HU_Y_5;
                break;

            case 2:
                cursor_y = INV_HU_Y_6;
                break;

            case 3:
                cursor_y = INV_HU_Y_7;
                break;

            default:
                return;
        }
    }

	if (enemymenu_on)
	{  
	  switch (enemymenu_selection)
	  {  
		case 1:
			cursor_y = INV_HU_Y_4;   
			break;
		case 2:
			cursor_y = INV_HU_Y_6;
			break;
		case 3:
			cursor_y = INV_HU_Y_7;
			break;
		case 4:
			cursor_y = INV_HU_Y_8;
			break;
		case 5:
			cursor_y = INV_HU_Y_9;
			break;
		case 6:
			cursor_y = INV_HU_Y_11;
			break;
		case 7:
			cursor_y = INV_HU_Y_12;
			break;
		case 8:
			cursor_y = INV_HU_Y_13;
			break;
		case 9:
			cursor_y = INV_HU_Y_14;
			break;
		case 10:
			cursor_y = INV_HU_Y_15;
			break;
		case 11:
			cursor_y = INV_HU_Y_16;
			break;

		default:
			return;
	  }
	}

	if (weaponmenu_on)
	{  
	  switch (weaponmenu_selection)
	  {  
		case 1:
			cursor_y = INV_HU_Y_4;   
			break;
		case 2:
			cursor_y = INV_HU_Y_6;
			break;
		case 3:
			cursor_y = INV_HU_Y_7;
			break;
		case 4:
			cursor_y = INV_HU_Y_8;
			break;
		case 5:
			cursor_y = INV_HU_Y_9;
			break;
		case 6:
			cursor_y = INV_HU_Y_10;
			break;
		case 7:
			cursor_y = INV_HU_Y_11;
			break;
		case 8:
			cursor_y = INV_HU_Y_12;
			break;
		case 9:
			cursor_y = INV_HU_Y_13;
			break;
		case 10:
			cursor_y = INV_HU_Y_14;
			break;

		default:
			return;
	  }
	}

	if (sandboxmenu_on)
	{
	switch (sandboxmenu_selection)
		{
		case 1:
			cursor_y = INV_HU_Y_3;
			break;
		case 2:
			cursor_y = INV_HU_Y_5;
			break;
		case 3:
			cursor_y = INV_HU_Y_8;
			break;
		//case 4:
		//	cursor_y = INV_HU_Y_10;
		//	break;

		default:
			return;
		}
	}

	if (skipmenu_on)
	{
	switch (skipmenu_selection)
		{
		case 1:
			cursor_y = INV_HU_Y_3;
			break;
		case 2:
			cursor_y = INV_HU_Y_4;
			break;
		case 3:
			cursor_y = INV_HU_Y_6;
			break;
		case 4:
			cursor_y = INV_HU_Y_7;
			break;
		case 5:
			cursor_y = INV_HU_Y_9;
			break;
		case 6:
			cursor_y = INV_HU_Y_11;
			break;
		case 7:
			cursor_y = INV_HU_Y_13;
			break;
		case 8:
			cursor_y = INV_HU_Y_14;
			break;
		case 9:
			cursor_y = INV_HU_Y_16;
			break;
		case 10:
			cursor_y = INV_HU_Y_19;
			break;

		default:
			return;
		}
	}

	if (optionsmenu_on)
	{
		switch (options_selection)
		{
		case 1:
			cursor_y = INV_HU_Y_3;
			break;
		case 2:
			cursor_y = INV_HU_Y_4;
			break;
		case 3:
			cursor_y = INV_HU_Y_5;
			break;
		case 4:
			cursor_y = INV_HU_Y_6;
			break;
		case 5:
			cursor_y = INV_HU_Y_8;
			break;
		case 6:
			cursor_y = INV_HU_Y_9;
			break;
		case 7:
			cursor_y = INV_HU_Y_10;
			break;
		case 8:
			cursor_y = INV_HU_Y_12;
			break;
		case 9:
			cursor_y = INV_HU_Y_13;
			break;
		case 10:
			cursor_y = INV_HU_Y_15;
			break;
		case 11:
			cursor_y = INV_HU_Y_16;
			break;
		case 12:
			cursor_y = INV_HU_Y_17;
			break;
		case 13:
			cursor_y = INV_HU_Y_18;
			break;
		case 14:
			cursor_y = INV_HU_Y_19;
			break;

		default:
			return;
		}
	}


	if (mainmenu_on)
	{  
	  switch (mainmenu_selection)
	  {  
		case 1:
			cursor_y = INV_HU_Y_3;  
			break;
		case 2:
			cursor_y = INV_HU_Y_4;
			break;
		case 3:
			cursor_y = INV_HU_Y_5;
			break;
		case 4:
			cursor_y = INV_HU_Y_6;
			break;
		case 5:
			cursor_y = INV_HU_Y_7;
			break;
		case 6:
			cursor_y = INV_HU_Y_8;
			break;
		case 7:
			cursor_y = INV_HU_Y_9;
			break;
		case 8:
			cursor_y = INV_HU_Y_10;
			break;

		default:
			return;
	  }
	}

	else if (botmenu_on)
	{  
	  switch (botmenu_selection)
	  {  
		case 1:
			cursor_y = INV_HU_Y_3;   
			break;
		case 2:
			cursor_y = INV_HU_Y_4;
			break;
		case 3:
			cursor_y = INV_HU_Y_5;
			break;
		case 4:
			cursor_y = INV_HU_Y_6;
			break;
		case 5:
			cursor_y = INV_HU_Y_8;
			break;
		case 6:
			cursor_y = INV_HU_Y_9;
			break;
		case 7:
			cursor_y = INV_HU_Y_10;
			break;
		case 8:
			cursor_y = INV_HU_Y_12;
			break;
		case 9:
			cursor_y = INV_HU_Y_14;
			break;
		case 10:
			cursor_y = INV_HU_Y_16;
			break;

		default:
			return;
	  }
	}

	else if (msgsmenu_on)
	{  
	  switch (msgsmenu_selection)
	  {  
		case 1:
			cursor_y = INV_HU_Y_3;   
			break;
		case 2:
			cursor_y = INV_HU_Y_4;
			break;
		case 3:
			cursor_y = INV_HU_Y_5;
			break;
		case 4:
			cursor_y = INV_HU_Y_6;
			break;
		case 5:
			cursor_y = INV_HU_Y_7;
			break;
		case 6:
			cursor_y = INV_HU_Y_8;
			break;
		case 7:
			cursor_y = INV_HU_Y_9;
			break;
		case 8:
			cursor_y = INV_HU_Y_10;
			break;
		case 9:
			cursor_y = INV_HU_Y_11;
			break;


		default:
			return;
	  }
	}

	else if (skillmenu_on)
	{  
	  switch (skill_selection)
	  {  
		case 1:
			cursor_y = INV_HU_Y_3;   
			break;
		case 2:
			cursor_y = INV_HU_Y_4;
			break;
		case 3:
			cursor_y = INV_HU_Y_5;
			break;
		case 4:
			cursor_y = INV_HU_Y_6;
			break;
		case 5:
			cursor_y = INV_HU_Y_7;
			break;
		case 6:
			cursor_y = INV_HU_Y_8;
			break;
		case 7:
			cursor_y = INV_HU_Y_10;
			break;

		default:
			return;
	  }
	}

	else if (musicmenu_on)
	{
		switch (musicmenu_selection)
		{
			case 1:
				cursor_y = INV_HU_Y_3;   
				break;
			case 2:
				cursor_y = INV_HU_Y_5;
				break;
			case 3:
				cursor_y = INV_HU_Y_9;
				break;
			case 4:
				cursor_y = INV_HU_Y_11;
				break;
			case 5:
				cursor_y = INV_HU_Y_13;
				break;
			case 6:
				cursor_y = INV_HU_Y_15;
				break;
			case 7:
				cursor_y = INV_HU_Y_17;
				break;
			case 8:
				cursor_y = INV_HU_Y_19;
				break;

			default:
				return;
		}
	}

	else if (vanillamusicmenu_on)
	{
		switch (vanillamusicmenu_selection)
		{
			case 1:
				cursor_y = INV_HU_Y_3;   
				break;
			case 2:
				cursor_y = INV_HU_Y_6;
				break;
		}

	}

	else if (blacklistmenu_on)
	{
		switch (blacklistmenu_selection)
		{
			case 1:
				cursor_y = INV_HU_Y_3;   
				break;
			case 2:
				cursor_y = INV_HU_Y_5;
				break;
			case 3:
				cursor_y = INV_HU_Y_7;
				break;
			case 4:
				cursor_y = INV_HU_Y_9;
				break;	
		}
	}

	else if (invmenu_on)
	{
		switch (invmenu_selection)
		{
			case 1:
				cursor_y = INV_HU_Y_6;  
				break;
			case 2:
				cursor_y = INV_HU_Y_7;
				break;
			case 3:
				cursor_y = INV_HU_Y_8;
				break;
			case 4:
				cursor_y = INV_HU_Y_9;
				break;
			case 5:
				cursor_y = INV_HU_Y_10;
				break;
			case 6:
				cursor_y = INV_HU_Y_11;
				break;

			default:
				cursor_y = INV_HU_Y_7;
		}
	}


	HUlib_initSText(&menu_cursor,
		cursor_x, cursor_y, HU_MSGHEIGHT,     
		hu_font,
		HU_FONTSTART, &menus_on);   
}


boolean WeHaveItem(invitem_t item);   
invitem_t OwnedItems[MAX_INV_ITEMS];  // probably no longer needed

static void CalcInvCursorPos(direction_t dir)   // consider having this return value for invmenu_selection
{
	/*
	// probably no longer needed

	int i;
	int numitems = 0;
	int test_index = 0;

	for (i=0; i<MAX_INV_ITEMS; i++)
	{
		if ( MAIN_PLAYER.extra_powers[i] )
		{
			numitems++;

			OwnedItems[ numitems ] = i;
		}
	}

	if (!numitems)
	{
		SHOW_MESSAGE "NO ITEMS OWNED.";
		return;
	}
	*/
	if (dir == FORWARD)
	{
		if (invmenu_selection < MAX_INV_ITEMS-1)
		{
			int i;
			
			for (i=invmenu_selection; i<MAX_INV_ITEMS; i++)
			{
				if (i==invmenu_selection)
					continue;

				if ( WeHaveItem( i ) )
				{
					invmenu_selection = i;
					break;
				}
			}
		}
	}

	if (dir == BACKWARD)
	{
		if (invmenu_selection > FIRST_MENU_ITEM)
		{
			int i;
			
			for (i=invmenu_selection; i>0; i--)
			{
				if (i == invmenu_selection)
					continue;

				if ( WeHaveItem( i ) )
				{
					invmenu_selection = i;
					break;
				}
			}
		}
	}
}


static void Bot_ChangeWeapon(int bot, direction_t dir)
{
	if (!bot_in_game[bot])
		return;

	if (deathmatch)
    {
        SHOW_MESSAGE "NOT AVAILABLE IN DEATHMATCH.";
        return;
    }

	if (dir == FORWARD)
	{
		Bots[bot].weaponmenu_selection++; 
	
		if (Bots[bot].weaponmenu_selection == BOTMENU_END)
			Bots[bot].weaponmenu_selection = 0;
	}
	else
	{
		Bots[bot].weaponmenu_selection--; 
	
		if (Bots[bot].weaponmenu_selection < 0)
			Bots[bot].weaponmenu_selection = BOTMENU_END-1;	
	}
}


void HUDMenuKeyInput()	
{
#if 0
	if (gamekeydown[key_l])
	{
		if (CheckKeyDelay())
			return;		

		if ( realnetgame || !Marshmallow_AlternateLighting || PlayerIsDead() || !Marshmallow_Flashlight)
			return;

		if (!flashlight_on)
		{
			flashlight_on = true;
			A_Light1(NULL, &MAIN_PLAYER, NULL);
			CrispyReplaceColor("FLASHLIGHT ON.", CR_GREEN, "ON");

			S_StartSound(0, sfx_tink);
		}
		else
		{
			flashlight_on = false;
			A_Light0(NULL, &MAIN_PLAYER, NULL);
			CrispyReplaceColor("FLASHLIGHT OFF.", CR_DARK, "OFF");

			S_StartSound(0, sfx_tink);
		}
	}
#endif

	if (gamekeydown[key_p])
	{
		if (CheckKeyDelay())
			return;		

		if (realnetgame || deathmatch)
		{
			return;
		}

		if (botcommandmenu_on)
			return;
		
		if (!pkereadout_on)  
		{
			S_StartSound(NULL, sfx_tink);
			HideAllMenus();
			pkereadout_on = true;
		}
		else 
		{
			HideAllMenus();
			S_StartSound(NULL, sfx_tink);
		}
	}

	if (gamekeydown[key_escape])
	{
		if (CheckKeyDelay())
			return;

		if (profilescreen_on)
		{
			profilescreen_on = false;

			if (!realnetgame)
				mainmenu_on = true;
		}
		else if (enemymenu_on)  // backtracking from a sub-sub menu
		{
			enemymenu_on = false;
			optionsmenu_on = true;
		}
		else if (weaponmenu_on)   // backtracking from a sub-sub menu
		{
			weaponmenu_on = false;
			optionsmenu_on = true;
		}
		else if (blacklistmenu_on)   // backtracking from a sub-sub menu
		{
			blacklistmenu_on = false;
			
			if (Marshmallow_DynamicMusic)
				musicmenu_on = true;	
			else
				vanillamusicmenu_on = true;
		}
		else if ( shortcutmenu_on )
        {
            HideAllMenus();
            menuactive = true;
        }
		else if ( optionsmenu_on )
        {
            if (mainmenu_breadcrumb)
            {
                HideAllMenus();
                shortcutmenu_on = true;
            }
            else
            {
                HideAllMenus();
                mainmenu_on = true;
            }
        }
        else if ( musicmenu_on )
        {
            if (mainmenu_breadcrumb)
            {
                HideAllMenus();
                shortcutmenu_on = true;
            }
            else
            {
                HideAllMenus();
                mainmenu_on = true;
            }
        }
        else if ( msgsmenu_on )
        {
            if (mainmenu_breadcrumb)
            {
                HideAllMenus();
                shortcutmenu_on = true;
            }
            else
            {
                HideAllMenus();
                mainmenu_on = true;
            }
        }
        else if ( vanillamusicmenu_on )
        {
            if (mainmenu_breadcrumb)
            {
                HideAllMenus();
                shortcutmenu_on = true;
            }
            else
            {
                HideAllMenus();
                mainmenu_on = true;
            }
        }
		else if ( sandboxmenu_on  // backtacking from a sub menu...
				 || skipmenu_on
				 || blacklistmenu_on
				 || optionsmenu_on
				 || botmenu_on )
		{
			HideAllMenus();
			mainmenu_on = true; 
		}
		else if (invmenu_on)
		{
			HideAllMenus();
		}
		else if (pkereadout_on)
		{
			HideAllMenus();
		}
		else if (mainmenu_on)   
		{
			HideAllMenus();
		}
	}

	if (gamekeydown[key_m] && !menuactive)  // our new main menu on the HUD
	{
		if (CheckKeyDelay())
			return;

		if (realnetgame)
		{
			if (profilescreen_on)
			{
				profilescreen_on = false;
				menus_on = false;			
			}
			else
			if ( LaunchProfileScreen() )
			{
				profilescreen_on = true;
				menus_on = true;
			}

			return;
		}

		if (!menus_on)  // opening the datapad
		{
			S_StartSound(MAIN_PLAYER.mo, sfx_tink);
			HideAllMenus();
			mainmenu_on = true;
		}
		else  // closing the datapad
		{
			HideAllMenus();
			S_StartSound(MAIN_PLAYER.mo, sfx_tink);
		}
	}


	if (gamekeydown[key_b] && !sandbox.design_mode && !realnetgame)  // bot menu shortcut
	{
		if (CheckKeyDelay())
			return;

		if (botmenu_on)  // if already up, close it
		{
			HideAllMenus();
			return;
		}

		HideAllMenus();
		//mainmenu_on = false;
		botmenu_on = true;

		if (!botmenu_selection
			|| (deathmatch && botmenu_selection > BOT3_WEAPON))
			botmenu_selection = FIRST_MENU_ITEM;
	}

    if (shortcutmenu_on)
    {
        if (MENUKEY_NEXT)
        {
            if (CheckKeyDelay())
                return;

            shortcutmenu_selection++;

            if (shortcutmenu_selection == MAX_SHORTCUT_OPTIONS)
                shortcutmenu_selection = FIRST_MENU_ITEM;
        }

        if (MENUKEY_PREVIOUS)
        {
            if (CheckKeyDelay())
                return;

            shortcutmenu_selection--;

            if (shortcutmenu_selection == 0)
                shortcutmenu_selection = MAX_SHORTCUT_OPTIONS - 1;
        }

        if (MENUKEY_SELECT)
        {
            if (CheckKeyDelay())
                return;

            switch (shortcutmenu_selection)
            {
                case GAMEPLAYMENU_SELECTED:
                    HideAllMenus();
                    optionsmenu_on = true;

                    if (!options_selection)
                    options_selection = FIRST_MENU_ITEM;

                    SetKeyDelay();

                    break;

                case MUSICMENU_SELECTED:
                    if (M_CheckParm("-nomusic"))
                    {
                        SHOW_MESSAGE "MUSIC IS DISABLED.";
                        break;
                    }

                    if (Marshmallow_DynamicMusic)
                    {
                        HideAllMenus();
                        musicmenu_on = true;

                        if (!musicmenu_selection)
                            musicmenu_selection = FIRST_MENU_ITEM;
                    }
                    else
                    {
                        HideAllMenus();
                        vanillamusicmenu_on = true;

                        if (!vanillamusicmenu_selection)
                            vanillamusicmenu_selection = FIRST_MENU_ITEM;
                    }

                    SetKeyDelay();

                    break;

                case MESSAGESMENU_SELECTED:
                    HideAllMenus();
                    msgsmenu_on = true;

                    if (!msgsmenu_selection)
                    msgsmenu_selection = FIRST_MENU_ITEM;

                    SetKeyDelay();

                    break;
            }
        }
    }

	if (msgsmenu_on)
	{
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			msgsmenu_selection++;

			if (msgsmenu_selection == MAX_MSGMENU_ITEMS)
				msgsmenu_selection = FIRST_MENU_ITEM;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			msgsmenu_selection--;

			if (msgsmenu_selection == 0)
				msgsmenu_selection = MAX_MSGMENU_ITEMS - 1;
		}

		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
			return;

			switch (msgsmenu_selection)
			{
			case DANGERINDEX_SELECTED:
				if (!hellscape_on)
				{
					hellscape_on = true;
					//mainmenu_on = false;
				}
				else
				{
					hellscape_on = false;
					//mainmenu_on = false;
				}

				SetKeyDelay();

				break;

			case PICKUPMSG_SELECTED:
				
				if (Marshmallow_PickupMessages)  // roll into InvertBoolean() and get rid of these console messages altogether
				{
					SHOW_MESSAGE "PICKUP MESSAGES DISABLED.";
					Marshmallow_PickupMessages = !Marshmallow_PickupMessages;
				}
				else
				{
					SHOW_MESSAGE "PICKUP MESSAGES ENABLED.";
					Marshmallow_PickupMessages = !Marshmallow_PickupMessages;
				}

				SetKeyDelay();  // TODO: only call this once after this switch block; apply to all of these switch blocks

				break;

			case MISSILEALARM_SELECTED:

				if (Marshmallow_MissileAlert)  // roll into InvertBoolean() and get rid of these console messages altogether
				{
					SHOW_MESSAGE "MISSILE-LOCK ALARM DISABLED.";
					Marshmallow_MissileAlert = !Marshmallow_MissileAlert;
				}
				else
				{
					SHOW_MESSAGE "MISSILE-LOCK ALARM ENABLED.";
					Marshmallow_MissileAlert = !Marshmallow_MissileAlert;
				}

				SetKeyDelay();  // TODO: only call this once after this switch block; apply to all of these switch blocks

				break;

			case TARGETHP_SELECTED:
#if 0
				if (Marshmallow_ShowTargetHP)  // roll into InvertBoolean() and get rid of these console messages altogether
				{
					SHOW_MESSAGE "HIDING TARGET HITPOINTS";
					Marshmallow_ShowTargetHP = !Marshmallow_ShowTargetHP;
				}
				else
				{
					SHOW_MESSAGE "SHOWING TARGET HITPOINTS";
					Marshmallow_ShowTargetHP = !Marshmallow_ShowTargetHP;
				}
#endif
				Marshmallow_ShowTargetHP++;
				if (Marshmallow_ShowTargetHP > 2)
					Marshmallow_ShowTargetHP = 0;

				SetKeyDelay();

				break;

			case DEATHMSG_SELECTED:
				
				if (Marshmallow_DeathMessages)
				{
					SHOW_MESSAGE "DEATH MESSAGES DISABLED.";
					Marshmallow_DeathMessages = !Marshmallow_DeathMessages;
				}
				else
				{
					SHOW_MESSAGE "DEATH MESSAGES ENABLED.";
					Marshmallow_DeathMessages = !Marshmallow_DeathMessages;
				}

				SetKeyDelay();

				break;

	/*		case DAMAGEMSG_SELECTED:
				
				if (Marshmallow_DamageMessages)
				{
					SHOW_MESSAGE "DAMAGE MESSAGES DISABLED.";
					Marshmallow_DamageMessages = !Marshmallow_DamageMessages;
				}
				else
				{
					SHOW_MESSAGE "DAMAGE MESSAGES ENABLED.";
					Marshmallow_DamageMessages = !Marshmallow_DamageMessages;
				}

				SetKeyDelay();

				break;*/

			case EXTENDEDMSG_SELECTED:
				
				if (Marshmallow_ExtendedMessages)
				{
					//SHOW_MESSAGE "EXTENDED MESSAGES DISABLED.";
					Marshmallow_ExtendedMessages = !Marshmallow_ExtendedMessages;
				}
				else
				{
					//SHOW_MESSAGE "EXTENDED MESSAGES ENABLED.";
					Marshmallow_ExtendedMessages = !Marshmallow_ExtendedMessages;
				}

				SetKeyDelay();

				break;

			case EXTRALINE_SELECTED:
				
				if (Marshmallow_ExtraTextLines)
				{
					SHOW_MESSAGE "EXTRA TEXT LINE DISABLED.";
					Marshmallow_ExtraTextLines = !Marshmallow_ExtraTextLines;
				}
				else
				{
					SHOW_MESSAGE "EXTRA TEXT LINE ENABLED.";
					Marshmallow_ExtraTextLines = !Marshmallow_ExtraTextLines;
				}

				SetKeyDelay();

				break;

			case BOSSALERTS_SELECTED:
				
				if (Marshmallow_BossAlert)
				{
					SHOW_MESSAGE "BOSS ALERTS DISABLED.";
					Marshmallow_BossAlert = !Marshmallow_BossAlert;
				}
				else
				{
					SHOW_MESSAGE "BOSS ALERTS ENABLED.";
					Marshmallow_BossAlert = !Marshmallow_BossAlert;
				}

				SetKeyDelay();

				break;

			case INFIGHT_SELECTED:
				
				if (Marshmallow_InfightAlert)
				{
					SHOW_MESSAGE "INFIGHT ALERTS DISABLED.";
					Marshmallow_InfightAlert = !Marshmallow_InfightAlert;
				}
				else
				{
					SHOW_MESSAGE "INFIGHT ALERTS ENABLED.";
					Marshmallow_InfightAlert = !Marshmallow_InfightAlert;
				}

				SetKeyDelay();

				break;

			}
		}
	}

	if (botmenu_on)
	{
		if (MENUKEY_NEXT)
		{
			int offset;
			
			if (CheckKeyDelay())
			return;

			if (deathmatch)
				offset = MAX_BOTMENU_ITEMS - 3;
			else
				offset = MAX_BOTMENU_ITEMS;

			botmenu_selection++;

			if (botmenu_selection == offset)
				botmenu_selection = FIRST_MENU_ITEM;
		}

		if (MENUKEY_PREVIOUS)
		{
			int offset;

			if (CheckKeyDelay())
			return;

			if (deathmatch)
				offset = MAX_BOTMENU_ITEMS - 4;
			else
				offset = MAX_BOTMENU_ITEMS - 1;

			botmenu_selection--;

			if (botmenu_selection == 0)
				botmenu_selection = offset;
		}

		if (MENUKEY_SELECT)
		{
			int i; 

			if (CheckKeyDelay())
			return;

			switch (botmenu_selection)
			{
			case SPAWN_BOT_SELECTED:

				//botmenu_on = false;

				AddBot();   

				//Marshmallow_ShowBotReadout = true;

				SetKeyDelay();
				break;

			case SPAWN_ALL_SELECTED:  
				
				for (i=1; i<4; i++)  // magic numbers; we just need cocoa_bot.h so we can use BOT_1 and MAX_BOTS macros
					AddBot();

				//botmenu_on = false;

				break;

			//case BOT_STATES_SELECTED:

			//	//botmenu_on = false;
			//	if (!BotsInGame)
			//	{
			//		SHOW_MESSAGE "NO BOTS CURRENTLY IN GAME!";
			//		break;
			//	}

			//	Marshmallow_ShowBotReadout = !Marshmallow_ShowBotReadout;
			//	
			//	SetKeyDelay();

			//	break;

			case REMOVE_BOTS_SELECTED:

				//botmenu_on = false;

				Bot_RemoveBots();

				SetKeyDelay();
				break;

			case KILL_BOTS_SELECTED:

				if (bot_in_game[BOT_1])
					P_KillMobj(NULL, Bots[BOT_1].player->mo);
				if (bot_in_game[BOT_2])
					P_KillMobj(NULL, Bots[BOT_2].player->mo);
				if (bot_in_game[BOT_3])
					P_KillMobj(NULL, Bots[BOT_3].player->mo);

				SetKeyDelay();
				break;

			case BOT1_WEAPON:

				Bot_ChangeWeapon(BOT_1, FORWARD);
				Bot_SetWeapon(BOT_1);

				SetKeyDelay();
				break;

			case BOT2_WEAPON:

				Bot_ChangeWeapon(BOT_2, FORWARD);
				Bot_SetWeapon(BOT_2);

				SetKeyDelay();
				break;

			case BOT3_WEAPON:

				Bot_ChangeWeapon(BOT_3, FORWARD);
				Bot_SetWeapon(BOT_3);

				SetKeyDelay();
				break;		

			case BOTFF_SELECTED:

				if (deathmatch)
				{
					SHOW_MESSAGE "SETTING IS FOR COOP ONLY.";
					break;
				}
				
				ToggleFriendlyFire();
				SetKeyDelay();
				break;

			case BOTSPEED_SELECTED:

				default_bot_speed++;

				if (default_bot_speed > BOTS_RUN)
					default_bot_speed = 0;

				break;

			case BOTSTATES_SELECTED:

				ShowBotStates = !ShowBotStates;
				break;

			//case BOTCOLLISION_SELECTED:

			//	Marshmallow_PlayerCollision = !Marshmallow_PlayerCollision;

			//	break;
			}
		}

		if (MENUKEY_RIGHTARROW)
		{
			if (CheckKeyDelay())
				return;

			switch (botmenu_selection)
			{
			case BOT1_WEAPON:

				Bot_ChangeWeapon(BOT_1, FORWARD);
				Bot_SetWeapon(BOT_1);

				SetKeyDelay();
				break;

			case BOT2_WEAPON:

				Bot_ChangeWeapon(BOT_2, FORWARD);
				Bot_SetWeapon(BOT_2);

				SetKeyDelay();
				break;

			case BOT3_WEAPON:

				Bot_ChangeWeapon(BOT_3, FORWARD);
				Bot_SetWeapon(BOT_3);

				SetKeyDelay();
				break;		

			case BOTFF_SELECTED:

				if (deathmatch)
				{
					SHOW_MESSAGE "SETTING IS FOR COOP ONLY.";
					break;
				}
				
				ToggleFriendlyFire();
				SetKeyDelay();
				break;

			case BOTSPEED_SELECTED:

				default_bot_speed++;

				if (default_bot_speed > BOTS_RUN)
					default_bot_speed = 0;

				break;

			case BOTSTATES_SELECTED:

				ShowBotStates = !ShowBotStates;
				break;
			}
		}

		if (MENUKEY_LEFTARROW)
		{
			if (CheckKeyDelay())
				return;

			switch (botmenu_selection)
			{
			case BOT1_WEAPON:

				Bot_ChangeWeapon(BOT_1, BACKWARD);
				Bot_SetWeapon(BOT_1);

				SetKeyDelay();
				break;

			case BOT2_WEAPON:

				Bot_ChangeWeapon(BOT_2, BACKWARD);
				Bot_SetWeapon(BOT_2);

				SetKeyDelay();
				break;

			case BOT3_WEAPON:

				Bot_ChangeWeapon(BOT_3, BACKWARD);
				Bot_SetWeapon(BOT_3);

				SetKeyDelay();
				break;

			case BOTFF_SELECTED:

				if (deathmatch)
				{
					SHOW_MESSAGE "SETTING IS FOR COOP ONLY.";
					break;
				}
				
				ToggleFriendlyFire();
				SetKeyDelay();
				break;

			case BOTSPEED_SELECTED:

				default_bot_speed--;

				if (default_bot_speed < 0)
					default_bot_speed = BOTS_RUN;

				break;

			case BOTSTATES_SELECTED:

				ShowBotStates = !ShowBotStates;
				break;
			}
		}
	}


	if (mainmenu_on)
	{
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			S_StartSound(MAIN_PLAYER.mo, sfx_stnmov);

			mainmenu_selection++;

			if (mainmenu_selection == MAX_MAINMENU_ITEMS)
				mainmenu_selection = FIRST_MENU_ITEM;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			S_StartSound(MAIN_PLAYER.mo, sfx_stnmov);
				
			mainmenu_selection--;

			if (mainmenu_selection == 0)
				mainmenu_selection = MAX_MAINMENU_ITEMS - 1;
		}

		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
				return;

			S_StartSound(MAIN_PLAYER.mo, sfx_tink);  // sfx_swtchn is the default menu sound

			switch (mainmenu_selection)
			{
			case SANDBOX_SELECTED:

				if (realnetgame)
				{
					SHOW_MESSAGE "NOT AVAILABLE IN NETGAME.";
					break;
				}

				if (!Marshmallow_Sandbox)
				{
					SHOW_MESSAGE "NO SANDBOX GAME IN PROGRESS.";
					break;				
				}

				sandboxmenu_on = true;
				mainmenu_on = false;
				sandboxmenu_selection = FIRST_MENU_ITEM;
				SetKeyDelay();

				// old:
				// confirm are you sure?  y/n
				// restart map with no monsters in sandbox mode

				break;

			case BOTMENU_SELECTED:

				if (realnetgame)
				{
					SHOW_MESSAGE "NOT AVAILABLE IN NETGAME.";
					break;
				}

				mainmenu_on = false;
				if (!botmenu_selection
					|| (deathmatch && botmenu_selection > BOT3_WEAPON))
					botmenu_selection = FIRST_MENU_ITEM;
				botmenu_on = true;
				SetKeyDelay();
				break;

			case PROFILE_SELECTED:
				if ( LaunchProfileScreen() )
				{
					profilescreen_on = true;
					menus_on = true;
					mainmenu_on = false;
					skill_selection = newskill;
				}
				SetKeyDelay();
				break;

			//case CHANGESKILL_SELECTED:
			//	mainmenu_on = false;
			//	skillmenu_on = true;
			//	skill_selection = SetCursorToSkill() + 1;
			//	CheckCurrentSkill();
			//	SetKeyDelay();
			//	break;

			//case INVMENU_SELECTED:
			//	LaunchInventoryMenu();

			//	mainmenu_on = false;
			//	break;

			case SUICIDE_SELECTED:
				if (!offer_suicide)
				{
					offer_suicide = true;
					offertimeout_suicide = DEFAULT_OFFER_TIMEOUT;
					SHOW_MESSAGE DEH_String(CONFIRMSUICIDE);
				}
				else if (offertimeout_suicide)
				{
					if (!netgame)
						PlayerKillsHimself(MAIN_PLAYER.mo);
					else
					//	MarshmallowEvents.suicide = 1;  // experiment 2018
						Marshmallow_SendMultiplayerEvent(MARSHMALLOW_SOMEONE_KILLED_THEMSELF);
					mainmenu_on = false;
					offer_suicide = false;
					offertimeout_suicide = 0;
				}
				break;

			case SKIPMAP_SELECTED:

				if (realnetgame)
				{
					SHOW_MESSAGE "NOT AVAILABLE IN NETGAME.";
					break;
				}

				if (!skipmenu_selection)
					skipmenu_selection = FIRST_MENU_ITEM; 

				SetSkills();
				
				mainmenu_on = false;
				skipmenu_on = true;
				SetKeyDelay();
				break;

			case MUSIC_SELECTED:
				
				if (M_CheckParm("-nomusic"))
				{
					SHOW_MESSAGE "MUSIC IS DISABLED.";
					break;
				}

				if (realnetgame)
				{
					SHOW_MESSAGE "NOT AVAILABLE IN NETGAME.";
					break;
				}

				if (Marshmallow_DynamicMusic)
				{
					mainmenu_on = false;
					musicmenu_on = true;
					if (!musicmenu_selection)
						musicmenu_selection = FIRST_MENU_ITEM; 
				}
				else
				{
					mainmenu_on = false;
					vanillamusicmenu_on = true;
					if (!vanillamusicmenu_selection)
						vanillamusicmenu_selection = FIRST_MENU_ITEM; 
				}
				
				SetKeyDelay();
				break;

			case MESSAGES_SELECTED:

				if (realnetgame)
				{
					SHOW_MESSAGE "NOT AVAILABLE IN NETGAME.";
					break;
				}

				mainmenu_on = false;
				msgsmenu_on = true;
				if (!msgsmenu_selection)
					msgsmenu_selection = FIRST_MENU_ITEM; 
				SetKeyDelay();
				break;

			case OPTIONS_SELECTED:

				if (realnetgame)
				{
					SHOW_MESSAGE "NOT AVAILABLE IN NETGAME.";
					break;
				}

				mainmenu_on = false;
				optionsmenu_on = true;
				if (!options_selection)
					options_selection = FIRST_MENU_ITEM; 
				SetKeyDelay();
				break;
			}
		}
	}

	if (enemymenu_on)
	{
		int offset;

		if (gamemode == commercial || Marshmallow_WadStealing)
			offset = 1;
		else
			offset = 7;

		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
			return;

			switch (enemymenu_selection)
			{
			case FASTMONSTERS_SELECTED:
				ToggleFastMonsters();
				break;
			case LOSTSOULHP_SELECTED:
				Marshmallow_NerfHP_LostSoul = !Marshmallow_NerfHP_LostSoul;
				NerfHP_LostSoul(Marshmallow_NerfHP_LostSoul);
				break;
			case CACOHP_SELECTED:
				Marshmallow_NerfHP_Cacodemon = !Marshmallow_NerfHP_Cacodemon;
				NerfHP_Cacodemon(Marshmallow_NerfHP_Cacodemon);
				break;
			case DEMONHP_SELECTED:
				Marshmallow_NerfHP_Demons = !Marshmallow_NerfHP_Demons;
				NerfHP_Demons(Marshmallow_NerfHP_Demons);
				break;		
			case CACOSOUND_SELECTED:
				Marshmallow_CacoMeleeSound = !Marshmallow_CacoMeleeSound;
				// no switcher function yet
				break;
			case SKELSPEED_SELECTED:
				Marshmallow_NerfSpeed_Undead = !Marshmallow_NerfSpeed_Undead;
				NerfSpeed_Undead(Marshmallow_NerfSpeed_Undead);
				break;
			case SKELMISSILE_SELECTED: 
				Marshmallow_NerfSkelMissile = !Marshmallow_NerfSkelMissile;
				NerfSkelMissile(Marshmallow_NerfSkelMissile);
				break;
			case PAINDEATH_SELECTED:
				Marshmallow_AltPainDeath = !Marshmallow_AltPainDeath;
				ChangePainDeath(Marshmallow_AltPainDeath);
				break;
			case VILEPAIN_SELECTED:
				Marshmallow_NerfPC_Archvile = !Marshmallow_NerfPC_Archvile;
				NerfPC_Archvile(Marshmallow_NerfPC_Archvile);
				break;
			case CGUYSND_SELECTED:
				Marshmallow_FixChainguySound = !Marshmallow_FixChainguySound;
				FixChainguySound(Marshmallow_FixChainguySound);
				break;
			case VILEZ_SELECTED:
				Marshmallow_VileZScopeLimit = !Marshmallow_VileZScopeLimit;
				// no switcher function yet
				break;
			}
		}

		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			if (enemymenu_selection == MAX_MONSTERTWEAKS - offset)
				enemymenu_selection = FIRST_MENU_ITEM;
			else
				enemymenu_selection++;	
		}
		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			if (enemymenu_selection == 1)
				enemymenu_selection = MAX_MONSTERTWEAKS - offset;
			else
				enemymenu_selection--;
		}
	}
	
	
	if (weaponmenu_on)
	{
		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
			return;

			switch (weaponmenu_selection)
			{
			case TRUERANDOM_SELECTED:
				Marshmallow_TrueRandomDamage = !Marshmallow_TrueRandomDamage;
				break;
			case BERSAUTO_SELECTED:
				Marshmallow_BerserkAutoSwitch = !Marshmallow_BerserkAutoSwitch;
				break;
			case BERSRED_SELECTED:
				ChangeBerserkRedLength();
				break;
			case CSAWDMG_SELECTED:
				Marshmallow_BoostedChainsaw = !Marshmallow_BoostedChainsaw;  
				BoostChainsaw(Marshmallow_BoostedChainsaw);
				break;
			case FISTDMG_SELECTED:
				Marshmallow_BoostedFist = !Marshmallow_BoostedFist;
				BoostFist(Marshmallow_BoostedFist);
				break;
			case BULDMG_SELECTED:
				Marshmallow_BoostedBullets = !Marshmallow_BoostedBullets;	
				BoostBullets(Marshmallow_BoostedBullets);
				break;		
			case INFAMMO_SELECTED:
				Marshmallow_InfinitePistol = !Marshmallow_InfinitePistol;	
				InfinitePistol(Marshmallow_InfinitePistol);
				break;
			case SGUNDMG_SELECTED:
				Marshmallow_BalanceShotguns = !Marshmallow_BalanceShotguns;	
				BalanceShotguns(Marshmallow_BalanceShotguns);
				break;
			case PLSLOW_SELECTED:
				Marshmallow_PlasmaSlowdown = !Marshmallow_PlasmaSlowdown;
				// no switcher funciton needed for this one
				break;
			case BFGWAVE_SELECTED:
				Marshmallow_BFGBlastWave = !Marshmallow_BFGBlastWave;
				// no switcher funciton needed for this one
				break;
			}
		}
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			if (weaponmenu_selection == MAX_WEAPONTWEAKS - 1)
				weaponmenu_selection = FIRST_MENU_ITEM;
			else
				weaponmenu_selection++;
		
		}
		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			if (weaponmenu_selection == 1)
				weaponmenu_selection = MAX_WEAPONTWEAKS - 1;
			else
				weaponmenu_selection--;
		}	
	}

	if (sandboxmenu_on)
	{
		if (gamekeydown[key_left])  // NEW: left arrow key for map selection
		{
			if (CheckKeyDelay())
			return;

			if (sandboxmenu_selection == WHICHMAP_SELECTED)
			{
				ChooseLevel_Prev();
			}
		}

		if (gamekeydown[key_right])  // NEW: left arrow key for map selection
		{
			if (CheckKeyDelay())
			return;

			if (sandboxmenu_selection == WHICHMAP_SELECTED)
			{
				ChooseLevel_Next();
			}
		}

		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
			return;

			switch (sandboxmenu_selection)
			{
			//case WHICHMAP_SELECTED:
				//ChooseLevel_Next();
				//break;
			
			case SANDBOXGO_SELECTED:
			//	if (skip_to_level)
			//	{   
					InitSandbox();
					SkipToLevel();
					HideAllMenus();
			//	}
			//	else
			//		SHOW_MESSAGE DEH_String(PICKMAP);
				break;

			case RESET_SELECTED:
				if (sandbox.count)
				{
					ResetSandbox();
					HideAllMenus();
				}
				else
					SHOW_MESSAGE DEH_String(NOMONSTERS);
				break;

			//case CANCEL_SELECTED:

			//	if (!Marshmallow_Sandbox)
			//		break;

			//	CancelSandbox();
			//	// TODO: clear-out sandbox struct here?
			//	SHOW_MESSAGE DEH_String(CHANGESBOX);
			//	break;
			}	
		}
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			//if (!skip_to_level)  // temporary - instead, hide "GO"
			//	return;
		
			if (sandboxmenu_selection == MAX_SANDBOX_OPTIONS - 1)
				sandboxmenu_selection = FIRST_MENU_ITEM;
			else
				sandboxmenu_selection++;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			//if (!skip_to_level)  // temporary - instead, hide "GO"
			//	return;

			if (sandboxmenu_selection == 1)
				sandboxmenu_selection = MAX_SANDBOX_OPTIONS - 1;
			else
				sandboxmenu_selection--;
		}
	}


	if (skipmenu_on)
	{
		//int offset;

		//if (!skip_to_level)
		//	offset = 2;
		//else
		//	offset = 1;

		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			skipmenu_selection++;

			if (skipmenu_selection > MAX_SKIPLEVEL_OPTIONS - 1)  
				skipmenu_selection = FIRST_MENU_ITEM;
			//else
//				skipmenu_selection++;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			skipmenu_selection--;

			if (skipmenu_selection < FIRST_MENU_ITEM)
				skipmenu_selection = MAX_SKIPLEVEL_OPTIONS - 1;
			//else
				//skipmenu_selection--;
		}

		if (MENUKEY_LEFTARROW)
		{
			if (CheckKeyDelay())
			return;

			switch (skipmenu_selection)
			{
			case SKIPTOMAP_SELECTED:
				ChooseLevel_Prev();
				break;

			case MAPSKILL_SELECTED:
				Skill_Prev();
				break;

			case UPGRADE_SELECTED:
				UpgradeChance_Down();
				break;

			case SCALEHP_SELECTED:
				HPScale_Down();
				break;

			case MAPSKIPWEAPONS_SELECTED:
				Marshmallow_GradedWeapons = !Marshmallow_GradedWeapons;
				break;

			case ITEMSPAWNS_SELECTED:
				Marshmallow_RandomItems--;

				if (Marshmallow_RandomItems < 0)
					Marshmallow_RandomItems = 2;
				break;
			}		
		}

		if (MENUKEY_RIGHTARROW)
		{
			if (CheckKeyDelay())
			return;

			switch (skipmenu_selection)
			{
			case SKIPTOMAP_SELECTED:
				ChooseLevel_Next();
				break;

			case MAPSKILL_SELECTED:
				Skill_Next();
				break;

			case UPGRADE_SELECTED:
				UpgradeChance_Up();
				break;

			case SCALEHP_SELECTED:
				HPScale_Up();
				break;

			case MAPSKIPWEAPONS_SELECTED:
				Marshmallow_GradedWeapons = !Marshmallow_GradedWeapons;
				break;

			case ITEMSPAWNS_SELECTED:
				Marshmallow_RandomItems++; 

				if (Marshmallow_RandomItems > 2)
					Marshmallow_RandomItems = 0;
				break;
			}
		}

		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
			return;

			switch (skipmenu_selection)
			{
			case SKIPTONEXT_SELECTED:

				HideAllMenus();  

				DisableStats();  // cheater...

				if (!netgame)
					G_ExitLevel();
				else
					Marshmallow_SendMultiplayerEvent(MARSHMALLOW_SOMEONE_ABORTED_MAP);

				MAIN_PLAYER.message = DEH_String(ABORTMAP);

				break;

			case RESTARTMAP_SELECTED:

				//if (netgame)   // at least until we add a CmdEvent for it to work in netgame UPDATE: this menu will be disabled in netgame anyway
				//{
				//	MAIN_PLAYER.message = "FEATURE CURRENTLY DISALBED IN NETGAME";
				//	return;
				//}

				mainmenu_on = false;

				RestartMap();

				break;

			case RANDOMMAP_SELECTED:

				HideAllMenus();  
			
				SkipToRandomLevel();

				//mainmenu_on = false;
				gaveweapons = false;

				break;

			//case SKIPTOMAP_SELECTED:

			//	ChooseLevel();

			//	break;

			case MAPSKIPWEAPONS_SELECTED:

				Marshmallow_GradedWeapons = !Marshmallow_GradedWeapons;
				break;

            case SKIPTOMAP_SELECTED:
                ChooseLevel_Next();
                break;

            case MAPSKILL_SELECTED:
                Skill_Next();
                break;

            case UPGRADE_SELECTED:
                UpgradeChance_Up();
                break;

            case SCALEHP_SELECTED:
                HPScale_Up();
                break;

			case ITEMSPAWNS_SELECTED:

				//Marshmallow_RandomItems = !Marshmallow_RandomItems;

				Marshmallow_RandomItems++;

				if (Marshmallow_RandomItems > 2)
					Marshmallow_RandomItems = 0;

				break;

			case GOSKIP_SELECTED:

				DisableStats();  // cheater...

				HideAllMenus();  // NEW

				SkipToLevel();
				gaveweapons = false;
				organic_levelchange = false;  // NEW

				break;
			}
		}
	}


	if (vanillamusicmenu_on)
	{
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			if (vanillamusicmenu_selection == MAX_VANILLAMUSIC_ITEMS - 1)  
				vanillamusicmenu_selection = FIRST_MENU_ITEM;
			else
				vanillamusicmenu_selection++;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			if (vanillamusicmenu_selection == 1)
				vanillamusicmenu_selection = MAX_VANILLAMUSIC_ITEMS - 1;
			else
				vanillamusicmenu_selection--;
		}

		if (MENUKEY_SELECT || MENUKEY_LEFTARROW || MENUKEY_RIGHTARROW)
		{
			if (CheckKeyDelay())
			return;

			switch (vanillamusicmenu_selection)
			{
			case MODE_VANILLA_SELECTED:

				//if ( Marshmallow_PlayingSigil )
				//{
					//SHOW_MESSAGE "SIGIL SONGS ONLY AVAILABLE IN VANILLA MODE.";
					//break;
				//}

				S_StartSound(NULL, sfx_hoof);
				ChangeMusicMode();
				vanillamusicmenu_on = false;
				musicmenu_on = true;
				if (!musicmenu_selection)
					musicmenu_selection = FIRST_MENU_ITEM;

				break;
			
			case BLACKLIST_VANILLA_SELECTED:

				vanillamusicmenu_on = false;
				blacklistmenu_on = true;

				if (!blacklistmenu_selection)
					blacklistmenu_selection = FIRST_MENU_ITEM;

				//SetKeyDelay();
				break;
			}
		}
	}


	if (musicmenu_on)
	{
		int offset;

		if (!Marshmallow_WadStealing)
			offset = 2;
		else
			offset = 1;

		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			if (!Marshmallow_DynamicMusic)
				return;

			//S_StartSound(MAIN_PLAYER.mo, sfx_hoof);
			if (musicmenu_selection == MAX_MUSICMENU_ITEMS - offset)   // account for "GAME" line not always being there
				musicmenu_selection = FIRST_MENU_ITEM;
			else
				musicmenu_selection++;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			if (!Marshmallow_DynamicMusic)
				return;

			//S_StartSound(MAIN_PLAYER.mo, sfx_hoof);
			if (musicmenu_selection == 1)
				musicmenu_selection = MAX_MUSICMENU_ITEMS - offset;
			else
				musicmenu_selection--;
		}

		if (MENUKEY_SELECT || MENUKEY_LEFTARROW || MENUKEY_RIGHTARROW)
		{
			if (CheckKeyDelay())
			return;

			switch (musicmenu_selection)
			{
			case MODE_SELECTED:
				S_StartSound(NULL, sfx_hoof);
				ChangeMusicMode();
				vanillamusicmenu_on = true;
				vanillamusicmenu_selection = FIRST_MENU_ITEM;
				musicmenu_on = false;
				break;

			case SKIP_SELECTED:
			    if (MENUKEY_LEFTARROW || MENUKEY_RIGHTARROW)
			        break;

				S_StartSound(NULL, sfx_tink);

				if (Marshmallow_DynamicMusic)
					DJ_NextTrack();

				break;

			//case SHUFFLE_SELECTED:
			//	S_StartSound(NULL, sfx_hoof);

			//	Doom_DJ.shuffle = !Doom_DJ.shuffle;

			//	//DJ_StartFadeout();
			//	//I_StopSong();
			//	//if (Marshmallow_DynamicMusic)
			//	//	DJ_PrevTrack();
			//	break;

			case DJMSGS_SELECTED:
				Marshmallow_DJMessages = !Marshmallow_DJMessages;
				break;

			case LENGTH_SELECTED:
				ChangeSongLength();
				break;

			case FASTCHANGES_SELECTED:
				Marshmallow_FastMusicChanges = !Marshmallow_FastMusicChanges;
				break;

			case PLAYLIST_SELECTED:
                if (MENUKEY_LEFTARROW || MENUKEY_RIGHTARROW)
                    break;

				S_StartSound(NULL, sfx_tink);
				// if (Marshmallow_DynamicMusic)
				ForcePlaylist();
				break;

			case GAME_SELECTED:
				S_StartSound(NULL, sfx_hoof);
				ChangeGameMusic();
				break;

			case MM_BLACKLIST_SELECTED:

                if (MENUKEY_LEFTARROW)
                    break;

				musicmenu_on = false;
				blacklistmenu_on = true;

				if (!blacklistmenu_selection)
					blacklistmenu_selection = FIRST_MENU_ITEM;

				//SetKeyDelay();
				break;
			}
		}
	}


	if (blacklistmenu_on)
	{
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			if (blacklistmenu_selection == MAX_BLACKLIST_ITEMS - 1)  
				blacklistmenu_selection = FIRST_MENU_ITEM;
			else
				blacklistmenu_selection++;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			if (blacklistmenu_selection == 1)
				blacklistmenu_selection = MAX_BLACKLIST_ITEMS - 1;
			else
				blacklistmenu_selection--;
		}

		if (MENUKEY_SELECT)
		{
			if (CheckKeyDelay())
			return;

			switch (blacklistmenu_selection)
			{
			case RUNNIN_SELECTED:
				ToggleSong_Runnin();
				break;

			case E1M1_SELECTED:
				ToggleSong_E1M1();
				break;

			case D1INTER_SELECTED:
				ToggleSong_DoomInter();
				break;

			case D2INTER_SELECTED:
				ToggleSong_Doom2Inter();
				break;
			}
		}
	}


	if (optionsmenu_on) 
	{
		int offset;
		
		if (!netgame)
			offset = 3;
		else
			offset = 1;

		
		if (MENUKEY_NEXT)
		{
			if (CheckKeyDelay())
			return;

			//S_StartSound(MAIN_PLAYER.mo, sfx_hoof);
			if (options_selection == MAX_GAMEPLAY_OPTIONS - offset)
				options_selection = FIRST_MENU_ITEM;
			else
				options_selection++;
		}

		if (MENUKEY_PREVIOUS)
		{
			if (CheckKeyDelay())
			return;

			//S_StartSound(MAIN_PLAYER.mo, sfx_hoof);
			if (options_selection == 1)
				options_selection = MAX_GAMEPLAY_OPTIONS - offset;
			else
				options_selection--;
		}

		if (MENUKEY_LEFTARROW)
		{
            if (CheckKeyDelay())
                return;

            switch (options_selection)
            {
                case TREASURE_SELECTED:
                    if (Marshmallow_Sandbox)
                    {
                        SHOW_MESSAGE "TREASURE NOT AVAILABLE IN SANDBOX!";
                        break;
                    }

                    Marshmallow_TreasureMode--;
                    if (Marshmallow_TreasureMode < 0)
                        Marshmallow_TreasureMode = 2;

                    SHOW_MESSAGE DEH_String(CHANGEFFECT);

                    break;

                case LUDICROUS_SELECTED:
                    Marshmallow_GibMode--;
                    if (Marshmallow_GibMode < 0)
                        Marshmallow_GibMode = 3;

                    if (Marshmallow_GibMode == BRUTAL_GIBS)
                        S_StartSound(NULL, sfx_slop);

                    break;

                case PHYSICS_SELECTED:

                    physics_mode--;

                    if (physics_mode < 0)
                        physics_mode = NUMPHYSICSMODES - 1;

                    break;

                case LIGHTING_SELECTED:

                    Marshmallow_AlternateLighting--;

                    if (Marshmallow_AlternateLighting < 0)
                        Marshmallow_AlternateLighting = NUMLIGHTLEVELS - 1;

                    SHOW_MESSAGE DEH_String(CHANGEFFECT);

                    break;

                case KEEPITEMS_SELECTED:
                    Marshmallow_SaveItems = !Marshmallow_SaveItems;
                    break;

                case CONSERVE_SELECTED:
                    Marshmallow_ConservePowerups = !Marshmallow_ConservePowerups;
                    break;

                case AUTOUSE_SELECTED:
                    Marshmallow_AutoUse = !Marshmallow_AutoUse;
                    break;

                case SELFDMG_SELECTED:
                    Marshmallow_SelfDamage = !Marshmallow_SelfDamage;
                    break;

                case KEEPWEAPONS_SELECTED:
                    Marshmallow_KeepWeapons = !Marshmallow_KeepWeapons;
                    break;

                case KEEPKEYS_SELECTED:
                    Marshmallow_KeepKeys = !Marshmallow_KeepKeys;
                    break;

                case WEAPONSSTAY_SELECTED:
                    Marshmallow_WeaponsStay = !Marshmallow_WeaponsStay;
                    break;

                case GOODIES_SELECTED:
                    ToggleGoodiesMode();

                    break;
            }
        }

		if (MENUKEY_SELECT || MENUKEY_RIGHTARROW)
		{
			if (CheckKeyDelay())
			return;

			switch (options_selection)
			{
			case KEEPITEMS_SELECTED:
				Marshmallow_SaveItems = !Marshmallow_SaveItems;
				break;

			case CONSERVE_SELECTED:
				Marshmallow_ConservePowerups = !Marshmallow_ConservePowerups;
				break;

			case AUTOUSE_SELECTED:
				Marshmallow_AutoUse = !Marshmallow_AutoUse;
				break;

			case SELFDMG_SELECTED:
				Marshmallow_SelfDamage = !Marshmallow_SelfDamage;
				break;

			case KEEPWEAPONS_SELECTED:
				Marshmallow_KeepWeapons = !Marshmallow_KeepWeapons;
				break;

			case KEEPKEYS_SELECTED:
				Marshmallow_KeepKeys = !Marshmallow_KeepKeys;
				break;

			case WEAPONSSTAY_SELECTED:
				Marshmallow_WeaponsStay = !Marshmallow_WeaponsStay;
				break;

			case WEAPONSMENU_SELECTED:
				optionsmenu_on = false;
				weaponmenu_on = true;

				if (!weaponmenu_selection)
				weaponmenu_selection = FIRST_MENU_ITEM;
				//SetKeyDelay();
				break;

			case MONSTERSMENU_SELECTED:
				optionsmenu_on = false;
				enemymenu_on = true;

				if (!enemymenu_selection)
				enemymenu_selection = FIRST_MENU_ITEM;
				//SetKeyDelay();
				break;

			case TREASURE_SELECTED:
				if (Marshmallow_Sandbox)
				{
					SHOW_MESSAGE "TREASURE NOT AVAILABLE IN SANDBOX!";
					break;
				}

				Marshmallow_TreasureMode++;
				if (Marshmallow_TreasureMode == 3)
					Marshmallow_TreasureMode = 0;

				SHOW_MESSAGE DEH_String(CHANGEFFECT);

				break;

			case LUDICROUS_SELECTED:
				Marshmallow_GibMode++;
				if (Marshmallow_GibMode > 3)
					Marshmallow_GibMode = 0;
				
				if (Marshmallow_GibMode == BRUTAL_GIBS)
				S_StartSound (NULL, sfx_slop);

				break;

			case PHYSICS_SELECTED:

				physics_mode++;

				if ( physics_mode > NUMPHYSICSMODES-1 )
					physics_mode = 0;

				break;

			case LIGHTING_SELECTED:

				Marshmallow_AlternateLighting++;

				if ( Marshmallow_AlternateLighting > NUMLIGHTLEVELS-1 )
					Marshmallow_AlternateLighting = 0;

				SHOW_MESSAGE DEH_String(CHANGEFFECT);

				break;

			case GOODIES_SELECTED:
				ToggleGoodiesMode();

				break;
			}
		}
	}

	if (gamekeydown[key_r] && offer_radsuit && offertimeout_radsuit)  // move into below block?
	{
		MAIN_PLAYER.message = DEH_String(USINGRADSUIT);
		MAIN_PLAYER.extra_powers[ITEM_RADSUIT] = false;
		offer_radsuit = false;
		offertimeout_radsuit = 0;
		P_GivePower(&MAIN_PLAYER, pw_ironfeet);
	}

	else  // else so that we don't activate both at once
	if (gamekeydown[key_r] && offer_medkit && offertimeout_medkit)  // move into below block?
	{
		UsePortableMedkit();	
	}

	// Take key input for our range selection option when PKE meter is on screen
	if (gamekeydown[key_r] && pkereadout_on)
    {
        if ( CheckKeyDelay() )
        return;

        switch (PKE_Meter.search_radius)
        {
            case PKE_RADIUS_SMALL:
                SHOW_MESSAGE DEH_String(PKERADIUS_STRING2);
                PKE_Meter.search_radius = PKE_RADIUS_MEDIUM;
            break;

            case PKE_RADIUS_MEDIUM:
                SHOW_MESSAGE DEH_String(PKERADIUS_STRING3);
                PKE_Meter.search_radius = PKE_RADIUS_LARGE;
            break;

            case PKE_RADIUS_LARGE:
                SHOW_MESSAGE DEH_String(PKERADIUS_STRING1);
                PKE_Meter.search_radius = PKE_RADIUS_SMALL;
            break;
        }

        PlayRadioNoise();
    }

	if (gamekeydown[key_inventory]) 
	{
		
		LaunchInventoryMenu();
	}

	if (Marshmallow_SaveItems)    // TODO: move this up before the offer_radsuit line?
	{
		if (invmenu_on)
		{
			if (MENUKEY_NEXT)  
			{
				if (CheckKeyDelay())
				return;

				CalcInvCursorPos(FORWARD);
			}

			if (MENUKEY_PREVIOUS)
			{
				if (CheckKeyDelay())
				return;
				
				CalcInvCursorPos(BACKWARD);
			}

			if (MENUKEY_SELECT && invmenu_selection == RADSUIT_SELECTED && MAIN_PLAYER.extra_powers[ITEM_RADSUIT])
			{
				MAIN_PLAYER.extra_powers[ITEM_RADSUIT] = false;
				P_GivePower (&MAIN_PLAYER, pw_ironfeet);
				MAIN_PLAYER.message = DEH_String(USINGRADSUIT);
				offer_radsuit = false;
				offertimeout_radsuit = 0;

				invmenu_on = false; // NEW!
			}
	

			if (MENUKEY_SELECT && invmenu_selection == INVUL_SELECTED  && MAIN_PLAYER.extra_powers[ITEM_INVUL])
			{
				MAIN_PLAYER.extra_powers[ITEM_INVUL] = false;
				P_GivePower (&MAIN_PLAYER, pw_invulnerability);
				MAIN_PLAYER.message = DEH_String(USINGINVUL);
				invmenu_on = false;
			}

			if (MENUKEY_SELECT && invmenu_selection == INVIS_SELECTED  && MAIN_PLAYER.extra_powers[ITEM_INVIS])
			{
				MAIN_PLAYER.extra_powers[ITEM_INVIS] = false;
				P_GivePower (&MAIN_PLAYER, pw_invisibility);
				MAIN_PLAYER.message = DEH_String(USINGINVIS);
				invmenu_on = false;
			}

			if (MENUKEY_SELECT && invmenu_selection == VISOR_SELECTED  && MAIN_PLAYER.extra_powers[ITEM_VISOR])
			{
				MAIN_PLAYER.extra_powers[ITEM_VISOR] = false;
				P_GivePower (&MAIN_PLAYER, pw_infrared);
				MAIN_PLAYER.message = DEH_String(USINGVISOR);
				invmenu_on = false;
			}

			//if (MENUKEY_SELECT && invmenu_selection == AUTOMAP_SELECTED  && MAIN_PLAYER.extra_powers[ITEM_AUTOMAP])
			//{
			//	MAIN_PLAYER.extra_powers[ITEM_AUTOMAP] = false;
			//	P_GivePower (&MAIN_PLAYER, pw_allmap);
			//	MAIN_PLAYER.message = DEH_String(USINGAUTOMAP);
			//	invmenu_on = false;
			//}

			if (MENUKEY_SELECT && invmenu_selection == MEDKIT_SELECTED  && MAIN_PLAYER.extra_powers[ITEM_MEDKIT])
			{
				UsePortableMedkit();
			}
		}
	}
}


void HUD_InitHelp()
{
    // Keyboard help display

	HUlib_initSText(&help_titlehelp_line1,
		TITLEHELP_LINE1_X, INV_HU_Y_2, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &mainmenuhelp_on);

	HUlib_initSText(&help_titlehelp_line2,
		TITLEHELP_LINE2_X, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenuhelp_on);

	HUlib_initSText(&help_titlehelp_line3,
		TITLEHELP_LINE3_X, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenuhelp_on);

	HUlib_initSText(&help_title,
		HELP_DISPLAY_X+10, INV_HU_Y_3, HU_MSGHEIGHT,    // +10 so it looks more centered
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_mainmenu,
		HELP_DISPLAY_X, INV_HU_Y_6, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_pkemeter,
		HELP_DISPLAY_X, INV_HU_Y_8, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &help_on);

    HUlib_initSText(&help_pkerange,
        HELP_DISPLAY_X, INV_HU_Y_9, HU_MSGHEIGHT,
        hu_font,
        HU_FONTSTART, &help_on);

	HUlib_initSText(&help_inventory,
		HELP_DISPLAY_X, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_botfollow,
		HELP_DISPLAY_X, INV_HU_Y_13, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_botassign,
		HELP_DISPLAY_X, INV_HU_Y_14, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_botusekey,
		HELP_DISPLAY_X, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_botshiftkey,
		HELP_DISPLAY_X, INV_HU_Y_17, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &help_on);

	HUlib_initSText(&help_togglehelp,
		HELP_DISPLAY_X, INV_HU_Y_20, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &help_on);

	// Sandbox design mode readout

	HUlib_initSText(&help_sandbox_object,
		SANDBOX_HELP_THING_X, INV_HU_Y_3 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &sandboxhelp_on);

	HUlib_initSText(&help_sandbox_scroll,
		SANDBOX_HELP_KEYS_X-15, INV_HU_Y_5 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &sandboxhelp_on);

	HUlib_initSText(&help_sandbox_select,
		SANDBOX_HELP_KEYS_X-15, INV_HU_Y_6 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &sandboxhelp_on);

	HUlib_initSText(&help_sandbox_battle,
		SANDBOX_HELP_KEYS_X-10, INV_HU_Y_8 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &sandboxhelp_on);
}


void HUD_InitEnemyMenu()
{
// enemy menu 

	HUlib_initSText(&enemymenu_fast,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_lostsoul,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_caco,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_demon,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_cacosound,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_cguysnd,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_skelmissile,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_12, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_skel,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_13, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_paindeath,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_14, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_vilepain,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_15, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);

	HUlib_initSText(&enemymenu_vilez,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &enemymenu_on);
}

void HUD_InitWeaponMenu()
{
// weapon menu   

	HUlib_initSText(&weaponmenu_truerandom,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_bersauto,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_bersred,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_csaw,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,  
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_fist,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_bullet,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_infammo,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_shotgun,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_12, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_plasma,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_13, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);

	HUlib_initSText(&weaponmenu_bfg,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_14, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &weaponmenu_on);
}


void HUD_InitSandboxMenu()
{
// sandbox menu							

		HUlib_initSText(&sandboxmenu_map,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &sandboxmenu_on);

		HUlib_initSText(&sandboxmenu_go,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &sandboxmenu_on);

		HUlib_initSText(&sandboxmenu_reset,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &sandboxmenu_on);

		HUlib_initSText(&sandboxmenu_cancel,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &sandboxmenu_on);
}


void HUD_InitMessagesMenu()
{
// messages menu:							

	HUlib_initSText(&messagesmenu_title,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_1, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);

	HUlib_initSText(&messagesmenu_blank,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);

	HUlib_initSText(&messagesmenu_danger,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);

	HUlib_initSText(&messagesmenu_targethp,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);	

	HUlib_initSText(&messagesmenu_missile,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);	

	HUlib_initSText(&messagesmenu_pickupmsg,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);	

	HUlib_initSText(&messagesmenu_deathmsg,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);

	HUlib_initSText(&messagesmenu_bossmsg,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);

	HUlib_initSText(&messagesmenu_infightmsg,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);	

	HUlib_initSText(&messagesmenu_extendedmsg,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);

	HUlib_initSText(&messagesmenu_extraline,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &msgsmenu_on);	
}

void HUD_InitMainMenu()
{
// main marshmallow menu:					

	HUlib_initSText(&MAINMENU_LINE1,
		MENU_X - 4, INV_HU_Y_1, HU_MSGHEIGHT,   // moving title back a few pixels
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE2,
		MENU_X, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE3,
		MENU_X, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE4,
		MENU_X, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE5,
		MENU_X, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE6,
		MENU_X, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE7,
		MENU_X, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE8,
		MENU_X, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE9,
		MENU_X, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);

	HUlib_initSText(&MAINMENU_LINE10,
		MENU_X, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &mainmenu_on);
}

void HUD_InitSkipLevelMenu()
{
	// skip level menu					

	HUlib_initSText(&skipmenu_next,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_redo,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_random,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_jump,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_skill,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_weapons,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_upgrade,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_13, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_scalehp,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_14, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_itemspawns,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

	HUlib_initSText(&skipmenu_go,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_19, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skipmenu_on);

    HUlib_initSText(&skipmenu_multiplier,
        FULLSCREEN_MENU_X_OFFSET+234, INV_HU_Y_14, HU_MSGHEIGHT,
        hu_font,
        HU_FONTSTART, &skipmenu_on);

    HUlib_initSText(&skipmenu_percent,
        FULLSCREEN_MENU_X_OFFSET+215, INV_HU_Y_13, HU_MSGHEIGHT,
        hu_font,
        HU_FONTSTART, &skipmenu_on);
}

void HUD_InitMusicMenu()
{
	// music menu						

	HUlib_initSText(&musicmenu_title,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_1, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_blank,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_mode,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_skip,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	//HUlib_initSText(&musicmenu_shuffle,
	//	FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
	//	hu_font,
	//	HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_msgs,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_length,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_fastchanges,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_13, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_playlist,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_15, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&songblacklist_marshmallow,
		FULLSCREEN_MENU_X_OFFSET+10, INV_HU_Y_17, HU_MSGHEIGHT,   // added +10 to appear indented
		hu_font,
		HU_FONTSTART, &musicmenu_on);

	HUlib_initSText(&musicmenu_game,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_19, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);


	// vanilla mode menu only:
	HUlib_initSText(&songblacklist_vanilla,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &musicmenu_on);


	// TODO: SONG NAME, COMPOSER?

}



void HUD_InitVanillaMusicMenu()
{
	// vanilla music menu

	HUlib_initSText(&musicmode_vanilla,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &vanillamusicmenu_on);

	HUlib_initSText(&songblacklist_vanilla,
		FULLSCREEN_MENU_X_OFFSET+10, INV_HU_Y_6, HU_MSGHEIGHT,   // +10 so it appears indented a bit
		hu_font,
		HU_FONTSTART, &vanillamusicmenu_on);
}



void HUD_InitBlacklistMenu()
{
	// blacklist menu						

	HUlib_initSText(&songblacklist_runnin,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &blacklistmenu_on);

	HUlib_initSText(&songblacklist_e1m1,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &blacklistmenu_on);

	HUlib_initSText(&songblacklist_d1inter,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &blacklistmenu_on);

	HUlib_initSText(&songblacklist_d2inter,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &blacklistmenu_on);
}


#define PKE_X_OFFSET 6

void HUD_InitMiscText()
{
	// Full PKE readout on datapad:

	HUlib_initSText(&pkeline1,
		MENU_X-PKE_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &pkereadout_on);

	HUlib_initSText(&pkeline2,
		MENU_X-PKE_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &pkereadout_on);

	HUlib_initSText(&pkeline3,
		MENU_X-PKE_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &pkereadout_on);

	HUlib_initSText(&pkeline4,
		MENU_X-PKE_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &pkereadout_on);

	HUlib_initSText(&pkeline8,
	    MENU_X, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &hellscape_on);

	// PKE_Widget (used to be called the "Hellscape Index")

	HUlib_initSText(&hellscape_text,
		HELLSCAPE_TEXT_X, HU_MSGY, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &hellscape_on);

	// for extra line of text:

	HUlib_initSText(&extra_textline,
		HU_MSGX, MARSHMALLOW_HU_PREVMSG_Y, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &message_on);

	// missile lock notification

	HUlib_initSText(&missilelock_text,
		MISSILELOCK_X, MISSILELOCK_Y, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &missilelock_on);

	// default inits for menu cursor position:

	cursor_y = INV_HU_Y_3;

	HUlib_initSText(&menu_cursor,
		cursor_x, cursor_y, HU_MSGHEIGHT,    
		hu_font,
		HU_FONTSTART, &menus_on);   

	// target HP readout:

	HUlib_initSText(&targethp_readout,
		TARGETHP_X, TARGETHP_Y, HU_MSGHEIGHT,    
		hu_font,
		HU_FONTSTART, &targethp_on);   

	// misc info readout:

	HUlib_initSText(&miscreadout_output[0],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_1 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,  // was HU_MSGX ?
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[1],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_2 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[2],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_3 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[3],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_4 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[4],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_5 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[5],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_6 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[6],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_7 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&miscreadout_output[7],
		INFO_OUTPUT_X-INFO_X_OFFSET, INV_HU_Y_8 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	// bot readout:				

	HUlib_initSText(&botreadout_orders,
		INFO_OUTPUT_X, INV_HU_Y_15, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &Marshmallow_ShowBotReadout);

	HUlib_initSText(&botreadout_health,
		INFO_OUTPUT_X, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &Marshmallow_ShowBotReadout);

	HUlib_initSText(&botreadout_state,
		INFO_OUTPUT_X, INV_HU_Y_17, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &Marshmallow_ShowBotReadout);

	HUlib_initSText(&botreadout_chat,
		INFO_OUTPUT_X, INV_HU_Y_18, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &Marshmallow_ShowBotReadout);

	// treasure readout:

	HUlib_initSText(&treasureinfo_line1,
		INFO_OUTPUT_X-8, INV_HU_Y_6 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,  // was HU_MSGX ?
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	HUlib_initSText(&treasureinfo_line2,
		INFO_OUTPUT_X-8, INV_HU_Y_8 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &miscreadout_on);

	// sandbox readout:

		// TODO...

	//HUlib_initSText(&miscreadout_output[0],
	//	INFO_OUTPUT_X, INV_HU_Y_1 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,  // was HU_MSGX ?
	//	hu_font,
	//	HU_FONTSTART, &miscreadout_on);

	//HUlib_initSText(&miscreadout_output[1],
	//	INFO_OUTPUT_X, INV_HU_Y_2 + INFOREADOUT_Y_OFFSET, HU_MSGHEIGHT,
	//	hu_font,
	//	HU_FONTSTART, &miscreadout_on);

}


void HUD_InitBotMenu()
{
// bot menu:				

	HUlib_initSText(&botmenu_title,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_1, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_blank,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_spawn,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_spawnall,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_remove,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_kill,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	//HUlib_initSText(&botmenu_states,
	//	FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
	//	hu_font,
	//	HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_bot1weapon,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_bot2weapon,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_bot3weapon,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_friendlyfire,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_12, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_speed,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_14, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	HUlib_initSText(&botmenu_statereadout,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &botmenu_on);

	//HUlib_initSText(&botmenu_collision,
	//	FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_14, HU_MSGHEIGHT,
	//	hu_font,
	//	HU_FONTSTART, &botmenu_on);
}


void HUD_InitGameplayMenu()
{
// gameplay options menu:			

	HUlib_initSText(&optionsmenu_title,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_1, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_blank,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_keepitems,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_conserve,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_goodies,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_treasure,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_ludicrous,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_physics,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_lighting,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_weapons,  
		FULLSCREEN_MENU_X_OFFSET+15, INV_HU_Y_12, HU_MSGHEIGHT,   // added +15 to appear indented
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_monsters,  
		FULLSCREEN_MENU_X_OFFSET+15, INV_HU_Y_13, HU_MSGHEIGHT,   // added +15 to appear indented
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_autouse,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_15, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_selfdmg,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_keepweapons,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_17, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_keepkeys,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_18, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);

	HUlib_initSText(&optionsmenu_weaponstay,  
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_19, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &optionsmenu_on);
}


void HUD_InitShortcutMenu()
{
    HUlib_initSText(&shortcutmenu_gameplay,
                    FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
                    hu_font,
                    HU_FONTSTART, &shortcutmenu_on);

    HUlib_initSText(&shortcutmenu_music,
                    FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
                    hu_font,
                    HU_FONTSTART, &shortcutmenu_on);

    HUlib_initSText(&shortcutmenu_messages,
                    FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
                    hu_font,
                    HU_FONTSTART, &shortcutmenu_on);
}


void HUD_InitInventoryMenu()
{
// inventory menu:			

	HUlib_initSText(&inv_menu_title,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_blank,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_empty,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_radsuit,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_invul,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_invis,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_medkit,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_visor,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	HUlib_initSText(&inv_menu_automap,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_11, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);

	
	// maybe move this one...

	HUlib_initSText(&inv_menu_treasure,
		HU_MSGX, INV_HU_Y_17, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &invmenu_on);
}


void HUD_InitSkillMenu()
{
// skill menu:												

	HUlib_initSText(&skillmenu_title,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_1, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_blank,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_baby,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_3, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_easy,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_medium,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_hard,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_6, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_nightmare,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_nightmare2,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);

	HUlib_initSText(&skillmenu_nmrespawn,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &skillmenu_on);
}


void HUD_InitProfileScreen()
{
// profile:				

	//HUlib_initSText(&PROFILE_LINE0,
	//	FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_1, HU_MSGHEIGHT,
	//	hu_font,
	//	HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE1,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_2, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE2,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_4, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE3,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_PERCENTSIGN,
		FULLSCREEN_MENU_X_OFFSET+125, INV_HU_Y_5, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE4,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_7, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE5,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_8, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE6,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_9, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE7,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_10, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE8,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_12, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE9,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_13, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE10,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_14, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE11,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_15, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE12,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_16, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE13,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_18, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	HUlib_initSText(&PROFILE_LINE14,
		FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_19, HU_MSGHEIGHT,
		hu_font,
		HU_FONTSTART, &profilescreen_on);

	//HUlib_initSText(&PROFILE_LINE15,
	//	FULLSCREEN_MENU_X_OFFSET, INV_HU_Y_20, HU_MSGHEIGHT,
	//	hu_font,
	//	HU_FONTSTART, &profilescreen_on);
}



void StartMenuCursor()
{
	if (mainmenu_on
		|| botmenu_on
		|| msgsmenu_on
		|| optionsmenu_on
		|| musicmenu_on
		|| vanillamusicmenu_on
		|| blacklistmenu_on
		|| invmenu_on
		|| skipmenu_on
		|| sandboxmenu_on
		|| enemymenu_on
		|| weaponmenu_on
		|| shortcutmenu_on
		//|| profilescreen_on  // when we do this, the stats vanish
		)
	{
		int i;

		menus_on = true;
		missilelock_on = false; // so this doesn't overlap with the menu display
			
		for (i=BOT_1;i<=BotsInGame;i++)  // this is to prevent main menu items showing up the same color as bot chat strings (don't know why this occurs)
			Bots[i].msgtimeout = NULL;
	}

	else if (!mainmenu_on   // NOTE: that this is a terrible way to handle turning off menus_on; do we even have all the menus in this list?
		&& !botmenu_on
		&& !msgsmenu_on
		&& !optionsmenu_on
		&& !musicmenu_on
		&& !invmenu_on
		&& !skipmenu_on
		&& !sandboxmenu_on
		&& !enemymenu_on
		&& !weaponmenu_on
		&& !shortcutmenu_on
		)
			menus_on = false;  
}






void HideAllMenus()
{
	pkereadout_on = false;
	skillmenu_on = false;
	invmenu_on = false;
	optionsmenu_on = false;
	botmenu_on = false;
	msgsmenu_on = false;
	musicmenu_on = false;
	skipmenu_on = false;
	sandboxmenu_on = false;
	weaponmenu_on = false;
	enemymenu_on = false;
	invmenu_on = false; 
	vanillamusicmenu_on = false;
	blacklistmenu_on = false;
	help_on = false; 
	profilescreen_on = false;
	treasure_on = false;
	shortcutmenu_on = false;

	mainmenu_on = false; 
	menus_on = false; 
}



static const char* maplist[2][64] = {
	{"RANDOM",
	 "E1M1","E1M2","E1M3","E1M4","E1M5","E1M6","E1M7","E1M8","E1M9",
	 "E2M1","E2M2","E2M3","E2M4","E2M5","E2M6","E2M7","E2M8","E2M9",
	 "E3M1","E3M2","E3M3","E3M4","E3M5","E3M6","E3M7","E3M8","E3M9",
	 "E4M1","E4M2","E4M3","E4M4","E4M5","E4M6","E4M7","E4M8","E4M9",
	 "E5M1","E5M2","E5M3","E5M4","E5M5","E5M6","E5M7","E5M8","E5M9"} ,

	{"RANDOM",
	 "MAP01","MAP02","MAP03","MAP04","MAP05","MAP06","MAP07","MAP08","MAP09", 
	 "MAP10","MAP11","MAP12","MAP13","MAP14","MAP15","MAP16","MAP17","MAP18",
	 "MAP19","MAP20","MAP21","MAP22","MAP23","MAP24","MAP25","MAP26","MAP27",
	 "MAP28","MAP29","MAP30","MAP31","MAP32"}
};


void ColorizeMapNames()
{
	int i;
	int color;

	//if (menuactive)
	//	color = CR_GRAY;
	//else
		color = CR_GOLD;

	for (i = 0; i < 64; i++)
	{
		if (gamemode == commercial)
		{
			if (maplist[DOOM2][i] == NULL)
				break;

			CrispyReplaceColor(maplist[DOOM2][i], color, maplist[DOOM2][i]);
		}
		else
		{
			if (maplist[DOOM1][i] == NULL)
				break;

			CrispyReplaceColor(maplist[DOOM1][i], color, maplist[DOOM1][i]);
		}
	}
}


char* ShowGameSelection()
{
	switch (newgame_mode)
	{
	case SINGLEPLAYER:
		return DEH_String(MODESP);

	case COOP:
		return DEH_String(MODECOOP);

	case DEATHMATCH:
		return DEH_String(MODEDM);

	case SANDBOX:
		return DEH_String(MODESBOX);
	}
}


char* ShowMapSelection()
{
	int game;

	if (gamemode == commercial)
		game = DOOM2;
	else
		game = DOOM1;

	return DEH_String(maplist[game][skip_to_level]);
}


char* DisplayRevMissileSpeed()
{
	if (Marshmallow_NerfSkelMissile)
		return DEH_String("    SLOW [PSX]");
	else
		return DEH_String("    FAST [DOS]");
}



char* DisplayBotSpeed()
{
	switch (default_bot_speed)
	{
	case BOTS_CAREFUL:
		return DEH_String("    CAREFUL");
	case BOTS_WALK:
		return DEH_String("    MODERATE");
	case BOTS_RUN:
		return DEH_String("    FAST");
	}
}



char* DisplayOnOff(boolean option)
{
	if (option)
		return DEH_String(OPTION_ON);
	else
		return DEH_String(OPTION_OFF);
}



char* DisplaySongStatus(boolean option)  
{
	if (option)
		return DEH_String(SONG_DISABLED);
	else
		return DEH_String(SONG_ENABLED);
}



char* DisplayCGuySnd(boolean option)
{
	if (option)
		return DEH_String(CHAINGUNLABEL);
	else
		return DEH_String(SHOTGUNLABEL);
}



char* DisplayMusicMode()
{
	if (Marshmallow_DynamicMusic)
		return DEH_String(DYNAMICMUSIC_ON);
	else
		return DEH_String(DYNAMICMUSIC_OFF);
}


char* DisplayRedScreenMode()
{
	switch (berserk_redscreen_length)
	{
	case BERSERK_REDSCREEN_OFF:
		return DEH_String(REDSCREENOFF);
	case BERSERK_REDSCREEN_SHORT:
		return DEH_String(REDSCREENSHORT);
	case BERSERK_REDSCREEN_VANILLA:
		return DEH_String(REDSCREENFULL);
	}
}


char* DisplayValue(int val)
{
	char output[16];
	//int  v;

	//v = val;
	
	//if (!val)
	//{
	//	sprintf(output, BLANK_LINE);  // this blank line thing could be causing a problem?
	//	return output;
	//}

	sprintf(output, "%d", val);

	return output;  // compiler warning here says: "returning address of local variable or temporary"
}


char* DisplayMedkitRemaining()  // nope... still doesn't work
{
	char output[16];
	int value;

	if (!invmenu_on)
		return "NONE";

	value = 100; //MAIN_PLAYER.medkit_remaining;

	sprintf(output, "%d", value);

	return DEH_String(output);
}


int ColorizeHealth(mobj_t* actor) 
{
	mobjtype_t type;
	int color, health;;
	
	health = actor->health;
	type = actor->type;

	if (type == MT_PLAYER)
	{
		if (health < 33)
			color = CR_DARK;
		else if (health < 66)
			color = CR_GOLD;
		else
			color = CR_GREEN;
	}
	else   // then it must be a monster
	{
		if (health < mobjinfo[type].spawnhealth * 0.33)
			color = CR_DARK;
		else if (health < mobjinfo[type].spawnhealth * 0.66)
			color = CR_GOLD;
		else
			color = CR_GREEN;
	}

	return color;

}



void ShowTargetHP()
{
	char output[16];  // in the other one we call it "buff"
	int health;
	//int color;

	mobj_t* target = MAIN_PLAYER.victim;

	if (!target
		|| !Marshmallow_ShowTargetHP)
			return;

	health = target->health;

	if (health > 0
		&& target->target_timeout     
	   )
	{
		targethp_on = true;

		//color = ColorizeHealth(target);   // diabled for now so these colors stop showing up on the PKE readout
		sprintf(output, "%d", health); 
		//CrispyReplaceColor( output, color, output);
		
		HUlib_addMessageToSText(&targethp_readout,DEH_String(TARGETHP), DEH_String(output));  //  WTF: WHY DOESNT DisplayValue() work here????????
	}
	else
	{
		targethp_on = false;
		
		//CrispyReplaceColor( output, ColorizeHealth(players[BOT_1].mo), output);
		
		HUlib_addMessageToSText(&targethp_readout, DEH_String(TARGETHP), BLANK_LINE); 
	}
}


char* ShowSkillLevel()
{
	switch (skill_selection)
	{
	case ITYTD_SELECTED:
		CrispyReplaceColor(SKILLS_BABY, CR_GREEN, SKILLS_BABY);
		return DEH_String(SKILLS_BABY);
		
	case HNTR_SELECTED:
		CrispyReplaceColor(SKILLS_EASY, CR_BLUE, SKILLS_EASY);
		return DEH_String(SKILLS_EASY);

	case HMP_SELECTED:
		CrispyReplaceColor(SKILLS_NORMAL, CR_GRAY, SKILLS_NORMAL);
		return DEH_String(SKILLS_NORMAL);

	case UV_SELECTED:
		CrispyReplaceColor(SKILLS_HARD, CR_DARK, SKILLS_HARD);
		return DEH_String(SKILLS_HARD);

	case NM_SELECTED:
		CrispyReplaceColor(SKILLS_NM, CR_GOLD, SKILLS_NM);
		return DEH_String(SKILLS_NM);

	case UV2_SELECTED:
		CrispyReplaceColor(SKILLS_UV2, CR_DARK, "UV");
		return DEH_String(SKILLS_UV2);
	
	case NM2_SELECTED:
		CrispyReplaceColor(SKILLS_NM2, CR_GOLD, "NM");
		return DEH_String(SKILLS_NM2);

	default:
		return DEH_String(SKILLS_NONE);
	}
}


char* ShowMapWeapons()
{
	if (Marshmallow_GradedWeapons)
		return DEH_String("PROGRESSIVE");
	else
		return DEH_String("PISTOL ONLY");
}


char* ShowRandomItemsMode()
{
	switch (Marshmallow_RandomItems)
	{
	case 0:  // macro?
		return DEH_String("OFF");
	case 1:  
		return DEH_String("BY TYPE");
	case 2:
		return DEH_String("FULL RANDOM");
	}
}


char* ShowThingName()
{
	player_t* p;
	p = &players[consoleplayer];

	if (p->sandbox_object == MT_POSSESSED)
		return "ZOMBIEMAN";

	if (p->sandbox_object == MT_SHOTGUY)	
		return "SHOTGUN SGT.";

    if (p->sandbox_object == MT_VILE) 
		return "ARCHVILE";

    if (p->sandbox_object == MT_UNDEAD)
		return "SKELETON";
    
    if (p->sandbox_object == MT_FATSO)
		return "MANCUBUS";
    
    if (p->sandbox_object == MT_CHAINGUY)
		return "CHAINGUNNER";

    if (p->sandbox_object == MT_TROOP)
		return "IMP";

    if (p->sandbox_object == MT_SERGEANT)
		return "DEMON";

    if (p->sandbox_object == MT_SHADOWS)
		return "SPECTRE";

    if (p->sandbox_object == MT_HEAD)
		return "CACODEMON";

    if (p->sandbox_object == MT_BRUISER)
		return "BARON";
    
    if (p->sandbox_object == MT_KNIGHT)
		return "HELL KNIGHT";

    if (p->sandbox_object == MT_SKULL)
		return "SKULL";
    
	if (p->sandbox_object == MT_SPIDER)
		return "SPIDER BOSS";

    if (p->sandbox_object == MT_BABY)
		return "BABY SPIDER";

    if (p->sandbox_object == MT_CYBORG)
		return "CYBERDEMON";

    if (p->sandbox_object == MT_PAIN)
		return "PAIN ELEMENTAL";

    if (p->sandbox_object == MT_WOLFSS)
		return "WOLF3D SS";

    if (p->sandbox_object == MT_KEEN)
		return "KEEN";

	if (p->sandbox_object == MT_BARREL)
		return "BARREL";

	//default:
		return "NO THING!";
}


void LaunchInventoryMenu()
{
	invitem_t i;

	if (CheckKeyDelay())
	return;

	if (realnetgame)
	{
		if (!treasure_on)
			treasure_on = true;
		else
			treasure_on = false;
	}
	
	if (!Marshmallow_SaveItems)  
	{
		//if (!realnetgame)
		//	MAIN_PLAYER.message = "INVENTORY FEATURE NOT ENABLED!";

		//if (!Marshmallow_TreasureMode)
			return;
	}

	S_StartSound(NULL, sfx_tink);

	if (invmenu_on)          
	{
		invmenu_on = false;

		// TODO: refresh status bar
	}
	else
	{
		HideAllMenus();
		invmenu_on = true;

		for (i=0; i<MAX_INV_ITEMS; i++)					// find the first item in our inventory and put the cursor there
		{
			if ( WeHaveItem(i) )
			{
				invmenu_selection = i;
				break;
			}
			else 
			{
				invmenu_selection = NO_SELECTION;
			}
		}
	}
}


char* ShowBotWeapon(int bot)
{
	if (!playeringame[bot])  // if bot doesn't exist
		return DEH_String("NOT PLAYING");

	switch (this_Bot.weaponmenu_selection)  // was Bots[bot].weapon
	{
	case BOTMENU_PISTOL:
		return DEH_String("PISTOL");
	case BOTMENU_SUPERSHOTGUN:
		return DEH_String("SUPERSHOTGUN");
	case BOTMENU_CHAINGUN:
		return DEH_String("CHAINGUN");
	case BOTMENU_SHOTGUN:
		return DEH_String("SHOTGUN");
	case BOTMENU_PLASMA:
		return DEH_String("PLASMA");
	case BOTMENU_MISSILE:
		return DEH_String("MISSILE");
	case BOTMENU_GREENPLASMA:
		return DEH_String("GREEN PLASMA");
	case BOTMENU_BFG:
		return DEH_String("BFG9000");
	case BOTMENU_RANDNOBFG:
		return DEH_String("RANDOM (NO BFG)");
	case BOTMENU_RANDBFG:
		return DEH_String("RANDOM (BFG)");
	case BOTMENU_AUTO:
		return DEH_String("AUTO");
	default:
		return DEH_String("NONE");
	}
}


static int SetCursorToSkill()
{
	if (gameskill == sk_baby)
		return sk_baby;
	if (gameskill == sk_easy)
		return sk_easy;
	if (gameskill == sk_medium)
		return sk_medium;
	if (gameskill == sk_hard)
		return sk_hard;
	if (Marshmallow_AlternateNightmare)
		return sk_nightmare + 1;
	if (gameskill == sk_nightmare)
		return sk_nightmare;
}


int ShowScore(int player)
{
	int score = 0;

	score += players[player].frags[0];
	score += players[player].frags[1];
	score += players[player].frags[2];
	score += players[player].frags[3];

	return score;
}


char* ShowGameType()
{
	if (Doom_DJ.game == DOOM2)
		return DEH_String(DOOM2LABEL);
	else if (Doom_DJ.game == DOOM1)
		return DEH_String(DOOMLABEL);
	else
		return DEH_String(BOTHLABEL);
}


char* DisplayTargetHPSetting(int input)
{
	if (input == 1)
		return DEH_String(TARGETHPALL);
	else if (input == 2)
		return DEH_String(TARGETHPBOSSES);
	else
		return DEH_String(OPTION_OFF);
}


char* DisplayTreasureMode()
{
	if (Marshmallow_TreasureMode == 1)
		return DEH_String(TREASURENORMAL);
	else if (Marshmallow_TreasureMode == 2)
		return DEH_String(TREASUREFULL);
	else
		return DEH_String(OPTION_OFF);			
}


char* DisplayGibMode()
{
	int gibmode = Marshmallow_GibMode;

	switch (gibmode)
	{
		case NO_GIBS:
			return DEH_String(VANILLA);
		case ROTT_GIBS:
			return DEH_String(LUDICROUS);
		case DUKE_GIBS:
			return DEH_String(GROOVY);
		case BRUTAL_GIBS:
			return DEH_String(BRUTAL);
	}
}


char* ShowSongLength()
{
	switch (Doom_DJ.song_length)
	{
		case SONGLENGTH_SHORT:
			return DEH_String(SHORTER);
		case SONGLENGTH_LONG:
			return DEH_String(LONGER);
		default:
			return DEH_String("NONE");
	}
}


char* ShowSongChanges()
{
	if (Marshmallow_FastMusicChanges)
		return DEH_String("ABRUPT");
	else
		return DEH_String("NORMAL");
}


char* DisplayPlaylistMode()
{
	//if (!musicmenu_on)
	//	return DEH_String("NONE");

	switch (Doom_DJ.play_mode)
	{
	case DYNAMIC:
		return DEH_String(PLAYLISTMODE1);
	case AMBIENT:
		return DEH_String(PLAYLISTMODE2);
	case SUSPENSEFUL:
		return DEH_String(PLAYLISTMODE3);
	case INTENSE:
		return DEH_String(PLAYLISTMODE4);
	case ALL_SONGS:
		return DEH_String(PLAYLISTMODE5);
	default:
		return DEH_String("NONE");
	}
}


char* DisplayPhysicsMode()
{
	switch (physics_mode)
	{
	case 0:
		return DEH_String(PHYSMODEOFF);
	case 1:
		return DEH_String(PHYSMODEONE);
	case 2:
		return DEH_String(PHYSMODETWO);
	default:
		return DEH_String("NONE");
	}
}


char* DisplayLightingMode()
{
	switch (Marshmallow_AlternateLighting)
	{
	case 0:
		return DEH_String(LIGHTINGOFF);
	case 1:
		return DEH_String(LIGHTINGCRT);
	case 2:
		return DEH_String(LIGHTINGDOOM3);
	default:
		return DEH_String("NONE");
	}
}


char* DisplayPKERadius()
{
    switch (PKE_Meter.search_radius)
    {
        case PKE_RADIUS_SMALL:
            return DEH_String(PKEHINT_SMALL);
        case PKE_RADIUS_MEDIUM:
            return DEH_String(PKEHINT_MEDIUM);
        case PKE_RADIUS_LARGE:
            return DEH_String(PKEHINT_LARGE);
        default:
            return DEH_String(" ");
    }
}