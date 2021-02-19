#ifndef __EGON__
#define __EGON__

#include "marshmallow.h"

extern void CrispyReplaceColor (const char *str, const int cr, const char *col);

#define MAXOBJECTS 1000  // More than enough?
#define DRAW_DELAY 25
#define PKE_RECALC_DELAY 25   // So we aren't spamming all of our scanning stuff every single tic
#define THIS_MOBJ_HP (mobjinfo[ monster->type ].spawnhealth * MonsterHitpointsScale)  // TODO: Try using real-time monster.health instead of spawnhealth
#define THIS_MOBJ_CURRENTHP monster.health  // Not used yet; possible alternative to the macro above

#define ZERO_STRING "O"
#define POSITIVE_COLOR CR_GREEN
#define NULL_COLOR CR_DARK

typedef enum {

	PKE_RADIUS_SMALL = (MISSILERANGE/6) ,  // Approx. same room
	PKE_RADIUS_MEDIUM = (MISSILERANGE/2), // Approx. one room away
	PKE_RADIUS_LARGE = (MISSILERANGE),   // Approx. most of the map
} pke_radius_t;

typedef enum {

    DANGERLEVEL_LOW = 100,
    DANGERLEVEL_MEDIUM = 1000,
    DANGERLEVEL_HIGH = 4000,
    DANGERLEVEL_CRITICAL = 10000,
    DANGERLEVEL_FUBAR = 50000,

} danger_threshold_t;

void PKE_Activate();
void PKE_Reset();
void PKE_Scanner();
void BuildObjList();
void PKE_KillBoss();
int PKE_GetDangerLevel();
int PKE_MonstersInArea();
void PKE_HUDisplay();
void PKE_ShowInfo();
void PKE_Readout();
void AllKills_Achievement();
void PKE_KillEntity(mobj_t* entity);

typedef struct pkemeter_s {

	pke_radius_t search_radius;

	int monsters_in_level;
	int monsters_remaining;

	int highdanger_threshold;
	int safety_time;

	int healthpoints_detected;
	int monsters_detected;
	mobj_t* monsters[MAXOBJECTS];

	int monsters_onradar;
	int healthpoints_onradar;

	int attackers_hp;
	int attackers_total;
	boolean attacker_index[MAXOBJECTS];

	boolean bossfight;
	mobj_t* boss;  

	short draw_delay;
	short heartbeat;

} pkemeter_t;

pkemeter_t PKE_Meter;


#endif