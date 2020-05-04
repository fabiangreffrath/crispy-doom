#ifndef __MARSHMALLOW_DJ__
#define __MARSHMALLOW_DJ__

#include "marshmallow.h"

typedef int	songlist_t;

typedef enum
{
	TRACK_1,  
	SONGLIST_SIZE = 128,
} track_t;  


typedef enum
{
	DYNAMIC,
	AMBIENT,
	SUSPENSEFUL,
	INTENSE,
	ALL_SONGS,  
	BOSS,       
	VICTORY,    
} musicstyle_t;


enum {  

	BLACKLIST_RUNNIN,
	BLACKLIST_E1M1,
	BLACKLIST_D1INTER,
	BLACKLIST_D2INTER,
	
	BLACKLIST_SIZE
};

boolean song_blacklist[BLACKLIST_SIZE];  

#define SONG_LENGTH_SAMEPLAYLIST Doom_DJ.song_length  

#define SHORT_SONG_LENGTH 2500 
#define LONG_SONG_LENGTH 3500 
#define INTENSE_SONG_LENGTH 1100
#define SONG_MINIMUM_LENGTH 475 
#define DOOM1_VICTORY_SONG_LENGTH 300 
#define DOOM2_VICTORY_SONG_LENGTH 550

#define AREA_CLEAR_THRESHOLD 0.10f 
#define SAFETY_THRESHOLD 8

#define	NUMSONGS 88

// TODO: switch to using the below enum (includes new names)

/*
enum {

	SONGLENGTH_SHORT = 2500,
	SONGLENGTH_LONG = 3500,
	SONGLENGTH_INTENSE = 1100,
	SONGLENGTH_MINIMUM = 420,
	SONGLENGTH_D1VICTORY = 300,
	SONGLENGTH_D2VICTORY = 550,
	//SONGLENGTH_SAMEMOOD  couldn't do this one

};
*/

// TODO: switch to using the below enum (includes new names)

#define MUSIC_THRESHOLD_SKILL1 250
#define MUSIC_THRESHOLD_SKILL2 325
#define MUSIC_THRESHOLD_SKILL3 375 
#define MUSIC_THRESHOLD_SKILL4 500  
#define MUSIC_THRESHOLD_SKILL5 500  

/*
typedef enum {

	MUSICTHRESHOLD_SKILL1 = 200,
	MUSICTHRESHOLD_SKILL2 = 300,
	MUSICTHRESHOLD_SKILL3 = 420,
	MUSICTHRESHOLD_SKILL4 = 500,
	MUSICTHRESHOLD_SKILL5 = 500,

} music_threshold_t;
*/

int GetNextTrackNum();
int RandomizeTrack();
int CheckForDuplicateSong(int next_song, int last_song);
void DJ_ImportPlaylist(songlist_t *songlist);
void DJ_StartPlaylist(musicstyle_t style);

//void DJ_MusicStates();  // static
void DJ_DoFadeout();
void DJ_StartFadeout();
void DJ_NextTrack();
void DJ_ShowInfo();
void InitDynamicMusic();
void HandleMusicOnRespawn();
void ResetMusicTic();
void RefreshShuffledTracks();
void SetMusicThresholds();
boolean DJ_BossFight();
boolean DJ_Victory();
boolean HighDanger();
boolean LowDanger();
boolean AreaClear();
boolean DJ_Sandbox();
boolean DJ_SongMinimum();

char*				songnames[NUMSONGS]; 
char*				plut_songnames[NUMSONGS]; 

struct
{
	musicstyle_t	play_mode;  // Will be DYNAMIC most of the time but this also allows us to force certain playlists to start if needed
	musicstyle_t	music_state; // When in DYNAMIC play_mode, this is our current music "mood"
	int				game;
	int				musictic;

	track_t			track_playing;   // Only an index of each song in the playlist, NOT the actual song id number from musicenum_t
	int				playlist_length;  
	int				last_intense_song;
	int				song_length;

    int	            current_song;  // Used for displaying song name in menus
	
	int				song[SONGLIST_SIZE];  // TODO: maybe rename to songlist or playlist
	int				default_song;

	boolean			init;
	boolean			shuffle;

	boolean			autostart;

} Doom_DJ;


#endif