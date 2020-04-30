////////////////////////////////////////////////////
//
//  PARTICLE.C:  Particles for blood and explosions
//
////////////////////////////////////////////////////


#include "marshmallow.h"

#include "..\doomtype.h"

#define NUM_PARTICLES_DEFAULT			1024
#define NUM_PARTICLES_MAX				2056

#define NUM_PARTICLES_BLOOD_MIN			128
#define NUM_PARTICLES_BLOOD_MAX			512
#define NUM_PARTICLES_SMOKE_MIN			128
#define NUM_PARTICLES_SMOKE_MAX			512


int imix(int a, int b, float s);

boolean brandom()
{
	return (F_Random() < 0.5);
}

int randomSign()
{
	return brandom() ? 1 : -1;
}




#ifdef FOUNDER_1027

void Founders_ParticleCloud1027(mobj_t *actor, mobjtype_t type, int desiredNumParticles,
							float size_scale, float h_scale, float v_scale, float mom_scale,
							const float random_scale_delta)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * size_scale;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - random_scale_delta;
	float scale_random_hi = 1.0 + random_scale_delta;

	particle_t particles[MAX_PARTICLES];
	int n, numParticles;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	A_Explode(actor);               // make normal explosion	 

	numParticles = 0;
	n = (int)sqrt((float)desiredNumParticles);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			mobj_t target = *actor, *mo;
			particle_t *p;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct = (float)i / (float)n;
			float j_pct = (float)j / (float)n;
			float k_pct = (float)numParticles / (float)numParticles;
			
			p = &particles[k];

			// SHAPE: radial blast outwards
#if 1
			dx = fmix(-maxWidth, maxWidth, i_pct);
			dy = fmix(-maxWidth, maxWidth, j_pct);
			dz = fmix(0, maxHeight, k_pct);					// WAS 1st arg = 0
#endif

			// SHAPE: random box
#if 0
			dx = fmix(-maxWidth, maxWidth, F_Random());
			dy = fmix(-maxWidth, maxWidth, F_Random());
			dz = fmix(0, maxHeight, F_Random());
#endif

			dx *= h_scale;
			dy *= h_scale;
			dz *= v_scale;

			// NEW as of midnight
			dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dz *= fmix(scale_random_lo, scale_random_hi, F_Random());


			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;

			p->x = dx;
			p->y = dy;
			p->z = dz;

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnMissile(actor, &target, type);    
			
			mo->momx *= mom_scale;
			mo->momy *= mom_scale;
			mo->momz *= mom_scale;

			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);



			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			// mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;

			numParticles++;
		}
	}

	x = 5;
	x = x;

}

void Founders_MushroomRedux(mobj_t *actor, mobjtype_t type)
{
	int i, j, n = actor->info->damage;
	float scale_h = 1;
	float scale_v = 0.1;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	A_Explode(actor);               // make normal explosion	

	n *= scale_h;
	for (i = -n; i <= n; i += 8)
	{
		// launch mushroom cloud
		for (j = -n; j <= n; j += 8)
		{
			int d, dm, ds;
			mobj_t target = *actor, *mo;

			d = P_AproxDistance(i, j);
			dm = d * misc1;
			ds = (int)(dm * scale_v);


			target.x += i<<FRACBITS + ds;    // Aim in many directions from source
			target.y += j<<FRACBITS + ds;
			target.z += ds;           // Aim fairly high
			mo = P_SpawnMissile(actor, &target, type);    
			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);
			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
      }
	}
}

void Founders_Matryoshka1027(mobj_t *actor, mobjtype_t type, int numLevels)
{
	const int numParticles = 1024;
	const int numParticlesPerLevel = numParticles / numLevels;
	const mobjtype_t types[] = {
		MT_PLASMA, MT_BFG_BLAST_PARTICLE, MT_FATSHOT, MT_BRUISERSHOT, MT_TROOPSHOT,
		MT_TRACER, MT_HEADSHOT, MT_ROCKET };
	const int numTypes = sizeof(types) / sizeof(mobjtype_t);

	int k;

	// Current
	for (k = 0; k <= numLevels; k++)
	{
		float level_pct		= (float)k / (float)numLevels;
		float level_pct_inv = 1.0 - level_pct;

		// Example 1
#if 0
		float size_scale	= 8.0 * fmix(1.0, 1.0 / (float)numLevels, level_pct); // true ? fmix(0.25, numLevels, level_pct) : 1.0;
		float h_scale		= false ? fmix(4.0, 0.0, level_pct) : 1.0;
		float v_scale		= true ? fmix(0.0, 1.0, level_pct) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= false ? fmix(0.01, 4.0, level_pct_inv) : 1.0;
		float random_scale  = true ? fmix(0.1, 0.3, level_pct) : 1.0;
#endif

		// Randomly Selected
#if 0
		float size_scale	= 8.0 * fmix(1.0, 1.0 / (float)numLevels, F_Random());
		float h_scale		= false ? fmix(32.0, 0.0, F_Random()) : 1.0;
		float v_scale		= true ? fmix(0.0, 1.0, F_Random()) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= false ? fmix(0.01, 4.0, F_Random()) : 1.0;
		float random_scale  = true ? fmix(0.1, 0.3, F_Random()) : 1.0;
#endif

				
		// Cool Baseline
#if 0
		float size_scale	= fmix(1.0, 1.0 / (float)numLevels, level_pct); // true ? fmix(0.25, numLevels, level_pct) : 1.0;
		float h_scale		= false ? fmix(32.0, 0.0, level_pct) : 1.0;
		float v_scale		= false ? fmix(0.0, 1.0, level_pct) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= false ? fmix(0.01, 4.0, level_pct_inv) : 1.0;
		float random_scale  = false ? fmix(0.1, 0.3, level_pct) : 0.1;
#endif

		
		// Current Work-in-Progress
#if 1
		float size_scale	= fmix(1.0, 1.0 / (float)numLevels, level_pct); // true ? fmix(0.25, numLevels, level_pct) : 1.0;
		float h_scale		= false ? fmix(32.0, 0.0, level_pct) : 1.0;
		float v_scale		= false ? fmix(0.0, 1.0, level_pct) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= true ? fmix(0.01, 8.0, level_pct) : 1.0;
		float random_scale  = false ? fmix(0.1, 0.3, level_pct) : 0.1;
#endif



		//float mom_scale		= false ? 1.0 : fmix(3.0, 1.0, level_pct);
		// WAS type = types[k % numTypes];
		Founders_ParticleCloud1027(actor, type, numParticlesPerLevel,
			size_scale, h_scale, v_scale, mom_scale, random_scale);
	}

}

void Founders_Mushroom1027(mobj_t *actor, mobjtype_t type)
{
	const int numParticles = 1024;
	const float randomScale = 0.1;

	// Nuclear
//	Founders_ParticleCloud(actor, MT_FATSHOT, numParticles, 32.0, 1.0, 1.0, 3.0, randomScale);

	// Saucer
//	Founders_ParticleCloud(actor, MT_PLASMA, numParticles, 1.0, 1.0, 0.1, 1.0, randomScale);

	// Vertical Blast Column
//	Founders_ParticleCloud(actor, MT_FATSHOT, numParticles, 128, 0.7, 1.3, 4.0, randomScale);

	// Current
	// Founders_Matryoshka(actor, type, 3);
	Founders_Matryoshka1027(actor, type, 3);


#if 0

	int i, j, n = actor->info->damage;
	float scale_h = 1;
	float scale_v = 1;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	A_Explode(actor);               // make normal explosion	 

	n *= scale_h;
	for (i = -n; i <= n; i += 8)
	{
		// launch mushroom cloud
		for (j = -n; j <= n; j += 8)
		{
			int delta_z;

			mobj_t target = *actor, *mo;
			target.x += i << FRACBITS;    // Aim in many directions from source
			target.y += j << FRACBITS;
			delta_z = (int)(P_AproxDistance(i,j) * misc1 * scale_v);
			target.z += delta_z;           // Aim fairly high
			mo = P_SpawnMissile(actor, &target, type);    
			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);
			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
		}

	}
	#endif
}

#endif				// FOUNDER_1027


extern float fmix(float, float, float);

typedef struct particle_s
{
	fixed_t x;
	fixed_t y;
	fixed_t z;
} particle_t;

typedef struct vec2_s
{
	float x;
	float y;
} vec2_t;

typedef struct vec3_s
{
	float x;
	float y;
	float z;
} vec3_t;

#define MAX_PARTICLES		4096


static mobj_t*
P_SpawnGib
        ( mobj_t*	source,
          mobj_t*	dest,
          mobjtype_t	type )
{
    mobj_t*	th;
    angle_t	an;
    int		dist;

    th = P_SpawnMobj (source->x,
                      source->y,
                      source->z + 4*8*FRACUNIT, type);

    if (th->info->seesound)
        S_StartSound (th, th->info->seesound);

    th->target = source;	// where it came from
    an = R_PointToAngle2 (source->x, source->y, dest->x, dest->y);

    th->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
    th->info->speed = 15*FRACUNIT;

    // fuzzy player
    if (dest->flags & MF_SHADOW)
        an += P_SubRandom() << 20;

    th->angle = an;
    an >>= ANGLETOFINESHIFT;
    th->momx = FixedMul (th->info->speed, finecosine[an]);
    th->momy = FixedMul (th->info->speed, finesine[an]);

    dist = P_AproxDistance (dest->x - source->x, dest->y - source->y);
    dist = dist / th->info->speed;

    if (dist < 1)
        dist = 1;

    th->momz = (dest->z - source->z) / dist;
    P_CheckMissileSpawn (th);

    return th;
}


void Founders_ParticleCloud_Advanced_1029(
	mobj_t *actor,
	mobjtype_t type,
	int desiredNumParticles,
	float origin_delta_x,
	float origin_delta_y,
	float origin_delta_z,
	const float size_scale,
	float h_scale,
	float v_scale,
	vec3_t mom_scales,
	const float random_scale_delta,
	boolean enableMissileMode)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * size_scale;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - random_scale_delta;
	float scale_random_hi = 1.0 + random_scale_delta;
	vec3_t origin_offset;

	particle_t particles[MAX_PARTICLES];
	int n, numParticles;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	//A_Explode(actor);               // make normal explosion	 

	numParticles = 0;
	n = (int)sqrt((float)desiredNumParticles);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			mobj_t target = *actor, *mo;
			particle_t *p;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct = (float)i / (float)n;
			float j_pct = (float)j / (float)n;
			float k_pct = (float)numParticles / (float)numParticles;
			
			p = &particles[k];

			// SHAPE: radial blast outwards
#if 1
			dx = fmix(-maxWidth, maxWidth, i_pct);
			dy = fmix(-maxWidth, maxWidth, j_pct);
			dz = fmix(0, maxHeight, k_pct);					// WAS 1st arg = 0
#endif

			// SHAPE: random box
#if 0
			dx = fmix(-maxWidth, maxWidth, F_Random());
			dy = fmix(-maxWidth, maxWidth, F_Random());
			dz = fmix(0, maxHeight, F_Random());
#endif

			dx *= h_scale;
			dy *= h_scale;
			dz *= v_scale;

			dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dz *= fmix(scale_random_lo, scale_random_hi, F_Random());

			// EXP TODO -- constrain Z to mo->floorz, mo->celingz

			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;

			p->x = dx;
			p->y = dy;
			p->z = dz;

			// target = *(viewplayer->mo);							// HACK NEW

			origin_offset.x = origin_delta_x * maxWidth;
			origin_offset.y = origin_delta_y * maxWidth;
			origin_offset.z = origin_delta_z * maxWidth;


			target.x += origin_offset.x + dx;
			target.y += origin_offset.y + dy;
			target.z += origin_offset.z + dz;

			mo = P_SpawnGib(actor, &target, type);
			mo->interp = true;									// NEW TODAY
			
			mo->momx *= mom_scales.x;
			mo->momy *= mom_scales.y;
			mo->momz *= mom_scales.z;

			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);



			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			if (enableMissileMode)
			{
				mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
			}

			numParticles++;
		}
	}

	x = 5;
	x = x;

}


void Founders_ParticleCloud1028(
	mobj_t *actor,
	mobjtype_t type,
	int desiredNumParticles,
	const float size_scale,
	float h_scale,
	float v_scale,
	vec3_t mom_scales,
	const float random_scale_delta,
	boolean enableMissileMode)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * size_scale;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - random_scale_delta;
	float scale_random_hi = 1.0 + random_scale_delta;

	particle_t particles[MAX_PARTICLES];
	int n, numParticles;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	//A_Explode(actor);               // make normal explosion	

	numParticles = 0;
	n = (int)sqrt((float)desiredNumParticles);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			mobj_t target = *actor, *mo;
			particle_t *p;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct = (float)i / (float)n;
			float j_pct = (float)j / (float)n;
			float k_pct = (float)numParticles / (float)numParticles;
			
			p = &particles[k];

			// SHAPE: radial blast outwards
#if 1
			dx = fmix(-maxWidth, maxWidth, i_pct);
			dy = fmix(-maxWidth, maxWidth, j_pct);
			dz = fmix(0, maxHeight, k_pct);					// WAS 1st arg = 0
#endif

			// SHAPE: random box
#if 0
			dx = fmix(-maxWidth, maxWidth, F_Random());
			dy = fmix(-maxWidth, maxWidth, F_Random());
			dz = fmix(0, maxHeight, F_Random());
#endif

			dx *= h_scale;
			dy *= h_scale;
			dz *= v_scale;

			dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dz *= fmix(scale_random_lo, scale_random_hi, F_Random());

			// EXP TODO -- constrain Z to mo->floorz, mo->celingz


			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;

			p->x = dx;
			p->y = dy;
			p->z = dz;

			// target = *(viewplayer->mo);							// HACK NEW

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, type);
			mo->interp = true;									// NEW TODAY
			
			mo->momx *= mom_scales.x;
			mo->momy *= mom_scales.y;
			mo->momz *= mom_scales.z;

			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);



			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			if (enableMissileMode)
			{
				mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
			}

			numParticles++;
		}
	}

	x = 5;
	x = x;

}



void Founders_Matryoshka1028(mobj_t *actor, mobjtype_t type, int numLevels, boolean enableMissleMode)
{
	const int numParticles = 1024;
	const int numParticlesPerLevel = numParticles / numLevels;
	const mobjtype_t types[] = {
		MT_PLASMA,
		MT_FATSHOT,
		MT_BRUISERSHOT,
		MT_TROOPSHOT,
		MT_ARACHPLAZ,
		MT_HEADSHOT };
	const int numTypes = sizeof(types) / sizeof(mobjtype_t);

	int k;
	float mom_scale_h, mom_scale_v;
	vec3_t mom_scales;

	// Current
	for (k = 0; k <= numLevels; k++)
	{
		float level_pct		= (float)k / (float)numLevels;
		float level_pct_inv = 1.0 - level_pct;

		// Example 1
#if 0
		float size_scale	= 8.0 * fmix(1.0, 1.0 / (float)numLevels, level_pct); // true ? fmix(0.25, numLevels, level_pct) : 1.0;
		float h_scale		= false ? fmix(4.0, 0.0, level_pct) : 1.0;
		float v_scale		= true ? fmix(0.0, 1.0, level_pct) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= false ? fmix(0.01, 4.0, level_pct_inv) : 1.0;
		float random_scale  = true ? fmix(0.1, 0.3, level_pct) : 1.0;
#endif

		// Randomly Selected
#if 0
		float size_scale	= 8.0 * fmix(1.0, 1.0 / (float)numLevels, F_Random());
		float h_scale		= false ? fmix(32.0, 0.0, F_Random()) : 1.0;
		float v_scale		= true ? fmix(0.0, 1.0, F_Random()) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= false ? fmix(0.01, 4.0, F_Random()) : 1.0;
		float random_scale  = true ? fmix(0.1, 0.3, F_Random()) : 1.0;
#endif

				
		// Cool Baseline
#if 0
		float size_scale	= fmix(1.0, 1.0 / (float)numLevels, level_pct); // true ? fmix(0.25, numLevels, level_pct) : 1.0;
		float h_scale		= false ? fmix(32.0, 0.0, level_pct) : 1.0;
		float v_scale		= false ? fmix(0.0, 1.0, level_pct) : 1.0;	//			(0.0 gives disc shape)
		float mom_scale		= false ? fmix(0.01, 4.0, level_pct_inv) : 1.0;
		float random_scale  = false ? fmix(0.1, 0.3, level_pct) : 0.1;
#endif

		
		// Current Work-in-Progress
#if 1
		float size_scale	= fmix(1.0, 1.0 / (float)numLevels, level_pct); // true ? fmix(0.25, numLevels, level_pct) : 1.0;
		float h_scale		= false ? fmix(32.0, 0.0, level_pct) : 1.0;
		float v_scale		= false ? fmix(0.0, 1.0, level_pct) : 1.0;	//			(0.0 gives disc shape)
		float random_scale  = false ? fmix(0.1, 0.3, level_pct) : 0.1;
#endif



		//float mom_scale		= false ? 1.0 : fmix(3.0, 1.0, level_pct);
		// WAS type = types[k % numTypes];
		type = types[GetRandomArrayIndex(numTypes)];								// NEW TODAY
		mom_scale_h = 1.0; // fmix(1.0, 1.0, level_pct);
		mom_scale_v = 0.5; // fmix(0.25, 0.25, level_pct);
		mom_scales.x = mom_scale_h;
		mom_scales.y = mom_scale_h;
		mom_scales.z = mom_scale_v;
		Founders_ParticleCloud1028(actor, type, numParticlesPerLevel,
			size_scale, h_scale, v_scale, mom_scales, random_scale, enableMissleMode);
	}

}


void Founders_Mushroom_Duo_1029(
	mobj_t *actor,
	int numParticles,
	mobjtype_t type1,
	mobjtype_t type2,
	boolean enableMissileMode)
{
	const float randomScale = 0.1;
	vec3_t mom_scales;
	
	mom_scales.x = 1.0;
	mom_scales.y = 1.0;
	mom_scales.z = 1.0;
	Founders_ParticleCloud1028(actor, type1, numParticles / 2, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);
	Founders_ParticleCloud1028(actor, type2, numParticles / 2, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);
}


void Founders_Mushroom_GreenBlueBurst_1029(mobj_t *actor, mobjtype_t type, int numParticles, boolean enableMissileMode)
{
	Founders_Mushroom_Duo_1029(actor, numParticles, MT_PLASMA, MT_BRUISERSHOT, enableMissileMode);
}

void Founders_Mushroom_Fractal_Helper_1029(mobj_t *actor, mobjtype_t type,
										   int numParticles, int maxDepth,
										   float origin_x, float origin_y, float origin_z, boolean enableMissileMode)
{
	const float randomScale = 0.1;
	const float c = 0.25;
	const float a = 1.0;
	const float h_scale = 1.0;
	const float v_scale = 1.0;
	const int numParticlesImmediate = 1;			// INTENDED TO BE 1
	const int numChunks = 8;
	const int numParticlesPerChunk = (numParticles - numParticlesImmediate) / numChunks;
	float size_scale;
	vec3_t mom_scales;
	const vec3_t deltas[] = {
		{ -a, -a, -a },
		{ -a, -a,  a },
		{ -a,  a, -a },
		{  a, -a, -a },
		{  a, -a,  a },
		{  a,  a, -a },
		{  a,  a,  a }
	};
	const float numDeltas = sizeof(deltas) / sizeof(deltas[0]);
	int k;

	size_scale = 0.5;
	mom_scales.x = 1.0;
	mom_scales.y = 1.0;
	mom_scales.z = 1.0;

	if (maxDepth < 0)
	{
		return;				// base case
	}


	// Draw Only Me
	Founders_ParticleCloud_Advanced_1029(actor, MT_FATSHOT, numParticlesImmediate, 
		origin_x, origin_y, origin_z,
			size_scale, h_scale, v_scale,  mom_scales, randomScale, enableMissileMode);

	// Recurse
	for (k = 0; k < numDeltas; k++)
	{
		vec3_t delta = deltas[k];
		delta.x *= c;
		delta.y *= c;
		delta.z *= c;
		Founders_Mushroom_Fractal_Helper_1029(actor, type, numParticlesPerChunk, maxDepth - 1,
				origin_x + delta.x,
				origin_y + delta.y,
				0, // origin_z + delta.z,
				enableMissileMode);
	}

}

void Founders_Mushroom_Fractal_1029(mobj_t *actor, mobjtype_t type, int numParticles, boolean enableMissileMode)
{
	int maxDepth = 2;
	Founders_Mushroom_Fractal_Helper_1029(actor, type, numParticles, maxDepth, 0, 0, 0, enableMissileMode);
}


void Founders_Mushroom1028(mobj_t *actor, mobjtype_t type)
{
	const int numParticles = 1024;
	const float randomScale = 0.1;
	boolean enableMissileMode = false;

	vec3_t mom_scales;


	// Generic
	mom_scales.x = 1.0;
	mom_scales.y = 1.0;
	mom_scales.z = 1.0;

#if 0
	mom_scales.x *= 0.01;
	mom_scales.y *= 0.01;
	mom_scales.z *= 0.01;
#endif

	// Generic: (good)
//	Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);

	// Nuclear
	// FIXME to have mom_scales * 3.0
	// Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 32.0, 1.0, 1.0, mom_scales, randomScale, enableMissileMode);

	// Saucer
//	Founders_ParticleCloud1028(actor, MT_PLASMA, numParticles, 0.1, 1.0, 0.1, mom_scales, randomScale, enableMissileMode);

	// Vertical Blast Column
	//mom_scales.x = 2.0;
	//mom_scales.y = 2.0;
	//mom_scales.z = 2.0;
	// these were all originally intended to be 4.0
//	Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 128, 0.7, 1.3, mom_scales, randomScale, enableMissileMode);

	// Matryoshka
//	Founders_Matryoshka1028(actor, type, 3, enableMissileMode);
//	Founders_Matryoshka1028(actor, type, imix(3, 9, F_Random()), enableMissileMode);

	// Current work in progress:
	// MT_TROOPSHOT = vibrant red
	// MT_HEADSHOT = purple/red
	// MT_ROCKET works, but is not directly colorful as an effect
	// MT_SPAWNSHOT = don't use
//	Founders_Mushroom_Duo_1029(actor, numParticles, MT_TROOPSHOT, MT_HEADSHOT, enableMissileMode);

//	Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);

	// Good...
 //	Founders_Mushroom_GreenBlueBurst_1029(actor, type, numParticles, enableMissileMode);

	// CURRENT WORK IN PROGRESS; change numParticlesImmediate = 64 to see effect, but intended is = 1
//	Founders_Mushroom_Fractal_1029(actor, MT_TROOPSHOT, numParticles, enableMissileMode);

//	Founders_Mushroom_GreenBlueBurst_1029(actor, type, numParticles, enableMissileMode);

	Founders_ParticleCloud1028(actor, type, numParticles, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);

#if 0

	int i, j, n = actor->info->damage;
	float scale_h = 1;
	float scale_v = 1;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	A_Explode(actor);               // make normal explosion	

	n *= scale_h;
	for (i = -n; i <= n; i += 8)
	{
		// launch mushroom cloud
		for (j = -n; j <= n; j += 8)
		{
			int delta_z;

			mobj_t target = *actor, *mo;
			target.x += i << FRACBITS;    // Aim in many directions from source
			target.y += j << FRACBITS;
			delta_z = (int)(P_AproxDistance(i,j) * misc1 * scale_v);
			target.z += delta_z;           // Aim fairly high
			mo = P_SpawnMissile(actor, &target, type);    
			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);
			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
		}

	}
	#endif
}
typedef enum
{
	PARTICLESYSTEM_STYLE_BOX_UNIFORM,
	PARTICLESYSTEM_STYLE_SPHERE,
	PARTICLESYSTEM_STYLE_BOX_RANDOM,
	PARTICLESYSTEM_STYLE_SPHERE_RANDOM
} particlessystem_style_t;

typedef struct
{
		int								num_particles;
		float							scale_size;
		float							scale_h;
		float							scale_v;
		float							scale_random;
		vec3_t							scale_mom;	
		mobjtype_t						type;
		particlessystem_style_t			style;
		boolean							is_missile;
		boolean							is_random_offset;
		boolean							is_random_dir;
} particlesystem_args_t;

typedef struct
{
		int								num_particles;
		float							scale_size_lo;
		float							scale_size_hi;
		float							scale_h;
		float							scale_v;
		float							scale_random;
		float							scale_mom_x_lo;
		float							scale_mom_x_hi;
		float							scale_mom_y_lo;
		float							scale_mom_y_hi;
		float							scale_mom_z_lo;
		float							scale_mom_z_hi;
		mobjtype_t						type;
		particlessystem_style_t			style;
		boolean							is_missile;
		boolean							is_random_offset;
		boolean							is_random_dir;
} particlesystem_range_args_t;




void initParticleSystemArgs(particlesystem_args_t *args)
{
	args->num_particles			= NUM_PARTICLES_DEFAULT;
	args->scale_size			= 1.0;
	args->scale_h				= 1.0;
	args->scale_v				= 1.0;
	args->scale_random			= 0.1;
	args->scale_mom.x			= 1.0;
	args->scale_mom.y			= 1.0;
	args->scale_mom.z			= 1.0;
	args->type					= MT_FATSHOT;
	args->style					= PARTICLESYSTEM_STYLE_BOX_UNIFORM;
	args->is_missile			= false;
	args->is_random_offset		= true;
	args->is_random_dir			= true;
}


void initParticleSystemRangeArgs(particlesystem_range_args_t *args)
{
	args->num_particles			= NUM_PARTICLES_DEFAULT;
	args->scale_size_lo			= 0.1;
	args->scale_size_lo			= 1.0;
	args->scale_h				= 1.0;
	args->scale_v				= 1.0;
	args->scale_random			= 0.1;
	args->scale_mom_x_lo		= 1.0;
	args->scale_mom_x_hi		= 1.0;
	args->scale_mom_y_lo		= 1.0;
	args->scale_mom_y_hi		= 1.0;
	args->scale_mom_z_lo		= 1.0;
	args->scale_mom_z_hi		= 1.0;
	args->type					= MT_FATSHOT;
	args->style					= PARTICLESYSTEM_STYLE_BOX_UNIFORM;
	args->is_missile			= false;
	args->is_random_offset		= true;
	args->is_random_dir			= true;
}


// address misc1 and misc2
void Founders_ParticleCloud1031(
	mobj_t *actor, particlesystem_args_t *args)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * args->scale_size;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - args->scale_random;					// TODO -- build safety here to allow/intend [0,1] input
	float scale_random_hi = 1.0 + args->scale_random;
	int theta, theta_delta;
	int pitch, pitch_delta;

	int n, numParticles;
	int maxInt = INT_MAX;

	// Mushroom parameters are part of code pointer's state
	// WAS fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	// WAS fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;
	// WAS A_Explode(actor);               // make normal explosion	 

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);

	for (i = 0; i <= n; i++)							// WAS i < n
	{
		for (j = 0; j <= n; j++)						// WAS j < n
		{
			mobj_t target = *actor, *mo;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct		= (float)i / (float)n;
			float j_pct		= (float)j / (float)n;
			float k_pct		= (float)numParticles / (float)args->num_particles;			// WAS
			int yaw			= i_pct * INT_MAX;
			int pitch		= j_pct * INT_MAX;
			int yaw_cos		= finecosine[yaw >> ANGLETOFINESHIFT];
			int yaw_sin		= finesine[yaw >> ANGLETOFINESHIFT];
			int pitch_cos	= finecosine[pitch >> ANGLETOFINESHIFT];
			int pitch_sin	= finesine[pitch >> ANGLETOFINESHIFT];
			int radius		= maxWidth;

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:						// box, even distribution
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = maxHeight;
				// EXP dz = fmix(-maxHeight, maxHeight, j_pct);							// NEW -- was = max height
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:						// box, randomly distributed
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(-maxHeight, maxHeight, F_Random());						// NEW -- was [0,maxHeight]
				break;
			case PARTICLESYSTEM_STYLE_SPHERE:						// spherical
				dx = FixedMul(radius, yaw_cos);
				dy = FixedMul(radius, yaw_sin);
				dz = FixedMul(radius, pitch_sin);
				break;
			case PARTICLESYSTEM_STYLE_SPHERE_RANDOM:						// spherical random		// NEW 1101
				{
					int r = radius * fmix(0.1, 3.0, F_Random());
					int theta = (int)fmix(0, INT_MAX, F_Random());
					int azimu = (int)fmix(0, INT_MAX, F_Random());
					int theta_cos	= finecosine[theta >> ANGLETOFINESHIFT];
					int theta_sin	= finesine[theta >> ANGLETOFINESHIFT];
					int azimu_cos	= finecosine[azimu >> ANGLETOFINESHIFT];
					int azimu_sin	= finesine[azimu >> ANGLETOFINESHIFT];
					dx = FixedMul(r, theta_cos);
					dy = FixedMul(r, theta_sin);
					dz = FixedMul(r, azimu_sin);
					break;
				}
			}


			dx *= args->scale_h;
			dy *= args->scale_h;
			dz *= args->scale_v;

			if (args->is_random_offset)
			{
				dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dz *= fmix(scale_random_lo, scale_random_hi, F_Random());
			}

			// EXP TODO -- constrain Z to mo->floorz, mo->celingz

#if 0											// Great...
			dx *= randomSign();					// NEW....
			dy *= randomSign();
			dz *= randomSign();
#endif
#if 0
			dz *= ((j_pct < 0.5) ? 1 : -1);
#endif


			dx << FRACBITS;						// like theta += theta_delta
			dy << FRACBITS;						// like pitch += pitch_delta
			// dz = 0;
			dz << FRACBITS;						// makes it way bigger

			// dz = P_AproxDistance(i, j);		// saucer
			// target = *(viewplayer->mo);							// HACK NEW


			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			// P_SpawnBlood(actor->x, actor->y, actor->z, 100, &target);
			// mo = P_SpawnMobj(actor->x, actor->y, actor->z, MT_REDSKULLKEY);
			
			mo->interp = true;			
			mo->momx *= args->scale_mom.x;
			mo->momy *= args->scale_mom.y;
			mo->momz *= args->scale_mom.z;

			if (args->is_random_dir)
			{
				mo->momx *= randomSign();
				mo->momy *= randomSign();
				mo->momz *= randomSign();
			}

			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			if (args->is_missile)
			{
				mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
			}


			numParticles++;
		}
	}
}




// address misc1 and misc2
void Founders_ParticleCloud0621(
	mobj_t *actor, particlesystem_args_t *args)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * args->scale_size;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - args->scale_random;					// TODO -- build safety here to allow/intend [0,1] input
	float scale_random_hi = 1.0 + args->scale_random;
	int theta, theta_delta;
	int pitch, pitch_delta;

	int n, numParticles;
	int maxInt = INT_MAX;

	// Mushroom parameters are part of code pointer's state
	// WAS fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	// WAS fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;
	// WAS A_Explode(actor);               // make normal explosion	

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);

	for (i = 0; i <= n; i++)							// WAS i < n
	{
		for (j = 0; j <= n; j++)						// WAS j < n
		{
			mobj_t target = *actor, *mo;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct		= (float)i / (float)n;
			float j_pct		= (float)j / (float)n;
			float k_pct		= (float)numParticles / (float)args->num_particles;			// WAS
			int yaw			= i_pct * INT_MAX;
			int pitch		= j_pct * INT_MAX;
			int yaw_cos		= finecosine[yaw >> ANGLETOFINESHIFT];
			int yaw_sin		= finesine[yaw >> ANGLETOFINESHIFT];
			int pitch_cos	= finecosine[pitch >> ANGLETOFINESHIFT];
			int pitch_sin	= finesine[pitch >> ANGLETOFINESHIFT];
			int radius		= maxWidth;

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:						// box, even distribution
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = maxHeight;
				// EXP dz = fmix(-maxHeight, maxHeight, j_pct);							// NEW -- was = max height
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:						// box, randomly distributed
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(-maxHeight, maxHeight, F_Random());						// NEW -- was [0,maxHeight]
				break;
			case PARTICLESYSTEM_STYLE_SPHERE:						// spherical
				dx = FixedMul(radius, yaw_cos);
				dy = FixedMul(radius, yaw_sin);
				dz = FixedMul(radius, pitch_sin);
				break;
			case PARTICLESYSTEM_STYLE_SPHERE_RANDOM:						// spherical random		// NEW 1101
				{
					int r = radius * fmix(0.1, 3.0, F_Random());
					int theta = (int)fmix(0, INT_MAX, F_Random());
					int azimu = (int)fmix(0, INT_MAX, F_Random());
					int theta_cos	= finecosine[theta >> ANGLETOFINESHIFT];
					int theta_sin	= finesine[theta >> ANGLETOFINESHIFT];
					int azimu_cos	= finecosine[azimu >> ANGLETOFINESHIFT];
					int azimu_sin	= finesine[azimu >> ANGLETOFINESHIFT];
					dx = FixedMul(r, theta_cos);
					dy = FixedMul(r, theta_sin);
					dz = FixedMul(r, azimu_sin);
					break;
				}
			}


			dx *= args->scale_h;
			dy *= args->scale_h;
			dz *= args->scale_v;

			if (args->is_random_offset)
			{
				dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dz *= fmix(scale_random_lo, scale_random_hi, F_Random());
			}

			// EXP TODO -- constrain Z to mo->floorz, mo->celingz

#if 0											// Great...
			dx *= randomSign();					// NEW....
			dy *= randomSign();
			dz *= randomSign();
#endif
#if 0
			dz *= ((j_pct < 0.5) ? 1 : -1);
#endif


			dx << FRACBITS;						// like theta += theta_delta
			dy << FRACBITS;						// like pitch += pitch_delta
			// dz = 0;
			dz << FRACBITS;						// makes it way bigger

			// dz = P_AproxDistance(i, j);		// saucer
			// target = *(viewplayer->mo);							// HACK NEW


			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			// P_SpawnBlood(actor->x, actor->y, actor->z, 100, &target);
			// mo = P_SpawnMobj(actor->x, actor->y, actor->z, MT_REDSKULLKEY);
			
			mo->interp = true;			
			mo->momx *= args->scale_mom.x;
			mo->momy *= args->scale_mom.y;
			mo->momz *= args->scale_mom.z;

			if (args->is_random_dir)
			{
				mo->momx *= randomSign();
				mo->momy *= randomSign();
				mo->momz *= randomSign();
			}

			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			if (args->is_missile)
			{
				mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
			}

			//mo->flags |= (-1)<<MF_TRANSSHIFT;   // [marshmallow]  trying to change color
			//mo->flipsprite = Crispy_Random() & 1;
			//SHOW_MESSAGE "here";

			numParticles++;
		}
	}
}


// address misc1 and misc2
void Founders_ParticleCloud0621_NU(
	mobj_t *actor, particlesystem_range_args_t *args)
{
	int i, j, k;
	int x;
	
	const int maxWidthBaseline = 12000000 / 2;
	float scale_random_lo = 1.0 - args->scale_random;					// TODO -- build safety here to allow/intend [0,1] input
	float scale_random_hi = 1.0 + args->scale_random;
	int theta, theta_delta;
	int pitch, pitch_delta;

	int n, numParticles;
	int maxInt = INT_MAX;

	// Mushroom parameters are part of code pointer's state
	// WAS fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	// WAS fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;
	// WAS A_Explode(actor);               // make normal explosion	 

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);

	for (i = 0; i <= n; i++)							// WAS i < n
	{
		for (j = 0; j <= n; j++)						// WAS j < n
		{
			mobj_t target = *actor, *mo;
			vec3_t scale_mom;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct		= (float)i / (float)n;
			float j_pct		= (float)j / (float)n;
			float k_pct		= (float)numParticles / (float)args->num_particles;			// WAS
			int yaw			= i_pct * INT_MAX;
			int pitch		= j_pct * INT_MAX;
			int yaw_cos		= finecosine[yaw >> ANGLETOFINESHIFT];
			int yaw_sin		= finesine[yaw >> ANGLETOFINESHIFT];
			int pitch_cos	= finecosine[pitch >> ANGLETOFINESHIFT];
			int pitch_sin	= finesine[pitch >> ANGLETOFINESHIFT];
			float scale_size = GetRandomFloatInRange(args->scale_size_lo, args->scale_size_hi);			// per particle dynamic size
			float maxWidth	= (float)maxWidthBaseline * scale_size;
			float maxHeight = maxWidth;
			int radius		= maxWidth;

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:						// box, even distribution
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = maxHeight;
				// EXP dz = fmix(-maxHeight, maxHeight, j_pct);							// NEW -- was = max height
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:						// box, randomly distributed
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(-maxHeight, maxHeight, F_Random());						// NEW -- was [0,maxHeight]
				break;
			case PARTICLESYSTEM_STYLE_SPHERE:						// spherical
				dx = FixedMul(radius, yaw_cos);
				dy = FixedMul(radius, yaw_sin);
				dz = FixedMul(radius, pitch_sin);
				break;
			case PARTICLESYSTEM_STYLE_SPHERE_RANDOM:						// spherical random		// NEW 1101
				{
					int r = radius * fmix(0.1, 3.0, F_Random());
					int theta = (int)fmix(0, INT_MAX, F_Random());
					int azimu = (int)fmix(0, INT_MAX, F_Random());
					int theta_cos	= finecosine[theta >> ANGLETOFINESHIFT];
					int theta_sin	= finesine[theta >> ANGLETOFINESHIFT];
					int azimu_cos	= finecosine[azimu >> ANGLETOFINESHIFT];
					int azimu_sin	= finesine[azimu >> ANGLETOFINESHIFT];
					dx = FixedMul(r, theta_cos);
					dy = FixedMul(r, theta_sin);
					dz = FixedMul(r, azimu_sin);
					break;
				}
			}


			dx *= args->scale_h;
			dy *= args->scale_h;
			dz *= args->scale_v;

			if (args->is_random_offset)
			{
				dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dz *= fmix(scale_random_lo, scale_random_hi, F_Random());
			}

			// EXP TODO -- constrain Z to mo->floorz, mo->celingz

#if 0											// Great...
			dx *= randomSign();					// NEW....
			dy *= randomSign();
			dz *= randomSign();
#endif
#if 0
			dz *= ((j_pct < 0.5) ? 1 : -1);
#endif


			dx << FRACBITS;						// like theta += theta_delta
			dy << FRACBITS;						// like pitch += pitch_delta
			// dz = 0;
			dz << FRACBITS;						// makes it way bigger

			// dz = P_AproxDistance(i, j);		// saucer
			// target = *(viewplayer->mo);							// HACK NEW


			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			// P_SpawnBlood(actor->x, actor->y, actor->z, 100, &target);
			// mo = P_SpawnMobj(actor->x, actor->y, actor->z, MT_REDSKULLKEY);
			
			mo->interp = true;

			scale_mom.x = GetRandomFloatInRange(args->scale_mom_x_lo, args->scale_mom_x_hi);
			scale_mom.y = GetRandomFloatInRange(args->scale_mom_y_lo, args->scale_mom_y_hi);
			scale_mom.z = GetRandomFloatInRange(args->scale_mom_z_lo, args->scale_mom_z_hi);

			mo->momx *= scale_mom.x;
			mo->momy *= scale_mom.y;
			mo->momz *= scale_mom.z;

			if (args->is_random_dir)
			{
				mo->momx *= randomSign();
				mo->momy *= randomSign();
				mo->momz *= randomSign();
			}

			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			if (args->is_missile)
			{
				mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
			}

			//mo->flipsprite = Crispy_Random() & 1;   // to add more variation to its appearance, randomly flip every particle

			numParticles++;
		}
	}
}




// address misc1 and misc2
void Founders_ParticleCloud1030(
	mobj_t *actor, particlesystem_args_t *args)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * args->scale_size;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - args->scale_random;					// TODO -- build safety here to allow/intend [0,1] input
	float scale_random_hi = 1.0 + args->scale_random;

	particle_t particles[MAX_PARTICLES];
	int n, numParticles;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

	//A_Explode(actor);               // make normal explosion	

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			mobj_t target = *actor, *mo;
			particle_t *p;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct = (float)i / (float)n;
			float j_pct = (float)j / (float)n;
			float k_pct = 1.0;						// original was probably a bug
			// float k_pct = (float)numParticles / (float)args->num_particles;			// WAS
			
			p = &particles[k];


			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:
				// SHAPE: radial blast outwards
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = fmix(0, maxHeight, k_pct);					// WAS 1st arg = 0
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(0, maxHeight, F_Random());
				break;
			}

			
			dx *= args->scale_h;
			dy *= args->scale_h;
			dz *= args->scale_v;

			if (args->is_random_offset)
			{
				dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
				dz *= fmix(scale_random_lo, scale_random_hi, F_Random());
			}

			// EXP TODO -- constrain Z to mo->floorz, mo->celingz


			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;						// makes it way bigger
			// dz = 0;							// saucer
// dz = P_AproxDistance(i, j);		// saucer

			p->x = dx;
			p->y = dy;
			p->z = dz;

			// target = *(viewplayer->mo);							// HACK NEW

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			mo->interp = true;									// NEW TODAY
			
			mo->momx *= args->scale_mom.x;
			mo->momy *= args->scale_mom.y;
			mo->momz *= args->scale_mom.z;

#if 0															// WAS - ORIGINAL A_Mushroom()

			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);

#endif




			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
			if (args->is_missile)
			{
				mo->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
			}

			numParticles++;
		}
	}

	x = 5;
	x = x;

}


//=============================================================================
//=============================================================================
void Founders_ParticleCloudMulti1030(mobj_t *actor, particlesystem_args_t *args,  mobjtype_t types[], int num_types)
{
	particlesystem_args_t local;
	int k;

	local = *args;
	local.num_particles /= num_types;
	for (k = 0; k < num_types; k++)
	{
		local.type = types[k];
		Founders_ParticleCloud1030(actor, &local);
	}
}

//=============================================================================
//=============================================================================
void Founders_ParticleCloudMulti1031(mobj_t *actor, particlesystem_args_t *args,  mobjtype_t types[], int num_types)
{
	particlesystem_args_t local;
	int k;

	local = *args;
	local.num_particles /= num_types;
	for (k = 0; k < num_types; k++)
	{
		local.type = types[k];
		Founders_ParticleCloud1031(actor, &local);
	}
}

//=============================================================================
//=============================================================================
void Founders_ParticleCloudDuo1030(mobj_t *actor, particlesystem_args_t *args,  mobjtype_t type1, mobjtype_t type2)
{
	mobjtype_t types[2] = { type1, type2 };
	Founders_ParticleCloudMulti1030(actor, args, types, 2);

#if 0
	local = *args;
	local.num_particles /= 2;
	local.type = type1;
	Founders_ParticleCloud1030(actor, &local);
	local.type = type2;
	Founders_ParticleCloud1030(actor, &local);
#endif
}

//=============================================================================
//=============================================================================
void Founders_ParticleCloudDuo1031(mobj_t *actor, particlesystem_args_t *args,  mobjtype_t type1, mobjtype_t type2)
{
	mobjtype_t types[2] = { type1, type2 };
	Founders_ParticleCloudMulti1031(actor, args, types, 2);
}



void Founders_Mushroom1030(mobj_t *actor, mobjtype_t type)
{
	particlesystem_args_t args;
	
	initParticleSystemArgs(&args);
	Founders_ParticleCloud1030(actor, &args);


	// Generic: (good)
//	Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);

	// Nuclear
	// FIXME to have mom_scales * 3.0
	// Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 32.0, 1.0, 1.0, mom_scales, randomScale, enableMissileMode);

	// Saucer
//	Founders_ParticleCloud1028(actor, MT_PLASMA, numParticles, 0.1, 1.0, 0.1, mom_scales, randomScale, enableMissileMode);

	// Vertical Blast Column
	//mom_scales.x = 2.0;
	//mom_scales.y = 2.0;
	//mom_scales.z = 2.0;
	// these were all originally intended to be 4.0
//	Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 128, 0.7, 1.3, mom_scales, randomScale, enableMissileMode);

	// Matryoshka
//	Founders_Matryoshka1028(actor, type, 3, enableMissileMode);
//	Founders_Matryoshka1028(actor, type, imix(3, 9, F_Random()), enableMissileMode);

	// Current work in progress:
	// MT_TROOPSHOT = vibrant red
	// MT_HEADSHOT = purple/red
	// MT_ROCKET works, but is not directly colorful as an effect
	// MT_SPAWNSHOT = don't use
//	Founders_Mushroom_Duo_1029(actor, numParticles, MT_TROOPSHOT, MT_HEADSHOT, enableMissileMode);

//	Founders_ParticleCloud1028(actor, MT_FATSHOT, numParticles, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);

	// Good...
 //	Founders_Mushroom_GreenBlueBurst_1029(actor, type, numParticles, enableMissileMode);

	// CURRENT WORK IN PROGRESS; change numParticlesImmediate = 64 to see effect, but intended is = 1
//	Founders_Mushroom_Fractal_1029(actor, MT_TROOPSHOT, numParticles, enableMissileMode);

//	Founders_Mushroom_GreenBlueBurst_1029(actor, type, numParticles, enableMissileMode);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Baseline(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_GreenBlueBurst(mobj_t *actor)
{
	mobjtype_t types[] = { MT_PLASMA, MT_BRUISERSHOT };
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles = 1024;
	Founders_ParticleCloudDuo1030(actor, &args, MT_PLASMA, MT_BRUISERSHOT);
//	Founders_ParticleCloudMulti1030(actor, &args, types, sizeof(types) / sizeof(types[0]));
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Nuclear(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles	= MAX_PARTICLES;
	args.type			= MT_TROOPSHOT;
	args.scale_mom.x	= 3.0;
	args.scale_mom.y	= 3.0;
	args.scale_mom.z	= 1.5;
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_VerticalBlastColumn(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.type			= MT_FATSHOT;
	args.scale_size		= 128;
	args.scale_h		= 0.7;
	args.scale_v		= 1.3;
	args.scale_mom.x	= 2.0;
	args.scale_mom.y	= 2.0;
	args.scale_mom.z	= 2.0;				// all originally intended to be 4.0
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_HorizontalBlast(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.type			= MT_FATSHOT;
	args.scale_size		= 128;
	args.scale_h		= 3.0;
	args.scale_v		= 1.0;
	args.scale_mom.x	= 2.0;
	args.scale_mom.y	= 2.0;
	args.scale_mom.z	= 2.0;				// all originally intended to be 4.0
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Blood(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	// WAS args.num_particles	= GetRandomIntegerInRange(NUM_PARTICLES_BLOOD_MIN, NUM_PARTICLES_BLOOD_MAX);
	args.num_particles	= NUM_PARTICLES_BLOOD_MAX;
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= 1.0; // 0.1;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 1.0; // 2.0; // 0.5;
	args.scale_mom.y	*= 1.0; // 2.0; // 0.5;
	args.scale_mom.z	*= 1.0; // 2.0; // 0.5;
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_Column(mobj_t *actor)
{
	particlesystem_args_t args;

	// THIS REQUIRES THAT mo's speed = 2*FRACUNIT, mass = 1
	initParticleSystemArgs(&args);
	args.num_particles		= GetRandomIntegerInRange(NUM_PARTICLES_SMOKE_MIN, NUM_PARTICLES_SMOKE_MAX);
	args.type				= MT_PARTICLE_SMOKE;
	args.is_missile			= true;
	Founders_ParticleCloud1030(actor, &args);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_VerticalBlastColumn(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.type			= MT_PARTICLE_SMOKE;
	args.scale_size		= 128;
	args.scale_h		= 0.7;
	args.scale_v		= 1.3;
	args.scale_mom.x	= 2.0;
	args.scale_mom.y	= 2.0;
	args.scale_mom.z	= 2.0;				// all originally intended to be 4.0
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Saucer(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles	= 1024;
	args.type			= MT_PLASMA;
	args.scale_size		= 0.5;
	args.scale_mom.x	= 1.0;
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= 0.1;
	Founders_ParticleCloud1030(actor, &args);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_Fog_WIP(mobj_t *actor)
{
	particlesystem_args_t args;
	
	initParticleSystemArgs(&args);
	args.num_particles	= 1024;
	args.type			= MT_FATSHOT;
	args.scale_size		= 1;
	args.scale_mom.x	= 0.5;
	args.scale_mom.y	= 0.5;
	args.scale_mom.z	= 0.5;
	args.is_missile		= true;
	Founders_ParticleCloud1030(actor, &args);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_WIP01(mobj_t *actor)
{
	particlesystem_args_t args;

	mobjtype_t types[] = { MT_FATSHOT, MT_TROOPSHOT, MT_PARTICLE_SMOKE };
	const int num_types = sizeof(types) / sizeof(mobjtype_t);
	initParticleSystemArgs(&args);
	args.num_particles		= 1024;
	args.is_missile			= true;
	args.scale_mom.x		= 1.0;
	args.scale_mom.y		= 1.0;
	args.scale_mom.z		= 1.0;
	Founders_ParticleCloudMulti1030(actor, &args, types, num_types);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Baseline1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.is_random_offset = false;				//  NEW
	args.type = MT_PARTICLE_FATSHOT;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_GreenBlueBurst1031(mobj_t *actor)
{
	mobjtype_t types[] = { MT_PLASMA, MT_BRUISERSHOT };
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;
	args.num_particles		= 1024;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PLASMA, MT_BRUISERSHOT);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Nuclear1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.is_missile		= true;
	args.num_particles	= MAX_PARTICLES;
	args.type			= MT_FATSHOT;
	args.scale_size		= 1; // 128;
	args.scale_mom.x	= 0.8;		// was 3.0
	args.scale_mom.y	= 0.8;
	args.scale_mom.z	= 0.1; // 0.2;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_VerticalBlastColumn1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.type			= MT_FATSHOT;
	args.scale_size		= 64;
	args.scale_h		= 0.7;
	args.scale_v		= 1.3;
	args.scale_mom.x	= 2.0;
	args.scale_mom.y	= 2.0;
	args.scale_mom.z	= 2.0;				// all originally intended to be 4.0
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Pillar1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles	= MAX_PARTICLES;
	args.type			= MT_TROOPSHOT;
	args.scale_size		= 128;
	args.scale_v		= 8.0;
	args.scale_mom.x	= 1.0;		// was 3.0
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= -3.0;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Wall1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.is_missile		= true;
	args.num_particles	= MAX_PARTICLES;
	args.type			= MT_FATSHOT;
	args.scale_size		= 128;
	args.scale_mom.x	= 0.2;		// was 3.0
	args.scale_mom.y	= 0.2;
	args.scale_mom.z	= 0; // 0.2;
	Founders_ParticleCloud1031(actor, &args);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_Foggi1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.is_missile		= true;
	args.num_particles	= 1024;
	args.type			= MT_SMOKE;
	args.scale_size		= 128;
	args.scale_mom.x	= 0.1;		// was 3.0
	args.scale_mom.y	= 0.1;
	args.scale_mom.z	= 0.1; // 0.2;
	Founders_ParticleCloud1031(actor, &args);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Blood1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	// WAS args.num_particles	= GetRandomIntegerInRange(NUM_PARTICLES_BLOOD_MIN, NUM_PARTICLES_BLOOD_MAX);
	args.num_particles	= NUM_PARTICLES_BLOOD_MAX;
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= 1; // 1.0; // 0.1;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 1.0; // 2.0; // 0.5;
	args.scale_mom.y	*= 1.0; // 2.0; // 0.5;
	args.scale_mom.z	*= 1.0; // 2.0; // 0.5;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_Column1031(mobj_t *actor)
{
	particlesystem_args_t args;

	// THIS REQUIRES THAT mo's speed = 2*FRACUNIT, mass = 1 (info.c)
	initParticleSystemArgs(&args);
	args.num_particles		= GetRandomIntegerInRange(8, 128);
	args.type				= MT_PARTICLE_SMOKE;
	args.scale_size			= 5.0;
	args.scale_mom.z		*= 5.0;
	args.style				= PARTICLESYSTEM_STYLE_BOX_RANDOM; // NEW July
	args.is_missile			= true;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_1101(mobj_t *actor)
{
	particlesystem_args_t args;

	// THIS REQUIRES THAT mo's speed = 2*FRACUNIT, mass = 1 (info.c)
	initParticleSystemArgs(&args);
	// args.num_particles		= GetRandomIntegerInRange(NUM_PARTICLES_SMOKE_MIN, NUM_PARTICLES_SMOKE_MAX);
	args.num_particles		= 512;
	args.type				= MT_PARTICLE_SMOKE;
	args.is_missile			= true;
	args.scale_size		= 128;
	args.scale_mom.x	= 0.2;		// was 3.0
	args.scale_mom.y	= 0.2;
	args.scale_mom.z	= 0; // 0.2;
	Founders_ParticleCloud1031(actor, &args);
}



//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Radial1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.style			= PARTICLESYSTEM_STYLE_SPHERE;
	args.is_missile		= true;
	args.num_particles	= 1024;
	args.type			= MT_FATSHOT;
	args.scale_size		= 1;
	args.scale_mom.x	= 1.0; // WAS 0.5;		// was 3.0
	args.scale_mom.y	= 1.0; // WAS 0.5;
	args.scale_mom.z	= 1.0; // WAS 0.5; // 0.2; // 0.2;
	args.is_random_dir	= true;									// BUG? INTENDED --> false
	Founders_ParticleCloud1031(actor, &args);
}


//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Radial1101(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.style			= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.is_missile		= true;
	args.num_particles	= 1024;
	args.type			= MT_FATSHOT;
//	args.type			= MT_REDSKULLKEY;
	args.scale_size		= 1;
	args.scale_mom.x	= 1.0; // 0.25; // WAS 0.5;		// was 3.0
	args.scale_mom.y	= 1.0; // 0.25; // WAS 0.5;
	args.scale_mom.z	= 1.0; // 0.25; // WAS 0.5; // 0.2; // 0.2;
	args.is_random_dir	= true;									// INTENDED --> false
	args.is_random_offset = false;								// NEW
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Radial1210(mobj_t *actor)
{
	particlesystem_args_t args;
	const float c = 1.0;

	initParticleSystemArgs(&args);
	args.style			= PARTICLESYSTEM_STYLE_SPHERE;		// WAS 1210 +_RANDOM
	// !!!BUG +_RANDOM looks broken
	args.is_missile		= true;
	args.num_particles	= 1024;
	args.type			= MT_FATSHOT;
//	args.type			= MT_REDSKULLKEY;
	args.scale_size		= 1;
	args.scale_mom.x	= c; // 0.25; // WAS 0.5;		// was 3.0
	args.scale_mom.y	= c; // 0.25; // WAS 0.5;
	args.scale_mom.z	= c; // 0.25; // WAS 0.5; // 0.2; // 0.2;
	args.is_random_dir	= true;									// INTENDED --> false
	args.is_random_offset = false;								// NEW
	Founders_ParticleCloud1031(actor, &args);
}



//=============================================================================
//=============================================================================
void Founders_ParticleSystem_GreenBlueBurst1101(mobj_t *actor)
{
	mobjtype_t types[] = { MT_PARTICLE_BLUEPLASMA, MT_PARTICLE_BRUISERSHOT };
	particlesystem_args_t args;

	const float mom_scale = 1.0;		// was 0.2
	initParticleSystemArgs(&args);
	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;					// Original
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE;
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.num_particles		= 1024;
	args.scale_size			= 1;
	args.is_missile			= false;											// Original
	// args.is_random_dir		= false;
	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLUEPLASMA, MT_PARTICLE_BRUISERSHOT);
}


//=============================================================================
//=============================================================================
void BloodBurst(mobj_t *actor)  // based on above function, just with blood
{
	mobjtype_t types[] = { MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD };
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3); 
	int particles =  GetRandomIntegerInRange(32,NUM_PARTICLES_BLOOD_MAX);
	float scale = GetRandomFloatInRange(0.8, 2.0);

	const float mom_scale = scale;

	//const float mom_scale = 1.0;		// was 0.2
	initParticleSystemArgs(&args);
	args.style = style;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_RANDOM;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;					// Original
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE;
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.num_particles		= particles;
	args.scale_size			=  1.0;
	args.is_missile			= true;											// Original
	// args.is_random_dir		= false;
	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD);
}


//=============================================================================
//=============================================================================
void CorpseGib(mobj_t *actor) 
{
	//mobjtype_t types[] = { MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD };
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3);
	int particles = NUM_PARTICLES_BLOOD_MAX; // GetRandomIntegerInRange(128,NUM_PARTICLES_BLOOD_MAX);
	float scale = GetRandomFloatInRange(0.8, 2.0); 

	const float mom_scale = scale;

	//const float mom_scale = 1.0;		// was 0.2
	initParticleSystemArgs(&args);
	args.style = style;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_RANDOM;
// args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;					// Original
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE;
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.num_particles		= particles;
	args.scale_size			=  1.0;
	args.is_missile			= true;											// Original
	//args.is_random_dir		= true;
	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD );
}


//=============================================================================
//=============================================================================
void XDeathSplat(mobj_t *actor) 
{
	//mobjtype_t types[] = { MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD };
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3); 
	int particles =  NUM_PARTICLES_BLOOD_MAX; //GetRandomIntegerInRange(32,NUM_PARTICLES_BLOOD_MAX);
	float scale = GetRandomFloatInRange(0.8, 2.0);

	const float mom_scale = scale;

	//const float mom_scale = 1.0;		// was 0.2
	initParticleSystemArgs(&args);
	args.style = style;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_RANDOM;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;					// Original
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE;
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.num_particles		= particles;
	args.scale_size			=  0.2;
	args.is_missile			= true;											// Original
	// args.is_random_dir		= false;
	//args.scale_mom.x *= mom_scale;
	//args.scale_mom.y *= mom_scale;
	//args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_BLOOD /*DARKBLOOD*/);
}


//=============================================================================
//=============================================================================
void BrutalSplat(mobj_t *actor) 
{
	//mobjtype_t types[] = { MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD };
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3);
	int particles = NUM_PARTICLES_BLOOD_MAX; // GetRandomIntegerInRange(32,NUM_PARTICLES_BLOOD_MAX);
	float scale = GetRandomFloatInRange(0.8, 2.0);

	const float mom_scale = scale;

	//const float mom_scale = 1.0;		// was 0.2
	initParticleSystemArgs(&args);
	args.style = style;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_RANDOM;
//	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;					// Original
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE;
//	args.style				= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.num_particles		= particles;
	args.scale_size			=  1.0;
	args.is_missile			= true;											// Original
	// args.is_random_dir		= false;
	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD);
}


void ParticleFX_ChainsawBloodSplat(mobj_t *actor)
{
	particlesystem_range_args_t args;

	initParticleSystemRangeArgs(&args);
	// WAS args.num_particles	= GetRandomIntegerInRange(NUM_PARTICLES_BLOOD_MIN, NUM_PARTICLES_BLOOD_MAX);
	args.num_particles	= NUM_PARTICLES_BLOOD_MAX / 8;  
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size_lo	= 2;  // was 2
	args.scale_size_hi	= 5;  // was 5
	args.style			= PARTICLESYSTEM_STYLE_SPHERE_RANDOM;
	args.scale_mom_x_lo	= 0.1;
	args.scale_mom_x_hi	= 0.75;
	args.scale_mom_y_lo	= 0.1;
	args.scale_mom_y_hi	= 0.75;
	args.scale_mom_z_lo	= 0.1;
	args.scale_mom_z_hi	= 0.75;

	Founders_ParticleCloud0621_NU(actor, &args);
}


//=============================================================================
//								E N T R Y
//=============================================================================
void Founders_Mushroom(mobj_t *actor, mobjtype_t type)
{
//	Founders_Mushroom1028(actor, MT_FATSHOT);
//	Founders_Mushroom1030(actor, MT_FATSHOT);

//	Founders_ParticleSystem_Baseline(actor);
//	Founders_ParticleSystem_GreenBlueBurst(actor);
//	Founders_ParticleSystem_Nuclear(actor);							// awesome
//	Founders_ParticleSystem_VerticalBlastColumn(actor);
//	Founders_ParticleSystem_HorizontalBlast(actor);
//	Founders_ParticleSystem_Blood(actor);
//	Founders_ParticleSystem_Smoke(actor);
//	Founders_ParticleSystem_Smoke_Blast(actor);		
//	Founders_ParticleSystem_WIP(actor);
//	Founders_ParticleSystem_Saucer(actor);
//	Founders_ParticleSystem_Smoke_Fog_WIP(actor);

//	Founders_ParticleSystem_Baseline1031(actor);
//	Founders_ParticleSystem_Baseline1031(actor);				// cool FX when disabling << FRACBITS section
//	Founders_ParticleSystem_GreenBlueBurst1031(actor);
//	Founders_ParticleSystem_Nuclear1031(actor);
//	Founders_ParticleSystem_VerticalBlastColumn1031(actor);
//	Founders_ParticleSystem_Pillar1031(actor);
//	Founders_ParticleSystem_Wall1031(actor);
//	Founders_ParticleSystem_Smoke_Foggi1031(actor);    // this one crashed the game
//	Founders_ParticleSystem_Blood1031(actor);
//	Founders_ParticleSystem_Smoke_Column1031(actor);		// this one might be good for barrels


// 	Founders_ParticleSystem_Radial1031(actor);					// "Good..."
//	Founders_ParticleSystem_Radial1101(actor);
	Founders_ParticleSystem_GreenBlueBurst1101(actor);
//	Founders_ParticleSystem_Radial1210(actor);
}


void ParticleFX_XDeath(mobj_t *actor)
{
	//if (actor->type == MT_SKULL)
	//	return;

	XDeathSplat(actor);
}


void BarrelFX_Test(mobj_t *actor)
{
	if (!barrel_fx/* || Marshmallow_GibMode != BRUTAL_GIBS*/ /*|| realnetgame*/)
		return;

	Founders_ParticleSystem_Smoke_Column1031(actor);
}


void ParticleFX_Test(mobj_t *actor)
{
	BloodBurst(actor);
}


void ParticleFX_BloodSplat(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	// WAS args.num_particles	= GetRandomIntegerInRange(NUM_PARTICLES_BLOOD_MIN, NUM_PARTICLES_BLOOD_MAX);
	args.num_particles	= NUM_PARTICLES_BLOOD_MAX;
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= 1.0 ;//GetRandomFloatInRange(0.01, 0.05); // was 1; // 1.0; // 0.1;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 1.0; // 2.0; // 0.5;
	args.scale_mom.y	*= 1.0; // 2.0; // 0.5;
	args.scale_mom.z	*= 1.0; // 2.0; // 0.5;
	Founders_ParticleCloud1031(actor, &args);
}




void ParticleFX_SmallBloodSplat(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	// WAS args.num_particles	= GetRandomIntegerInRange(NUM_PARTICLES_BLOOD_MIN, NUM_PARTICLES_BLOOD_MAX);
	args.num_particles	= NUM_PARTICLES_BLOOD_MAX/* / 4*/;  
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= GetRandomFloatInRange(1.0, 1.0); // was 1; // 1.0; // 0.1;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 0.5; // 2.0; // 0.5;
	args.scale_mom.y	*= 0.5; // 2.0; // 0.5;
	args.scale_mom.z	*= 0.5; // 2.0; // 0.5;
	Founders_ParticleCloud0621(actor, &args);
}



void ParticleFX_EXPBloodSplat(mobj_t *actor)
{
	particlesystem_range_args_t args;

	initParticleSystemRangeArgs(&args);
	// WAS args.num_particles	= GetRandomIntegerInRange(NUM_PARTICLES_BLOOD_MIN, NUM_PARTICLES_BLOOD_MAX);
	args.num_particles	= NUM_PARTICLES_BLOOD_MAX / 2;  
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size_lo	= 0.5;
	args.scale_size_hi	= 1.0;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom_x_lo	= 0.1;
	args.scale_mom_x_hi	= 0.75;
	args.scale_mom_y_lo	= 0.1;
	args.scale_mom_y_hi	= 0.75;
	args.scale_mom_z_lo	= 0.1;
	args.scale_mom_z_hi	= 0.75;

	Founders_ParticleCloud0621_NU(actor, &args);
}






void ParticleFX_SmokeBlast(mobj_t *actor)
{
	//Founders_ParticleSystem_Smoke_Column1031(actor);
	Founders_ParticleSystem_Smoke_1101(actor);
}




void ParticleFX_Nuclear(mobj_t *actor)
{
	Founders_ParticleSystem_Nuclear(actor);
}



void ParticleFX_CyberdemonDeath(mobj_t *actor)
{
	//Founders_ParticleSystem_Nuclear(actor);  // this one looks good but is a bit big/extreme, and also often kills you
	//Founders_ParticleSystem_Pillar1031(actor);
	Founders_ParticleSystem_Baseline1031(actor);  // WAS using this one for a long time
	//Founders_ParticleSystem_Smoke_Column(actor);
}



void ParticleFX_SpiderDeath(mobj_t *actor)
{
	Founders_ParticleSystem_GreenBlueBurst1101(actor);
}


void ParticleFX_NewSpiderDeath(mobj_t *actor)
{
	mobjtype_t types[] = { MT_PLASMA, MT_ARACHPLAZ };
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles = 1024;
	Founders_ParticleCloudDuo1030(actor, &args, MT_PLASMA, MT_ARACHPLAZ);
//	Founders_ParticleCloudMulti1030(actor, &args, types, sizeof(types) / sizeof(types[0]));
}


void ParticleFX_HorizontalBlast(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.type			= MT_ARACHPLAZ;  // was FATSHOT
	args.scale_size		= 48;   // was 128
	args.scale_h		= 4.0;  // was 3.0
	args.scale_v		= 0.1;  // was 1.0, then 0.5
	args.scale_mom.x	= 1.0;  // was 2.0
	args.scale_mom.y	= 1.0;  // was 2.0
	args.scale_mom.z	= 1.0;  // was 2.0	// all originally intended to be 4.0
	Founders_ParticleCloud1031(actor, &args);
}


void ParticleFX_SaucerBlast(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles	= 1024;
	args.type			= MT_ARACHPLAZ;
	args.scale_size		= 0.5;
	args.scale_mom.x	= 1.0;
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= 0.1;
	Founders_ParticleCloud1030(actor, &args);
}



void ParticleFX_BossDeaths(mobj_t* actor)  // TODO: convert this to DoBossExplosions()
{
   	if (!Marshmallow_EpicBossDeaths)		
	return;

	if (actor->type == MT_SPIDER)   // spiderboss has a green/blue plasma blast
	{
		ParticleFX_SpiderDeath(actor);
		//ParticleFX_NewSpiderDeath(actor);
	}
	else if (actor->type == MT_CYBORG)   // cyberdemon has a red/orange blastwave
	{
		ParticleFX_CyberdemonDeath(actor);
	}
}