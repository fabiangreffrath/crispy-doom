#ifndef __EGON__
#define __EGON__

#include "marshmallow.h"

extern void CrispyReplaceColor (char *str, const int cr, const char *col);

#define MAXOBJECTS 1000  // more than enough?
#define DRAW_DELAY 25
#define PKE_RECALC_DELAY 25   // so we aren't spamming all of our scanning stuff every single tic
#define THIS_MOBJ_HP (mobjinfo[ monster->type ].spawnhealth * MonsterHitpointsScale)  // TODO: try using real-time monster.health instead of spawnhealth
#define THIS_MOBJ_CURRENTHP monster.health  // not used yet; possible alternative to the macro above

typedef enum {

	PKE_RADIUS_SMALL = (MISSILERANGE/6) ,  // approx. same room
	PKE_RADIUS_MEDIUM = (MISSILERANGE/4), // approx. one room away
	PKE_RADIUS_LARGE = (MISSILERANGE) , // approx. most of the map

} pke_radius_t;


typedef struct pkemeter_s {

	pke_radius_t search_radius;

	int monsters_in_level;
	int monsters_remaining;

	int highdanger_threshold;
	int safety_time;

	int healthpoints_detected;
	int monsters_detected;
	mobj_t* monsters[MAXOBJECTS];
	mobj_t* temp;  // was monster in old struct; now rename to entity_detected or something

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


void PKE_Activate();
void PKE_Reset();
void PKE_Sensor();
void PKE_Scanner();
//void PKE_CheckIfAllKills();  // NOTE: this might have to move to a stat-related method when we implement a stat system
void BuildObjList(); 
void PKE_KillStaypuft();
void PKE_GetAttacker(mobj_t* actor);
void PKE_KillEntity(mobj_t* target);  // rename input
void PKE_StayPuft();
void PKE_HUDisplay();
void PKE_ShowInfo();
void PKE_Readout();
int PKE_GetDangerLevel();
void AllKills_Achievement();
//boolean MonstersInArea(); 
int PKE_MonstersInArea();

//void RemoveMonsterFromRadar(mobj_t* monster); // static
//void AddMonsterToRadar(mobj_t* monster); // static
//void AddMonsterToList(mobj_t* monster);  // static
//boolean PKE_Heartbeat(); // static
//boolean PKE_Analyze(mobj_t* entity);  // static


typedef enum {

	DANGERLEVEL_LOW = 100, 
	DANGERLEVEL_MEDIUM = 1000, 
	DANGERLEVEL_HIGH = 4000, 
	DANGERLEVEL_CRITICAL = 10000, 
	DANGERLEVEL_FUBAR = 50000,

} danger_threshold_t;  


#endif