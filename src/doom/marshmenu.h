// Include for marshmallow code found in m_menu.c

#ifndef CRISPY_DOOM_MARSHMENU_H
#define CRISPY_DOOM_MARSHMENU_H

#define MENUHELP1 " ver. 0.79"
#define MENUHELP2 " Press 'H' for"
#define MENUHELP3 "Keyboard Ref."
#define MARSHMALLOW_DOOMLOGO_Y_OFFSET 7
#define MARSHMALLOW_MENU_Y_OFFSET 6
#define MARSHMALLOW_TITLE_X 112
#define MARSHMALLOW_TITLE_Y 1
#define WALLPAPER_TILE "FLOOR5_2"
#define UNPAUSED "GAME UNPAUSED."
#define TITLEHELP_LINE1_X 231
#define TITLEHELP_LINE1_Y 8
#define TITLEHELP_LINE2_X 222
#define TITLEHELP_LINE2_Y 31
#define TITLEHELP_LINE3_X 222
#define TITLEHELP_LINE3_Y 46
int shortcutmenu_selection;
boolean menus_on;
boolean mainmenu_breadcrumb;
boolean shortcutmenu_on;
boolean optionsmenu_on;
boolean help_on;
boolean profilescreen_on;
boolean pkereadout_on;
boolean mainmenuhelp_on;
boolean realnetgame;
boolean Marshmallow_GradedWeapons;
int Marshmallow_RandomItems;
boolean Marshmallow_AlternateUltraViolence;
boolean Marshmallow_AlternateNightmare;
boolean Marshmallow_RespawnInNightmare;
void M_Marshmallow(int choice);
void M_DrawMarshmallowNewGame(void);
void M_MarshmallowMenu();
void CrispyReplaceColor (char *str, const int cr, const char *col);
extern void PKE_Reset();
extern void LaunchHelpWidget();
extern void ChooseLevel_Next();
extern void ChooseLevel_Prev();
extern void Skill_Next();
extern void Skill_Prev();
extern void NewgameMode_Up();
extern void NewgameMode_Down();
extern void UpgradeChance_Up();
extern void UpgradeChance_Down();
extern void HPScale_Up();
extern void HPScale_Down();
extern void SkipToLevel();
extern void SkipToRandomLevel();
extern void CheckKeyDelay();
extern char* ShowMapSelection();
extern char* ShowSkillLevel();
extern char* ShowMapWeapons();
extern char* ShowRandomItemsMode();
extern void  ColorizeMapNames();
extern char* DisplayOnOff(boolean option);
extern char* ShowGameSelection();
extern void  StripWeapons(int player);
extern void ResetSandbox();
extern void SetSkills();
int upgrade_chance;
int MonsterHitpointsScale;
int newgame_mode;
int newskill;
int skill_selection;
extern void SetSpecialSkills();
extern boolean organic_levelchange;
extern void HideAllMenus();

#endif