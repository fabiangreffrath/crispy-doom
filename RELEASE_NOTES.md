**General Fixes and Improvements**

 * Fix regression in uncapped automap mouse panning introduced by fast polling changes (@mikeday0)

**Crispy Doom**

 * Support KEX masterlevels.wad (@Noseey)
 * Fix A11Y flickering glitches (@Noseey)
 * Fix demo footer after level restart (@rfomin)
 * Fix `-levelstat` command (@rfomin)
 * Add support for non-power-of-2 wide textures (@elf-alchemist)
 * Consider patch offset when limit checking during finale, fix rust2.wad end screen (@mikeday0)
 * Add IDDST, IDDKT, IDDIT cheats, ported from Woof (@jiffygist)
 * Reduce memory consumption for swirling flats (@fabiangreffrath)
 * Don't let sideloaded PWADs replace default Doom 2 artwork (@fabiangreffrath)
 * Add support for Gib health feature from DOOM Retro (@fabiangreffrath)
 * Strafe in mid air was not strafe but forward / backward (@PascalCorpsman)

**Crispy Heretic**

 * Support Quick-Reverse (@Noseey)
 * Add Crosshairs (@Noseey)
 * Add A11Y Functions (@Noseey)
 * Add Map Markers (@Noseey)
 * Add Crispy Hud + Variations (@Noseey)
 * Add Translucency Option (@Noseey)
 * Add Weapon sound alignment Option, dragonclaw/gauntlets (@Noseey)
 * Support wide RAW images from new "Heretic + Hexen" release (@rfomin)
 * Add support for non-power-of-2 wide textures, support the new non-power-of-2 textures from the H+H Heretic IWAD (@elf-alchemist)
 * Make sky tutti-frutti fix work for any sky height (@mikeday0)
 * Add support for H+H remastered Heretic IWAD (@JNechaevsky)
 * Fix the artifact flash animation in uncapped framerate mode (@JNechaevsky)
 * Add IWAD search folders for the H+H rerelease (@JNechaevsky)
 * Add IDDT, IDDST, IDDKT, IDDIT cheats, ported from Woof (@jiffygist)

**Crispy Hexen**
 * Support Quick-Reverse (@Noseey)
 * Add Crosshairs (@Noseey)
 * Add A11Y Functions (@Noseey)
 * Add Map Markers (@Noseey)
 * Add Translucency Option (@Noseey)
 * Add Crispy Hud + Variations (@Noseey)
 * Support wide RAW images from new "Heretic + Hexen" release (@rfomin)
 * Add support for non-power-of-2 wide textures, Support the new non-power-of-2 textures from the H+H Hexen IWAD (@elf-alchemist)
 * Fix bug where saving a save on pages 1, 2 or 3 could delete save on pages 2, 4 or 6 (@mikeday0)
 * Make sky tutti-frutti fix work for any sky height (@mikeday0)
 * Fix the artifact flash animation in uncapped framerate mode (@JNechaevsky)
 * Add IWAD search folders for the H+H rerelease (@JNechaevsky)
 * Fix wyvern + porkalator bug, check for `P_FindMobjFromTID()` returning a `NULL` mobj in `DragonSeek()` (@fabiangreffrath)

**Crispy Strife**

 * Support Quick-Reverse (@Noseey)
 * Support persistent Map Markers (@Noseey)
 * Add A11Y Function, Flickering (@Noseey)
 * Add support for non-power-of-2 wide textures (@elf-alchemist)

