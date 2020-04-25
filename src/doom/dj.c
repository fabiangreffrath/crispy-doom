/////////////////////////////
//
//  DJ.C:  Dynamic Music
//
/////////////////////////////


#include "dj.h"
#include "songs.h"
#include "pkemeter.h" 

 
static void DJ_MusicStates()  
{
	musicstyle_t music_state = Doom_DJ.music_state;
	int song_tic = Doom_DJ.musictic;

	if ( DJ_BossFight() )  
		return;

	if ( DJ_Victory() )					
		return;

	if ( DJ_Sandbox() )  
		return;

	if ( DJ_SongMinimum() )   
		return; 
	
	// Change to AMBIENT playlist when most monsters in the level have been eliminated
	if ( AreaClear() )  
	{
		switch (music_state)
		{
		case AMBIENT:

			if (song_tic > SONG_LENGTH_SAMEPLAYLIST)
				DJ_NextTrack();

			break;

		case INTENSE:

			if (song_tic > INTENSE_SONG_LENGTH / 2)
				DJ_StartPlaylist(AMBIENT);

			break;

		case SUSPENSEFUL:

			if (song_tic > SONG_LENGTH_SAMEPLAYLIST / 2)
				DJ_StartPlaylist(AMBIENT);

			break;
		}
	}

	// Trigger INTENSE playlist during high-danger monster encounters
	else if ( HighDanger() ) 
	{
		if ( PlayerIsDead() || spawntics < 100 )
			return;

		if (PLAYING_DOOM1)
		{
			if (E1M8 || E2M8 || E3M8)  // Save the intense music for the boss playlist on these maps
				return;
		}

		switch (music_state)
		{
		case INTENSE:
			
			if (song_tic > SONG_LENGTH_SAMEPLAYLIST)
				DJ_NextTrack();

			break;

		default:

			DJ_StartPlaylist(INTENSE);

		}
	}

	// By default we stay in the SUSPENSE playlist unless overriden by any of the conditions above
	else if ( LowDanger() )   
	{
		switch (music_state)
		{
		case SUSPENSEFUL:

			if (song_tic > SONG_LENGTH_SAMEPLAYLIST)
				DJ_NextTrack();

			break;

		case INTENSE:

			if (song_tic > INTENSE_SONG_LENGTH)
				DJ_StartPlaylist(SUSPENSEFUL);

			break;
		}
	}
}


void DJ_StartPlaylist(musicstyle_t style)  
{
	int      next_song, last_song;		
	int		 game = Doom_DJ.game;
	boolean	 looping = true;

	// Set the playlist considering which songs are available
	switch (style)
	{
	case AMBIENT:

		if ( DJ_Msg_On() )
	//if (Marshmallow_DJMessages)
			SHOW_MESSAGE DEH_String(AMBIENTSONGS);

		if (game == DOOM2)
			DJ_ImportPlaylist(doom2_ambient_songs);
		else if (game == DOOM1)
			DJ_ImportPlaylist(doom1_ambient_songs);
		else
			DJ_ImportPlaylist(both_ambient_songs);

		break;

	case SUSPENSEFUL:

		if ( DJ_Msg_On() )
		//if (Marshmallow_DJMessages)
			SHOW_MESSAGE DEH_String(SUSPENSEFULSONGS);

		if (game == DOOM2)
			DJ_ImportPlaylist(doom2_standard_songs);
		else if (game == DOOM1)
			DJ_ImportPlaylist(doom1_suspense_songs);
		else
			DJ_ImportPlaylist(both_suspense_songs);

		break;

	case INTENSE:

		if ( DJ_Msg_On() )
		//if (Marshmallow_DJMessages)
			SHOW_MESSAGE DEH_String(INTENSESONGS);

		if (game == DOOM2)
			DJ_ImportPlaylist(doom2_intense_songs);
		else if (game == DOOM1)
			DJ_ImportPlaylist(doom1_intense_songs);
		else
			DJ_ImportPlaylist(both_intense_songs);

		break;

	case BOSS:

		if ( DJ_Msg_On() )
		//if (Marshmallow_DJMessages)
			SHOW_MESSAGE DEH_String(BOSSSONGS);

		if (game == DOOM2)
			DJ_ImportPlaylist(doom2_boss_songs);
		else if (game == DOOM1)
			DJ_ImportPlaylist(doom1_boss_songs);
		else
			DJ_ImportPlaylist(both_boss_songs);

		break;

	case VICTORY:

		if ( DJ_Msg_On() )
		//if (Marshmallow_DJMessages)
			SHOW_MESSAGE DEH_String(VICTORYSONGS);

		if (game == DOOM2)
			DJ_ImportPlaylist(doom2_victory_songs);
		else if (game == DOOM1)
			DJ_ImportPlaylist(doom1_victory_songs);
		else
			DJ_ImportPlaylist(both_victory_songs);

		looping = false;  // Don't loop our victory song

		break;

	case ALL_SONGS:

		if ( DJ_Msg_On() )
		//if (Marshmallow_DJMessages)
			SHOW_MESSAGE DEH_String(ALLSONGS);

		if (game == DOOM2)
			DJ_ImportPlaylist(doom2_all_songs);
		else if (game == DOOM1)
			DJ_ImportPlaylist(doom1_all_songs);
		else
			DJ_ImportPlaylist(both_all_songs);

		break;
	}

	// Choose the next track number in our playlist
	Doom_DJ.track_playing = GetNextTrackNum();
	next_song = Doom_DJ.song[ Doom_DJ.track_playing ];

	// Make sure we didn't pick the same INTENSE song twice in a row 
	if (style == INTENSE)
	{
		// Set the last song if zero
		if (!Doom_DJ.last_intense_song)
		{
			Doom_DJ.last_intense_song = next_song;
		}
		else
		{
			last_song = Doom_DJ.last_intense_song;
			next_song = CheckForDuplicateSong(next_song, last_song);  
			Doom_DJ.last_intense_song = next_song;
		}
	}

	// Save our new music state
	Doom_DJ.music_state = style;

	// Play it!
	S_ChangeMusic(next_song, looping);  
}


int GetNextTrackNum()
{
	int next_track;

	if ( !Doom_DJ.shuffle )
	{
		return TRACK_1;
	}

	RefreshShuffledTracks();

	next_track = RandomizeTrack();		

	if ( !next_track )
		next_track = TRACK_1;

	return next_track; 
}


boolean AreaClear()
{
	float threshold = (PKE_Meter.monsters_in_level * AREA_CLEAR_THRESHOLD);  
	int remaining = PKE_Meter.monsters_remaining;

	// Don't report area clear if we still have a pile of monsters attacking us
	if ( (PKE_Meter.attackers_hp > PKE_Meter.highdanger_threshold 
		|| PKE_Meter.safety_time < SAFETY_THRESHOLD) 
		&& !Marshmallow_FastMusicChanges )
	{
		return false;
	}

	if ( remaining < threshold )
		return true;
	else
		return false;
}


boolean LowDanger()
{
	int dangerlevel = PKE_GetDangerLevel();
	int threshold = PKE_Meter.highdanger_threshold;

	// If we haven't been in safety for very long, don't switch moods yet
	if ( PKE_Meter.safety_time < SAFETY_THRESHOLD )
	{
		if (Doom_DJ.musictic > Doom_DJ.song_length)
			DJ_NextTrack();

		return false;
	}
	
	if (dangerlevel < threshold)
		return true;
	else
		return false;
}


boolean HighDanger()
{
	int dangerlevel = PKE_GetDangerLevel();
	int threshold = PKE_Meter.highdanger_threshold;

	if ( PlayerIsDead() )
		return false;
	
	if (dangerlevel > threshold)
		return true;
	else
		return false;
}


void SetMusicThresholds()
{
	float modifier;  
	
	if (!upgrade_chance)
		modifier = 0;
	else
		modifier = (upgrade_chance * 0.01);

	modifier *= 1.5;  

	switch (gameskill)
	{
	case sk_baby:
		PKE_Meter.highdanger_threshold = (MUSIC_THRESHOLD_SKILL1 + MUSIC_THRESHOLD_SKILL1*modifier) * MonsterHitpointsScale;   
		break;
	case sk_easy:
		PKE_Meter.highdanger_threshold = (MUSIC_THRESHOLD_SKILL2 + MUSIC_THRESHOLD_SKILL2*modifier) * MonsterHitpointsScale;
		break;
	case sk_medium:
		PKE_Meter.highdanger_threshold = (MUSIC_THRESHOLD_SKILL3 + MUSIC_THRESHOLD_SKILL3*modifier) * MonsterHitpointsScale;
		break;
	case sk_hard:
		PKE_Meter.highdanger_threshold = (MUSIC_THRESHOLD_SKILL4 + MUSIC_THRESHOLD_SKILL4*modifier) * MonsterHitpointsScale;
		break;
	case sk_nightmare:
		PKE_Meter.highdanger_threshold = (MUSIC_THRESHOLD_SKILL5 + MUSIC_THRESHOLD_SKILL5*modifier) * MonsterHitpointsScale;
		break;
	}
}


static void DJ_BossDeath()  
{
	if (PLAYING_DOOM1) 
	{
		if (E2M8)   // Episode 2 boss
		{
			DJ_StartPlaylist(VICTORY);	 
		}

		if (E3M8)  // Episode 3 boss
		{
			DJ_StartPlaylist(VICTORY);	 
		}
	}
	else
	{
		DJ_StartPlaylist(VICTORY);	  // Doom 2 (MAP32 only)
	}
}


boolean DJ_BossFight()
{
	if (Marshmallow_Sandbox)
		return false;

	if (PKE_Meter.bossfight)
	{
		if (!PKE_Meter.boss)
			return false;   

		if (PKE_Meter.boss->health <= 0 && Doom_DJ.music_state != VICTORY)
		{
			DJ_BossDeath();	 
			return false;   // Boss was killed
		}
		else 
		{
			return true;   // Boss remains
		}
	}	
	else
	{
		return false;   // Not a bossfight
	}
}


boolean DJ_Victory()
{
	int length;
	int song_tic = Doom_DJ.musictic;
	musicstyle_t music_state = Doom_DJ.music_state;

	if ( GetGameType() == DOOM2 )
		length = DOOM2_VICTORY_SONG_LENGTH;
	else
		length = DOOM1_VICTORY_SONG_LENGTH;

	if (music_state == VICTORY)
	{
		if (song_tic > length) 
		{
			if (Marshmallow_Sandbox)
				DJ_StartPlaylist(SUSPENSEFUL);
			else
				DJ_StartPlaylist(AMBIENT);
			
			return false;
		}
		else
			return true;  // Victory state overrides any other music state change 
	}
	else
	{
		return false;
	}
}


boolean DJ_SongMinimum()
{
	int song_tic = Doom_DJ.musictic;

	if (PLAYING_DOOM2)
	{
		if (MAP07)
			return false;  // On MAP07, just let 'er rip
	}

	if (Marshmallow_FastMusicChanges)
		return false;

	if (song_tic < SONG_MINIMUM_LENGTH)   
		return true;  // Aborting calling function			
	else
		return false;  // Continuing through calling function
}


boolean DJ_Sandbox()
{
	int song_tic = Doom_DJ.musictic;

	if (Marshmallow_Sandbox) 
	{
		if (song_tic > SONG_LENGTH_SAMEPLAYLIST)
			 DJ_NextTrack();

		return true;    // Aborting calling function
	}
	else
	{
		return false;   // Continuing through calling function
	}
}


void DJ_NextTrack() 
{
	track_t  new_song;			

	if (Doom_DJ.shuffle)
		Doom_DJ.track_playing = RandomizeTrack();
	else
		Doom_DJ.track_playing++;

	if (Doom_DJ.track_playing >= Doom_DJ.playlist_length)
	{
		Doom_DJ.track_playing = TRACK_1;

		if (Marshmallow_DJMessages && !Doom_DJ.shuffle)
		SHOW_MESSAGE "END OF PLAYLIST. RESTARTING.";		
	}
	else
	{
		if (Marshmallow_DJMessages)
		SHOW_MESSAGE DEH_String(NEXTSONG);	
	}

	new_song = Doom_DJ.song[ Doom_DJ.track_playing ];

	S_ChangeMusic(new_song, true);   // Looping in case the song is shorter than our SONG_LENGTH
}


int GetReplacementSong()  
{
	int randomsong;
	int game = GetGameType();
	
	if (Marshmallow_DynamicMusic)
	{
		RefreshShuffledTracks(); 
		Doom_DJ.track_playing = RandomizeTrack();		

		randomsong = Doom_DJ.song[ Doom_DJ.track_playing ];
	}
	else
	{
		switch (game)
		{
		case BOTH:
			randomsong = GetRandomIntegerInRange(1, NUMSONGS_END);  // Both WADs are available so pick any song
			break;

		case DOOM2:
			randomsong = GetRandomIntegerInRange(mus_runnin, mus_ultima);  // Doom2-only songs 
			break;

		case DOOM1:
			randomsong = GetRandomIntegerInRange(mus_e1m1, mus_e3m9);  // Doom1-only songs 
			break;
		}

		// Check for unwanted songs 
		if ( randomsong > mus_e3m9 && randomsong < mus_runnin )
		{
			if (PLAYING_DOOM1)
				randomsong = mus_e1m2;
			else
				randomsong = mus_evil;  
		}
	}

	return randomsong; 
}


boolean IsBlacklistedSong(int musicnum)
{
	if (realnetgame)
		return false;

	switch (musicnum)
	{
	case mus_runnin:
		if (song_blacklist[BLACKLIST_RUNNIN])
		return true;

		break;

	case mus_e1m1:
		if (song_blacklist[BLACKLIST_E1M1])
		return true;

		break;

	case mus_inter:
		if (song_blacklist[BLACKLIST_D1INTER])
		return true;

		break;

	case mus_dm2int:
		if (song_blacklist[BLACKLIST_D2INTER])
		return true;

		break;
	}

	return false;
}


void ToggleSong_Runnin()
{
	if (song_blacklist[BLACKLIST_RUNNIN] == true)  // Enable the song
	{
		song_blacklist[BLACKLIST_RUNNIN] = false;

		if (PLAYING_DOOM2)
		{
			// If this happens to be the current level's default song, start playing it now
			if ( (MAP01 || MAP15)    
				&& !Marshmallow_DynamicMusic )
			{
				S_StartMusic(mus_runnin);
			}
		}
	}
	else  // Disable the song
	{
		song_blacklist[BLACKLIST_RUNNIN] = true;

		if (Marshmallow_DynamicMusic)
		{
			SHOW_MESSAGE "REMOVED FROM SONG SELECTION.";
			return;
		}

		if (PLAYING_DOOM2 && gamemap == 1)
		{
			int newsong = GetReplacementSong();  
			Doom_DJ.default_song = newsong;   
			S_StartMusic(newsong); 
		}
	}
}


void ToggleSong_E1M1()
{
	if (song_blacklist[BLACKLIST_E1M1] == true)  // Enable the song
	{
		song_blacklist[BLACKLIST_E1M1] = false;

		// If this happens to be the current level's default song, start playing it now
		if (PLAYING_DOOM1)
		{
			if (E1M1							
				&& !Marshmallow_DynamicMusic)
			{
				S_StartMusic(mus_e1m1);
			}
		}
	}
	else  // Disable the song
	{
		song_blacklist[BLACKLIST_E1M1] = true;

		if (Marshmallow_DynamicMusic)
		{
			SHOW_MESSAGE "REMOVED FROM SONG SELECTION.";
			return;
		}

		if (PLAYING_DOOM1)
		{
			if (E1M1) 
			{
				int newsong = GetReplacementSong();   
				Doom_DJ.default_song = newsong;   
				S_StartMusic(newsong); 
			}
		}
	}
}


void ToggleSong_DoomInter()
{
	boolean song_disabled;

	song_blacklist[BLACKLIST_D1INTER] = !song_blacklist[BLACKLIST_D1INTER];
	song_disabled = song_blacklist[BLACKLIST_D1INTER];

	if (Marshmallow_DynamicMusic && song_disabled)
		SHOW_MESSAGE "REMOVED FROM SONG SELECTION.";
}


void ToggleSong_Doom2Inter()
{
	boolean song_disabled;

	song_blacklist[BLACKLIST_D2INTER] = !song_blacklist[BLACKLIST_D2INTER];
	song_disabled = song_blacklist[BLACKLIST_D2INTER];

	if (Marshmallow_DynamicMusic && song_disabled)
		SHOW_MESSAGE "REMOVED FROM SONG SELECTION.";
}


void DJ_ImportPlaylist(songlist_t *songlist)  
{
	int		i;

	for (i = 0; i < SONGLIST_SIZE; i++)
	{
		if (songlist[i] == mus_None)
		{
			Doom_DJ.playlist_length = i;
			break;
		}

		Doom_DJ.song[i] = songlist[i];
	}
}


// If we picked the same intense-style song twice in a row, randomly select a different song
int CheckForDuplicateSong(int next_song, int last_song)
{
	if (next_song == last_song)  
	{
		int new_song;
			
		if (debugmode)
			SHOW_MESSAGE "FOUND DUPLICATE INTENSE SONG!";

		do { 
			new_song = GetReplacementSong();  
		}  while (new_song == last_song);    
		  
		next_song = new_song;
	}

	return next_song;
}


void ChangeGameMusic()
{
	Doom_DJ.game++;

	if (Doom_DJ.game > BOTH)
		Doom_DJ.game = DOOM1;  
				
	DJ_StartPlaylist(Doom_DJ.music_state);
}


void ForcePlaylist()
{
	Doom_DJ.play_mode++;
	
	if (Doom_DJ.play_mode >= BOSS)  
	{
		if (deathmatch)
			Doom_DJ.play_mode = AMBIENT;
		else
			Doom_DJ.play_mode = DYNAMIC;
	}

	if (Doom_DJ.play_mode == DYNAMIC)
		DJ_StartPlaylist(SUSPENSEFUL);
	else
		DJ_StartPlaylist(Doom_DJ.play_mode);
}


void DynamicMusic()  
{
	if (!Marshmallow_DynamicMusic)  
		return;			

	if (!Doom_DJ.init)
		InitDynamicMusic();    

	if (Doom_DJ.play_mode == DYNAMIC)	
	{
		DJ_MusicStates();
	}
	else if (Doom_DJ.musictic > SONG_LENGTH_SAMEPLAYLIST)  // Handle next track if we are using a forced playlist
	{
		DJ_NextTrack(); 
	}
}


void ChangeMusicMode()  
{
	if (Marshmallow_DynamicMusic)  
		S_ChangeMusic(Doom_DJ.default_song , true);
	else
		InitDynamicMusic();

	Marshmallow_DynamicMusic = !Marshmallow_DynamicMusic;
}


void InitMusic()
{
	if (!Doom_DJ.autostart)
		return;

	InitDynamicMusic();
}


void InitDynamicMusic()
{
	if (M_CheckParm("-nomusic"))  
	{
		Marshmallow_DynamicMusic = false;
		return;
	}

	Doom_DJ.init = true;
	Doom_DJ.shuffle = true; 
	Doom_DJ.autostart = false;

	Doom_DJ.game = GetGameType();

	if (deathmatch)
	{
		DJ_StartPlaylist(ALL_SONGS);
		Doom_DJ.play_mode = ALL_SONGS;
	}
	else
		DJ_StartPlaylist(SUSPENSEFUL);	
}


void HandleMusicOnRespawn()
{
	if (netgame || realnetgame)
		return;

	if (Doom_DJ.init)  // So we don't do this when the game starts
		DJ_StartPlaylist(SUSPENSEFUL);
}


void ChangeSongLength()
{
	if (Doom_DJ.song_length == SHORT_SONG_LENGTH)
		Doom_DJ.song_length = LONG_SONG_LENGTH;
	else
		Doom_DJ.song_length = SHORT_SONG_LENGTH;
}


void ResetMusicTic()
{
	Doom_DJ.musictic = NULL; 
}


boolean DJ_Msg_On()
{
	if (Marshmallow_Sandbox)
		return false;

	if (Marshmallow_DJMessages)
		return true;
	else
		return false;
}


void DJ_ShowInfo()
{
	AddToInfoReadout("music_tic: ", Doom_DJ.musictic, 1);
	AddToInfoReadout("bossfight: ", PKE_Meter.bossfight, 2);
	AddToInfoReadout("safety_time: ", PKE_Meter.safety_time, 3);
	AddToInfoReadout("last_intense_song: ", Doom_DJ.last_intense_song, 4);
	AddToInfoReadout("track_playing: ", Doom_DJ.song[ Doom_DJ.track_playing ], 5);	
	AddToInfoReadout("music_threshold: ", PKE_Meter.highdanger_threshold, 6);	

	AddToInfoReadout("marshmallow_tic: ", marshmallow_tic, 8);	
}


int PlutoniaSong(int song)
{
	switch (song)
	{
	case mus_e1m2:
		song = mus_runnin;
		break;
	case mus_e1m3:
		song = mus_stalks;
		break;
	case mus_e1m6:
		song = mus_countd;
		break;
	case mus_e1m4:
		song = mus_betwee;
		break;
	case mus_e1m9:
		song = mus_doom;
		break;
	case mus_e1m8:
		song = mus_theda2;
		break;
	case mus_e2m1:
		song = mus_shawn;
		break;
	case mus_e2m2:
		song = mus_ddtblu;
		break;
	case mus_e3m3:
		song = mus_in_cit;
		break;
	case mus_e1m7:
		song = mus_dead;
		break;
	}
}


///////////////////////////////
//
//  Randomized track selection
//
///////////////////////////////

#define MAX_TRACKS			128

int shuffledTracks[MAX_TRACKS];
int shuffledTrackIndex = 0;
int numShuffledTracks = 0;
int isShuffledTracksInitialized = false;

// [ceben 2017]
void RefreshShuffledTracks()
{
	int j, k;
	int playlistLowerBound, playlistUpperBound;

	playlistLowerBound = 0;
	playlistUpperBound = Doom_DJ.playlist_length - 1;
	numShuffledTracks = playlistUpperBound - playlistLowerBound + 1;  // Oh?
	
	for (k = playlistLowerBound, j = 0; k <= playlistUpperBound; k++, j++) {
		shuffledTracks[j] = k;
	}
	shuffledTrackIndex = 0;
	RandomizeIntArray(shuffledTracks, numShuffledTracks);
	isShuffledTracksInitialized = true;
}

// [ceben 2017]
int GetShuffledTrack()
{
	int track;

	if (!isShuffledTracksInitialized || ++shuffledTrackIndex >= numShuffledTracks)
	{
		RefreshShuffledTracks();
	}

	track = shuffledTracks[shuffledTrackIndex];
	return track;
}

// [ceben 2017]
int RandomizeTrack()
{
	int trackIndex;
	int size = Doom_DJ.playlist_length;

	trackIndex = GetShuffledTrack();

	return trackIndex;
}