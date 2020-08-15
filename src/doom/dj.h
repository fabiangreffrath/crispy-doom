#ifndef __MARSHMALLOW_DJ__
#define __MARSHMALLOW_DJ__

#include "marshmallow.h"

#define SONG_LENGTH_SAMEPLAYLIST Doom_DJ.song_length
#define AREA_CLEAR_THRESHOLD 0.10f
#define SAFETY_THRESHOLD 8
#define	NUMSONGS 88

typedef int	songlist_t;
char*	songnames[NUMSONGS];

enum {
    SONGLENGTH_SHORT = 2500,
    SONGLENGTH_LONG = 3500,
    SONGLENGTH_INTENSE = 1100,
    SONGLENGTH_MINIMUM = 475,
    SONGLENGTH_D1VICTORY = 300,
    SONGLENGTH_D2VICTORY = 550,
};

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

enum
{
	BLACKLIST_RUNNIN,
	BLACKLIST_E1M1,
	BLACKLIST_D1INTER,
	BLACKLIST_D2INTER,
	
	BLACKLIST_SIZE
};

typedef enum {

	MUSICTHRESHOLD_SKILL1 = 200,
	MUSICTHRESHOLD_SKILL2 = 325,
	MUSICTHRESHOLD_SKILL3 = 375,
	MUSICTHRESHOLD_SKILL4 = 500,
	MUSICTHRESHOLD_SKILL5 = 500,

} music_threshold_t;


int GetNextTrackNum();
int RandomizeTrack();
int CheckForDuplicateSong(int next_song, int last_song);
void DJ_ImportPlaylist(songlist_t *songlist);
void DJ_StartPlaylist(musicstyle_t style);
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
boolean song_blacklist[BLACKLIST_SIZE];

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
	
	int				playlist[SONGLIST_SIZE];
	int				default_song;

	boolean			init;
	boolean			shuffle;

	boolean			autostart;

} Doom_DJ;


#endif