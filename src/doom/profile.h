#ifndef __PROFILE__H__   
#define __PROFILE__H__


typedef struct {

	int	score;
	
	int blueskulls;
	int goldskulls;
	int redskulls;

	int total_in_level; 
	int remaining_in_level;
	boolean all_collected;  
	
} treasure_stats_t;

treasure_stats_t treasure_bag;  // our temporary cache of treasure collection data which is merged in to profile.cfg on organic level completion


typedef struct {

	int monsters_killed;

	int accuracy;
	int shots_fired;
	int shots_hit;
	int weapon_accuracy[NUMWEAPONS];
	int dm_frags;
	int kd_ratio;
	
	int favorite_skill;
	int favorite_weapon;  // most accurate weapon in the array above

	int players_teamkilled;
	int squad_kills;  // incremented on any kill by you or a team member
	int bosses_killed;  // count every spiderdemon and cyberdemon?  what about barons only if killed on E1M8?
	int secrets_found;  // do we keep counting the same secrets over and over?  probably yeah
	
	int alltreasure;
	int allkills;

	int sandbox_kills;
	int sandbox_games_won;
	int sandbox_bosses_killed;

	int levels_completed;  // only if exit switch is hit organically with no cheats
	int episodes_completed;  // only counts if all 8 maps were completed organically with no cheats
	int games_completed; // only when an entire iwad is completed organically

	// TODO: add intermission stats when any map is completed organically

} player_stats_t;

player_stats_t level_stats;  // filled-in during the level, then merged into profile.cfg on organic level completion


typedef struct {

	char player_name[16];
	int  color;

	treasure_stats_t treasure_bank;  // renamed to treasure_bank or similar since we using treasure_bag for cache instance
	player_stats_t stats;

	// date created?  or other time-based stats?

	int playtime_sp;
	int playtime_bots;
	int playtime_network;
	int playtime_sandbox;
	
} player_profile_t;

player_profile_t profile;  // moved out of player struct; we only need one global profile because this does not apply to multiple players


#endif