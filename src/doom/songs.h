#define	NUMSONGS 88 // only for size of songnames[] array
#define NUMSONGS_DOOM1 45
#define NUMSONGS_END 82 

char* songnames[NUMSONGS] =
{
    "NONE",
    "At Doom's Gate",
    "The Imp's Song",
    "Dark Halls", //mus_e1m3,
    "Kitchen Ace (And Taking Names)", //mus_e1m4,
    "Suspense", //mus_e1m5,
    "On The Hunt", //mus_e1m6,
    "Demons On The Prey", //mus_e1m7,
    "Sign Of Evil", //mus_e1m8,
    "Hiding The Secrets", //mus_e1m9,
    "I Sawed The Demons", //mus_e2m1,
    "Demons From Adrian's Pen", //mus_e2m2,
    "Intermission from DOOM", //mus_e2m3,
    "They're Going To Get You", //mus_e2m4,
    "Demons On The Prey", //mus_e2m5,
    "Sinister", //mus_e2m6,
    "Waltz Of The Demons", //mus_e2m7,
    "Nobody Told Me About id", //mus_e2m8,
    "Untitled (Mouth For War)", //mus_e2m9,
    "Untitled (Mouth For War)", //mus_e3m1,
    "Donna To The Rescue", //mus_e3m2,
    "Deep Into The Code", //mus_e3m3,
    "Sign Of Evil", //mus_e3m4,
    "Demons On The Prey", //mus_e3m5,
    "On The Hunt", //mus_e3m6,
    "Waltz Of The Demons", //mus_e3m7,
    "Facing The Spider", //mus_e3m8,
    "Hiding The Secrets", //mus_e3m9,
    "Sign Of Evil", //mus_e4m1,
    "Donna To The Rescue", //mus_e4m2,
    "Deep Into The Code", //mus_e4m3,
    "Suspense", //mus_e4m4,
    "Waltz Of The Demons", //mus_e4m5,
    "They're Going To Get You", //mus_e4m6,
    "Sinister", //mus_e4m7,
    "Demons On The Prey", //mus_e4m8,
    "Hiding The Secrets", //mus_e4m9,
	"Hate Machine",//mus_e5m1
	"You Ain't the Boss O' Me",//mus_e5m2
	"Quell the Beast",//mus_e5m3
	"Riot Squadron",//mus_e5m4
	"Alice",//mus_e5m5
	"Besieged City",//mus_e5m6
	"Watching You",//mus_e5m7
	"Easel",//mus_e5m8
	"Adrenaline in the Blood",//mus_e5m9
    "Intermission from DOOM", //mus_inter,
    "DOOM Title Theme", //mus_intro,
    "Sweet Little Dead Bunny", //mus_bunny,
    "The End Of DOOM", //mus_victor,
    "DOOM Title Theme (OPL)", //mus_introa,
    "Running From Evil", //mus_runnin,
    "The Healer Stalks", //mus_stalks,
    "Countdown To Death", //mus_countd,
    "Between Levels", //mus_betwee,
    "DOOM", //mus_doom,
    "In The Dark", //mus_the_da,
    "Shawn's Got The Shotgun", //mus_shawn,
    "The Dave D. Taylor Blues", //mus_ddtblu,
    "Into Sandy's City", //mus_in_cit,
    "The Demon's Dead", //mus_dead,
    "The Healer Stalks", //mus_stlks2,
    "In The Dark", //mus_theda2,
    "DOOM", //mus_doom2,
    "The Dave D. Taylor Blues", //mus_ddtbl2,
    "Running From Evil", //mus_runni2,
    "The Demon's Dead", //mus_dead2,
    "The Healer Stalks", //mus_stlks3,
    "Waiting For Romero To Play", //mus_romero,
    "Shawn's Got The Shotgun", //mus_shawn2,
    "Message For The Archvile", //mus_messag,
    "Countdown To Death", //mus_count2,
    "The Dave D. Taylor Blues", //mus_ddtbl3,
    "Bye Bye American Pie", //mus_ampie,
    "In The Dark", //mus_theda3,
    "Adrian's Asleep", //mus_adrian,
    "Message For The Archvile", //mus_messg2,
    "Waiting For Romero To Play", //mus_romer2,
    "Getting Too Tense", //mus_tense,
    "Shawn's Got The Shotgun", //mus_shawn3,
    "Opening To Hell", //mus_openin,
    "Evil Incarnate", //mus_evil,
    "The Ultimate Challenge", //mus_ultima,
    "DOOM II Endgame", //mus_read_m,
    "DOOM II Title", //mus_dm2ttl,
    "DOOM II Intermission", //mus_dm2int,
    "NUMMUSIC" , //NUMMUSIC,
    "mus_musinfo" , //mus_musinfo
} ;


songlist_t doom2_standard_songs[SONGLIST_SIZE] = { mus_ddtblu, mus_doom, mus_openin , mus_dead, mus_adrian, mus_tense, mus_None };  
songlist_t doom2_ambient_songs[SONGLIST_SIZE] = {mus_stalks, mus_countd ,mus_romero ,mus_messag, mus_openin, mus_None };
songlist_t doom2_intense_songs[SONGLIST_SIZE] = {mus_ampie, mus_shawn, mus_in_cit, mus_betwee, mus_dm2int ,mus_None }; // removed mus_runnin and mus_evil
songlist_t doom2_boss_songs[SONGLIST_SIZE] = {mus_ultima, mus_runnin, mus_None };  // choose only one if we are looping the boss song
songlist_t doom2_victory_songs[SONGLIST_SIZE] = {mus_dm2ttl, mus_None };

songlist_t doom1_intense_songs[SONGLIST_SIZE] = { mus_e1m9, mus_e2m1, mus_e1m4, mus_e3m8, mus_inter , mus_e3m1, mus_None };  // removed mus_e3m2
songlist_t doom1_suspense_songs[SONGLIST_SIZE] = {mus_e1m6, mus_e1m5 ,  mus_e1m2,mus_e1m7 ,mus_e2m6 , mus_e2m8, mus_None };
songlist_t doom1_ambient_songs[SONGLIST_SIZE] = { mus_e1m3 , mus_e1m8 , mus_e2m2 , mus_e2m4 , mus_e2m7, mus_None };
songlist_t doom1_boss_songs[SONGLIST_SIZE] = { mus_e3m8, mus_e1m1, mus_None };  // removed mus_e1m4 for now
songlist_t doom1_victory_songs[SONGLIST_SIZE]= { mus_intro, mus_None }; // removed mus_victor

songlist_t both_intense_songs[SONGLIST_SIZE] = { mus_e1m9, mus_e2m1, mus_e1m4, mus_e3m8, mus_inter , mus_ampie, mus_None };  // removed mus_evil, mus_e3m2, and mus_in_cit
songlist_t both_suspense_songs[SONGLIST_SIZE] = {mus_e1m5 ,  mus_e1m2,mus_e1m7 ,mus_e2m6 , mus_e2m8, mus_ddtblu, mus_shawn ,mus_doom, mus_the_da, mus_openin , mus_dead, mus_adrian, mus_tense, mus_None };
songlist_t both_ambient_songs[SONGLIST_SIZE] = { mus_e1m3 , mus_e1m8 , mus_e2m2 , mus_e2m4 , mus_e2m7, mus_stalks, mus_countd ,mus_romero ,mus_messag, mus_None };
songlist_t both_boss_songs[SONGLIST_SIZE] = { mus_e3m8, mus_e1m1, mus_runnin, mus_None }; // removed mus_ultima and mus_inter
songlist_t both_victory_songs[SONGLIST_SIZE] = { mus_intro/*, mus_dm2ttl*/, mus_None };  // removed doom 2 title for now

songlist_t doom1_all_songs[SONGLIST_SIZE] = { mus_e1m6, mus_e1m5 , mus_e1m2,mus_e1m7 ,mus_e2m6 , mus_e2m8, mus_e3m1, mus_e1m9, mus_e2m1, mus_e1m4,   // removed mus_intro
											  mus_e3m8, mus_e3m2, mus_inter , mus_e1m1, mus_e1m3 , mus_e1m8 , mus_e2m2 , mus_e2m4 , mus_e2m7, mus_victor, mus_None } ;  // TODO: make sure we've got them all and check for duplicates

songlist_t doom2_all_songs[SONGLIST_SIZE] = { mus_ddtblu, mus_betwee, mus_shawn ,mus_doom, mus_the_da, mus_openin , mus_dead, mus_adrian, mus_tense, mus_evil, // removed mus_dm2ttl
											  mus_ampie, mus_openin, mus_in_cit, mus_runnin, mus_stalks, mus_countd ,mus_romero ,mus_messag, mus_ultima, mus_None } ;  // TODO: make sure we've got them all and check for duplicates

songlist_t both_all_songs[SONGLIST_SIZE] = {  mus_e1m6, mus_e1m5 , mus_e1m2,mus_e1m7 ,mus_e2m6 , mus_e2m8, mus_e3m1, mus_e1m9, mus_e2m1, mus_e1m4, mus_e3m8, 
											  mus_e3m2, mus_inter , mus_e1m1, mus_e1m3 , mus_e1m8 , mus_e2m2 , mus_e2m4 , mus_e2m7, mus_betwee, mus_victor,
											  mus_ddtblu, mus_shawn ,mus_doom, mus_the_da, mus_openin , mus_dead, mus_adrian, mus_tense, mus_evil, 
											  mus_ampie, mus_openin, mus_in_cit, mus_runnin, mus_stalks, mus_countd ,mus_romero ,mus_messag, mus_ultima, mus_None };


// NOTE: moved here from dj.c
boolean song_blacklist[BLACKLIST_SIZE] = { true,     // runnin
										   true,     // e1m1
										   false,    // d1inter
										   false };  // d2inter







// Another option for ordering the all_songs songlist:
#if 0
songlist_t both_all_songs[SONGLIST_SIZE] = 
{     
	mus_None,
    mus_e1m1,
    mus_e1m2,
    mus_e1m3,
    mus_e1m4,
    mus_e1m5,
    mus_e1m6,
    mus_e1m7,
    mus_e1m8,
    mus_e1m9,
    mus_e2m1,
    mus_e2m2,
    mus_e2m3,
    mus_e2m4,
    mus_e2m5,
    mus_e2m6,
    mus_e2m7,
    mus_e2m8,
    mus_e2m9,
    mus_e3m1,
    mus_e3m2,
    mus_e3m3,
    mus_e3m4,
    mus_e3m5,
    mus_e3m6,
    mus_e3m7,
    mus_e3m8,
    mus_e3m9,
    // [crispy] support dedicated music tracks for the 4th episode
    mus_e4m1,
    mus_e4m2,
    mus_e4m3,
    mus_e4m4,
    mus_e4m5,
    mus_e4m6,
    mus_e4m7,
    mus_e4m8,
    mus_e4m9,
    mus_inter,
    mus_intro,
    mus_bunny,
    mus_victor,
    mus_introa,
    mus_runnin,
    mus_stalks,
    mus_countd,
    mus_betwee,
    mus_doom,
    mus_the_da,
    mus_shawn,
    mus_ddtblu,
    mus_in_cit,
    mus_dead,
    mus_stlks2,
    mus_theda2,
    mus_doom2,
    mus_ddtbl2,
    mus_runni2,
    mus_dead2,
    mus_stlks3,
    mus_romero,
    mus_shawn2,
    mus_messag,
    mus_count2,
    mus_ddtbl3,
    mus_ampie,
    mus_theda3,
    mus_adrian,
    mus_messg2,
    mus_romer2,
    mus_tense,
    mus_shawn3,
    mus_openin,
    mus_evil,
    mus_ultima,
    mus_read_m,
    mus_dm2ttl,
    mus_dm2int,
    NUMMUSIC,
    mus_musinfo
} ;
#endif



// Idea for multi-dimensional playlist arrays    

//songlist_t suspense_songs[WHICH_GAME][SONGLIST_SIZE] = {
//	{mus_e1m5 ,  mus_e1m2,mus_e1m7 ,mus_e2m6 , mus_e2m8,mus_e3m1 , mus_None} ,
//	{ mus_ddtblu, mus_shawn ,mus_doom, mus_the_da, mus_openin , mus_dead, mus_adrian, mus_tense, mus_None}
//};
//
//songlist_t ambient_songs[WHICH_GAME][SONGLIST_SIZE] = {
//	{ mus_e1m3 , mus_e1m8 , mus_e2m2 , mus_e2m4 , mus_e2m7, mus_None },
//	{mus_stalks, mus_countd ,mus_romero ,mus_messag, mus_None},
//};
//
//songlist_t intense_songs[WHICH_GAME][SONGLIST_SIZE] = {
//	{ mus_e1m9, mus_e2m1, mus_e1m4, mus_e3m8, mus_e3m2,mus_inter , mus_e1m1, mus_None},
//	{mus_evil, mus_ampie, mus_openin, mus_in_cit, mus_runnin, mus_None},
//};
//
//songlist_t boss_songs[WHICH_GAME][SONGLIST_SIZE] = {
//	{ mus_e1m1, mus_inter, mus_None  },  
//	{mus_ultima, mus_runnin, mus_None  }, 
//};
//
//songlist_t victory_songs[WHICH_GAME][SONGLIST_SIZE] = {
//	{ mus_intro, mus_victor, mus_None  },
//	{mus_dm2ttl, mus_None  } ,
//};  