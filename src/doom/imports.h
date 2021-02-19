/*
#include "..\src\doom\p_mobj.h"
#include "..\src\doom\d_player.h"
#include "..\src\m_argv.h"
#include "..\src\doom\p_inter.h"
#include "..\src\doom\g_game.h"
#include "..\src\doom\p_local.h"
#include "..\src\doom\doomstat.h"
#include "..\src\m_cheat.h"
#include "..\src\doom\info.h"
#include "..\src\doom\sounds.h"
#include "..\src\doom\d_englsh.h"
#include "..\src\doom\hu_lib.h"
#include "..\src\m_controls.h"
#include "..\src\v_trans.h"
#include "..\src\d_iwad.h"
#include "..\src\doom\m_random.h"
#include "..\src\z_zone.h"
#include "SDL_endian.h"
*/

#include "deh_str.h"
#include "p_mobj.h"
#include "d_player.h"
#include "m_argv.h"
#include "p_inter.h"
#include "g_game.h"
#include "p_local.h"
#include "doomstat.h"
#include "m_cheat.h"
#include "info.h"
#include "sounds.h"
#include "d_englsh.h"
#include "hu_lib.h"
#include "m_controls.h"
#include "v_trans.h"
#include "d_iwad.h"
#include "w_wad.h"
#include "m_random.h"
#include "z_zone.h"
#include "SDL_endian.h"

// Prototypes for vanilla doom objects that we'll need:
void *W_CacheLumpName(const char *name, int tag);
void CrispyReplaceColor (char *str, const int cr, const char *col);

int showMessages;
void P_KillMobj ( mobj_t*	source, mobj_t*	target );
boolean P_GiveAmmo ( player_t*	player, ammotype_t	ammo, int num, boolean	dropped );
//mobj_t* P_SpawnMobj ( fixed_t	x, fixed_t	y, fixed_t	z, mobjtype_t	type );
void HU_queueChatChar(char c);
boolean P_GiveBody ( player_t*	player, int		num );
boolean P_GiveWeapon( player_t*	player,  weapontype_t	weapon,  boolean	dropped );
void P_GiveCard( player_t*	player,  card_t	card );
void S_StartSound(void *origin_p, int sfx_id);
//extern const char *player_names[4];
//extern int M_snprintf(char *buf, size_t buf_len, const char *s, ...);
//extern void PlayerQuitGame(player_t *player);
//extern mapthing_t	playerstarts[MAXPLAYERS];
void A_FaceTarget (mobj_t* actor);
void A_Look(mobj_t*	actor);
void A_Chase (mobj_t*	actor);
//boolean P_CheckSight( mobj_t*	t1,  mobj_t*	t2 );
boolean P_Move (mobj_t*	actor);
boolean P_TryWalk (mobj_t* actor);
void P_NewChaseDir (mobj_t*	actor);
//int P_Random (void);
boolean P_CheckMeleeRange (mobj_t*	actor);
//char *DEH_String(char *s);
//boolean M_StringCopy(char *dest, const char *src, size_t dest_size);
boolean playeringame[MAXPLAYERS];
//void P_UseLines (player_t*	player);
boolean	PTR_UseTraverse (intercept_t* in);
boolean		onground;
void P_CalcHeight (player_t* player) ;
boolean P_CheckMissileRange (mobj_t* actor);
void P_Thrust (player_t* player, angle_t angle, fixed_t move);
int P_SubRandom (void);
mobj_t*		usething;   
void P_MovePlayer (player_t* player);
void A_Recoil (player_t* player);
#define INVERSECOLORMAP		32
#define ANG5   	(ANG90/18)
void S_ChangeMusic(int music_id, int looping);
void ST_doPaletteStuff(void);
void P_RadiusAttack( mobj_t*	spot,  mobj_t*	source,  int	damage );
boolean gamekeydown[256];
boolean menuactive;
#define BONUSADD 6
extern fixed_t forwardmove[2]; 
extern fixed_t sidemove[2]; 
extern fixed_t angleturn[3];    // + slow turn 
#define SHORT(x)  ((signed short) SDL_SwapLE16(x))
#define HU_MSGX			0
#define HU_MSGY			0  // don't change this
#define HU_MSGHEIGHT	1	// in lines
#define HU_FONTSTART	'!'	// the first font characters
#define HU_FONTEND	'_'	// the last font characters
#define HU_FONTSIZE	(HU_FONTEND - HU_FONTSTART + 1)	
patch_t*		hu_font[HU_FONTSIZE];
extern boolean message_on;
void S_ChangeMusic(int musicnum, int looping);
void S_StartMusic(int m_id);
int M_Random (void);
void S_SetMusicVolume(int volume);
void V_DrawPatch(int x, int y, patch_t *patch);
void W_MergeFile(char *filename);
void S_StopMusic();
boolean S_MusicPlaying();
boolean P_LookForPlayers( mobj_t* actor, boolean allaround );
void V_DrawFilledBox(int x, int y, int w, int h, int c);
void V_DrawBox(int x, int y, int w, int h, int c);
extern void A_FirePistol(mobj_t* mobj, player_t* player, pspdef_t*	psp);
extern void A_FireShotgun(mobj_t* mobj, player_t* player, pspdef_t*	psp);
extern void A_FireShotgun2(mobj_t* mobj, player_t* player, pspdef_t* psp);
extern void A_FireCGun( mobj_t* mobj, player_t* player, pspdef_t* psp );
extern void P_BulletSlope (mobj_t*	mo);
extern void P_GunShot( mobj_t*	mo, boolean	accurate );
extern void V_DrawPatchDirect(int x, int y, patch_t *patch);
extern void V_DrawTLPatch(int x, int y, patch_t *patch);
extern void V_MarkRect(int x, int y, int width, int height);
boolean debugmode;
extern int deh_initial_bullets;
extern void A_Light0(mobj_t *mobj, player_t *player, pspdef_t *psp);
extern void A_Light1(mobj_t *mobj, player_t *player, pspdef_t *psp);
extern boolean chat_on;