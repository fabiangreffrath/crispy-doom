# Marshmallow Doom

[![Icon](https://www.chocolate-doom.org/wiki/images/thumb/c/c9/Marshmallows.jpg/300px-Marshmallows.jpg)]

Marshmallow Doom is a source port which lightly sweetens classic Doom gameplay with new additions such as dynamic gameplay-driven music, cooperative bot players, persistent player stats, enhanced blood and gore, treasure items, new difficulty levels, customizable sandbox battles, a much-improved multiplayer experience, and more.

## Synopsis

Marshmallow Doom is a fork of the Crispy Doom source port. Itself being a fork of Chocolate Doom, Crispy Doom adds features such as 640x400 resolution, colored HUD text, removal of static engine limits, translucency, free vertical looking, jumping, and other modern improvements.  Marshmallow Doom greatly expands on this same ethos of a lightly-sweetened-yet-still-classic Doom gameplay experience.

## Objectives and features

Here is a partial list of Marshmallow Doom's new features:

    * Dynamic music soundtrack that morphs based on the player’s in-game experience, taking Bobby Prince’s original soundtrack to a whole new level
    
    * Re-balanced multiplayer experience adding modern features such as progressive weapon loadout, dropping ammo for squad mates, conserved item pickups, weapons stay, friendly fire on/off, and more
    
    * Bots available for cooperative or deathmatch games
    
    * Treasure mode helps give Doom an even more classic vibe like that of Commander Keen or Wolfenstein 3D
    
    * Sandbox mode allows the player (or multiple players in a network game) to build their own custom battles in any map
    
    * Monster upgrades can give any old map a new challenge, even repopulating Doom maps with Doom II monsters
    
    * New skill levels Ultra Violence 2.0 and Nightmare 2.0 utilize monster upgrading to create an entirely new standard for Doom’s difficulty
    
    * Inventory system allows the player to store items for future use, such as radsuits, invisibility, portable medkit, and more
    
    * Both Doom and Doom II IWADs can be combined so that music, sound, and graphics from both games can be used
    
    * Enhanced blood, gore, and epic boss deaths
    
    * The invisibility powerup now makes the player truly invisible to enemies, introducing a stealth tactic to help the player better survive any Doom level
    
    * Overplayed songs such as those from MAP01, E1M1, and intermission songs can be blacklisted, where an alternate song is randomly selected in its place
    
    * Player stats such as total kills and accuracy are tracked by the player profile
    
    * Various monster and weapon mutators
    
    * Most new options are quickly accessible via the player’s HUD datapad, without having to tediously dig into Doom’s options menus
    
    * All preferences are saved to disk for future sessions, and a separate server.cfg configuration file is available for multiplayer-specific game settings
    
For the full documentation, see the [wiki](https://www.chocolate-doom.org/wiki/index.php/Marshmallow_Doom).  (Alternate link [here](https://doomwiki.org/wiki/Marshmallow_Doom))

## Download

Download the latest Windows binary at http://www.marshmallowdoom.com.

## News

### April 30, 2020

Up until now Marshmallow Doom has been written and compiled in Microsoft Visual Studio 2012.  This was not ideal as this has made it nearly impossible to port the code to other platforms.  

Finally, the code has been moved into a new project that compiles with MinGW, and is also properly tracked using Git.  

As of now, only about one-third of Marshmallow Doom's features have been re-implemented in the new project, but forward progress is being made every day.  You can check each commit comment to see what has been implemented.

Stay tuned!

## Documentation

 * **[Website](http://www.marshmallowdoom.com)**
 * **[Wiki](https://www.chocolate-doom.org/wiki/index.php/Marshmallow_Doom)**
 * **[Wiki (alternate)](https://doomwiki.org/wiki/Marshmallow_Doom)**
 * **[FAQ](http://marshmallowdoom.com/marshmallow-wp/faq/)**

## Versioning

So far, Marshmallow Doom's version numbers have no significant meaning.  

In true Doom fashion, the first release was version 0.666, and they have progressed from there.  Now that the project is being developed in a more organized fashion, version numbers will likely be given more thought.

## Contact

Marshmallow Doom is maintained by [Douglas R. Bell](mailto:drbelljazz@removethisgmail.com). (aka JT_Marshmallow)

Please report any bugs, glitches or crashes that you encounter to the GitHub [Issue Tracker](https://github.com/drbelljazz/marshmallow-doom/issues).

## Legalese

_(this section left unchanged from Crispy Doom readme.md)_

Doom is © 1993-1996 Id Software, Inc.; 
Boom 2.02 is © 1999 id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman;
PrBoom+ is © 1999 id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman,
© 1999-2000 Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze,
© 2005-2006 Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko;
Chocolate Doom is © 1993-1996 Id Software, Inc., © 2005 Simon Howard; 
Chocolate Hexen is © 1993-1996 Id Software, Inc., © 1993-2008 Raven Software, © 2008 Simon Howard;
Strawberry Doom is © 1993-1996 Id Software, Inc., © 2005 Simon Howard, © 2008-2010 GhostlyDeath; 
Crispy Doom is additionally © 2014-2019 Fabian Greffrath;
all of the above are released under the [GPL-2+](https://www.gnu.org/licenses/gpl-2.0.html).

SDL 2.0, SDL_mixer 2.0 and SDL_net 2.0 are © 1997-2016 Sam Lantinga and are released under the [zlib license](http://www.gzip.org/zlib/zlib_license.html).

Secret Rabbit Code (libsamplerate) is © 2002-2011 Erik de Castro Lopo and is released under the [GPL-2+](http://www.gnu.org/licenses/gpl-2.0.html).
Libpng is © 1998-2014 Glenn Randers-Pehrson, © 1996-1997 Andreas Dilger, © 1995-1996 Guy Eric Schalnat, Group 42, Inc. and is released under the [libpng license](http://www.libpng.org/pub/png/src/libpng-LICENSE.txt).
Zlib is © 1995-2013 Jean-loup Gailly and Mark Adler and is released under the [zlib license](http://www.zlib.net/zlib_license.html).