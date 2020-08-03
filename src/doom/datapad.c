/////////////////////////////////////////////////////////
//
//  DATAPAD.C:  HUD "datapad" like Unreal, Quake2, etc.
//
/////////////////////////////////////////////////////////

#include "datapad.h"
#include "pkemeter.h"

//extern void *W_CacheLumpName(const char *name, int tag);

void Draw_DatapadBorders1()
{
	int k, offset = 0;

	// top border
	for ( k=0; k<MAX_COLUMNS; k++)
	{
		V_DrawPatchDirect(DATAPAD_TOP1_X+offset, DATAPAD_TOP_Y, W_CacheLumpName(DEH_String(BORDER_TILE), PU_CACHE));
		offset += GRAYPANEL_COLUMN_WIDTH;
	}

	// bottom border
	offset = 0;

	for ( k=0; k<MAX_COLUMNS; k++)
	{
		V_DrawPatchDirect(DATAPAD_TOP1_X+offset, DATAPAD_BOTTOM_Y, W_CacheLumpName(DEH_String(BORDER_TILE), PU_CACHE));
		offset += GRAYPANEL_COLUMN_WIDTH;
	}
}


void Draw_Datapad1()  // unused
{
	Draw_DatapadBorders1();

	// the "screen" - top half
	V_DrawPatchDirect(DATAPAD1_LINE1_SCREEN_TILE1_X, DATAPAD1_LINE1_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));
	V_DrawPatchDirect(DATAPAD1_LINE1_SCREEN_TILE2_X, DATAPAD1_LINE1_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));
	V_DrawPatchDirect(DATAPAD1_LINE1_SCREEN_TILE3_X, DATAPAD1_LINE1_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));

	// the "screen" - bottom half
	V_DrawPatchDirect(DATAPAD1_LINE2_SCREEN_TILE1_X, DATAPAD1_LINE2_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));
	V_DrawPatchDirect(DATAPAD1_LINE2_SCREEN_TILE2_X, DATAPAD1_LINE2_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));
	V_DrawPatchDirect(DATAPAD1_LINE2_SCREEN_TILE3_X, DATAPAD1_LINE2_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));

	// handle - top half
	V_DrawPatchDirect(DATAPAD1_LINE1_LEFT_X, DATAPAD1_LINE1_Y, W_CacheLumpName(DEH_String(GRAYPANEL_COLUMN_WITHBLUE_LEFT), PU_CACHE));
	V_DrawPatchDirect(DATAPAD1_LINE1_RIGHT_X, DATAPAD1_LINE1_Y, W_CacheLumpName(DEH_String(GRAYPANEL_COLUMN_WITHBLUE_RIGHT), PU_CACHE));

	// handle - bottom half
	V_DrawPatchDirect(DATAPAD1_LINE2_LEFT_X, DATAPAD1_LINE2_Y, W_CacheLumpName(DEH_String(GRAYPANEL_COLUMN_WITHBLUE_LEFT), PU_CACHE));
	V_DrawPatchDirect(DATAPAD1_LINE2_RIGHT_X, DATAPAD1_LINE2_Y, W_CacheLumpName(DEH_String(GRAYPANEL_COLUMN_WITHBLUE_RIGHT), PU_CACHE));
}


void SetDatapadBackground()
{
	if (gamemode == commercial)
		background = DOOM2_SUBSTITUTE;
	else
		background = BLUE_TECHBASE_TEXTURE;

	if (Marshmallow_WadStealing)
		background = BLUE_TECHBASE_TEXTURE;
	
}

#define PKE_BLINKTIMER 50

void PKE_BlinkingLight()
{
    if (!blink_timer)
    {
        int dangerlevel = PKE_Meter.healthpoints_onradar;

        if (dangerlevel <= DANGERLEVEL_LOW)
        {
            blink_timer = PKE_BLINKTIMER*5;
        }

        if (dangerlevel <= DANGERLEVEL_MEDIUM)
        {
            blink_timer = PKE_BLINKTIMER*4;
        }

        if (dangerlevel <= DANGERLEVEL_HIGH)
        {
            blink_timer = PKE_BLINKTIMER*3;
        }

        if (dangerlevel <= DANGERLEVEL_CRITICAL)
        {
            blink_timer = PKE_BLINKTIMER*2;
        }

        if (dangerlevel <= DANGERLEVEL_FUBAR)
        {
            blink_timer = PKE_BLINKTIMER*1;
        }
    }
    else
        blink_timer--;

    if (blink_timer > blink_timer/2)
        V_DrawPatchDirect(DATAPAD2_DISKLED_X, DATAPAD2_DISKLED_Y, W_CacheLumpName(DEH_String(YELLOWLIGHT_TILE), PU_CACHE));

    else if (blink_timer <= blink_timer/2)
        V_DrawPatchDirect(DATAPAD2_DISKLED_X, DATAPAD2_DISKLED_Y, W_CacheLumpName(DEH_String(REDLIGHT_TILE), PU_CACHE));
}


void BlinkingLight()
{
    if (!blink_timer)
        //blink_timer = GetRandomIntegerInRange( 120, BLINK_TIMER );  // TRYING RANDOM HERE...
        blink_timer = BLINK_TIMER;
    else
        blink_timer--;

    if (blink_timer > BLINK_TIMER/2)
        V_DrawPatchDirect(DATAPAD2_DISKLED_X, DATAPAD2_DISKLED_Y, W_CacheLumpName(DEH_String(YELLOWLIGHT_TILE), PU_CACHE));

    else if (blink_timer <= BLINK_TIMER/2)
        V_DrawPatchDirect(DATAPAD2_DISKLED_X, DATAPAD2_DISKLED_Y, W_CacheLumpName(DEH_String(REDLIGHT_TILE), PU_CACHE));
}


void Draw_Datapad2()
{
	if ( GetGameType() == DOOM2 
		|| gameversion == exe_chex)
	{
		// TODO: find the right Doom2-only textures for an alternate datapad design

		return;
	}

	// floppy disk sticking out at the top
	V_DrawPatchDirect(DATAPAD2_DISK_X, DATAPAD2_DISK_Y, W_CacheLumpName(DEH_String("STDISK"), PU_CACHE));  // [WTF] rare crash in here

	// screen
	//V_DrawFilledBox(55, 25, 250, 100, 5);  // now if we could just make this box dark green...

    V_DrawPatchDirect(DATAPAD2_OLDBLUETILE_X, DATAPAD2_OLDBLUETILE_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));
    V_DrawPatchDirect(DATAPAD2_OLDBLUETILE_X+64, DATAPAD2_OLDBLUETILE_Y, W_CacheLumpName(DEH_String(BLUE_TECHBASE_TEXTURE), PU_CACHE));
	
	// handles
    V_DrawPatchDirect(DATAPAD2_LEFTHANDLE, DATAPAD2_Y, W_CacheLumpName(DEH_String(GRAYPANEL_COLUMN_WITHBLUE_LEFT), PU_CACHE));
    V_DrawPatchDirect(DATAPAD2_RIGHTHANDLE, DATAPAD2_Y, W_CacheLumpName(DEH_String(GRAYPANEL_COLUMN_WITHBLUE_RIGHT), PU_CACHE));

	// buttons
    V_DrawPatchDirect(DATAPAD2_BUTTON1_X, DATAPAD2_BUTTON1_Y, W_CacheLumpName(DEH_String(BUTTON_TILE), PU_CACHE));
    V_DrawPatchDirect(DATAPAD2_BUTTON2_X, DATAPAD2_BUTTON2_Y, W_CacheLumpName(DEH_String(BUTTON_TILE), PU_CACHE));
    V_DrawPatchDirect(DATAPAD2_BUTTON3_X, DATAPAD2_BUTTON3_Y, W_CacheLumpName(DEH_String(BUTTON_TILE), PU_CACHE));

	// lights
	//V_DrawPatchDirect(DATAPAD2_POWERLED_X+3, DATAPAD2_POWERLED_Y, W_CacheLumpName(DEH_String(REDLIGHT_TILE), PU_CACHE));

	//if (pkereadout_on)
	//    PKE_BlinkingLight();
	//else
	    BlinkingLight();
}


void Draw_WallpaperBorders()
{
	// left edge
	V_DrawPatchDirect(LEFT_BORDER_X, LEFT_BORDER_Y, W_CacheLumpName(DEH_String(LEFT_BORDER_TILE), PU_CACHE));
	V_DrawPatchDirect(LEFT_BORDER_X, LEFT_BORDER_Y+64, W_CacheLumpName(DEH_String(LEFT_BORDER_TILE), PU_CACHE));
	V_DrawPatchDirect(LEFT_BORDER_X, LEFT_BORDER_Y+128, W_CacheLumpName(DEH_String(LEFT_BORDER_TILE), PU_CACHE));

	// right edge
	V_DrawPatchDirect(RIGHT_BORDER_X, RIGHT_BORDER_Y, W_CacheLumpName(DEH_String(RIGHT_BORDER_TILE), PU_CACHE));
	V_DrawPatchDirect(RIGHT_BORDER_X, RIGHT_BORDER_Y+64, W_CacheLumpName(DEH_String(RIGHT_BORDER_TILE), PU_CACHE));
	V_DrawPatchDirect(RIGHT_BORDER_X, RIGHT_BORDER_Y+128, W_CacheLumpName(DEH_String(RIGHT_BORDER_TILE), PU_CACHE));
}


void Draw_Wallpaper(char* tile)
{
	int x, y, i, k = 0;

	if ( !Marshmallow_DrawWallpaper )
		return;

	x = X_START;
	y = Y_TOCLEAR_STATUSBAR;

	for ( i = 0; i < HORIZONTAL_TILE_LIMIT; i++ )
	{
		V_DrawPatchDirect(x, y, W_CacheLumpName(DEH_String(tile), PU_CACHE));
		x += GRAYPANEL_TILE_WIDTH;
	}

	x = X_START;
	y += Y_TOCLEAR_STATUSBAR;

	for ( i = 0; i < HORIZONTAL_TILE_LIMIT; i++ )
	{
		V_DrawPatchDirect(x, y, W_CacheLumpName(DEH_String(tile), PU_CACHE));
		x += GRAYPANEL_TILE_WIDTH;
	}

	x = X_START;
	y += Y_TOCLEAR_STATUSBAR;

	for ( i = 0; i < HORIZONTAL_TILE_LIMIT; i++ )
	{
		V_DrawPatchDirect(x, y, W_CacheLumpName(DEH_String(tile), PU_CACHE));
		x += GRAYPANEL_TILE_WIDTH;
	}

	Draw_WallpaperBorders();

	// These two lines will fill-in the gap we have between our wallpaper and status bar
	V_DrawHorizLine(0, 334, 640, 0);
	V_DrawHorizLine(0, 335, 640, 0);
}


void Draw_Tiles(int x, int y, char* tile, int num)
{
	int i;

	for ( i=0; i<num; i++ )
	{
		V_DrawPatchDirect(x, y, W_CacheLumpName(DEH_String(tile), PU_CACHE));
	}
}