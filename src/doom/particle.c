////////////////////////////////////////////////////
//
//  PARTICLE.C:  Particles for blood and explosions
//
////////////////////////////////////////////////////


#include "marshmallow.h"

extern void P_CheckMissileSpawn (mobj_t* th);
extern float fmix(float, float, float);

#define MAX_PARTICLES		            4096
#define NUM_PARTICLES_DEFAULT			1024

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

    th->info->deathsound = sfx_None;
    th->info->damage = 0;

    th->target = source;
    an = R_PointToAngle2 (source->x, source->y, dest->x, dest->y);

    th->flags |= MF_NOBLOCKMAP|MF_MISSILE|MF_DROPOFF|MF_NOGRAVITY|MF_TRANSLUCENT;
    th->info->speed = 15*FRACUNIT;

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

	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

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

			dx = fmix(-maxWidth, maxWidth, i_pct);
			dy = fmix(-maxWidth, maxWidth, j_pct);
			dz = fmix(0, maxHeight, k_pct);


			// SHAPE: random box

			dx *= h_scale;
			dy *= h_scale;
			dz *= v_scale;

			dx *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dy *= fmix(scale_random_lo, scale_random_hi, F_Random());
			dz *= fmix(scale_random_lo, scale_random_hi, F_Random());

			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;

			p->x = dx;
			p->y = dy;
			p->z = dz;

			origin_offset.x = origin_delta_x * maxWidth;
			origin_offset.y = origin_delta_y * maxWidth;
			origin_offset.z = origin_delta_z * maxWidth;


			target.x += origin_offset.x + dx;
			target.y += origin_offset.y + dy;
			target.z += origin_offset.z + dz;

			mo = P_SpawnGib(actor, &target, type);
			mo->interp = true;
			
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

	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

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

			dx = fmix(-maxWidth, maxWidth, i_pct);
			dy = fmix(-maxWidth, maxWidth, j_pct);
			dz = fmix(0, maxHeight, k_pct);

			// SHAPE: random box

			dx *= h_scale;
			dy *= h_scale;
			dz *= v_scale;

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

			mo = P_SpawnGib(actor, &target, type);
			mo->interp = true;
			
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

		type = types[GetRandomArrayIndex(numTypes)];
		mom_scale_h = 1.0;
		mom_scale_v = 0.5;
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
	const int numParticlesImmediate = 1;
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
		return;			// Base case
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

	Founders_ParticleCloud1028(actor, type, numParticles, 1.0, 1.0, 1.0,  mom_scales, randomScale, enableMissileMode);
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


void Founders_ParticleCloud1031(
	mobj_t *actor, particlesystem_args_t *args)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * args->scale_size;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - args->scale_random;
	float scale_random_hi = 1.0 + args->scale_random;
	int theta, theta_delta;
	int pitch, pitch_delta;

	int n, numParticles;
	int maxInt = INT_MAX;

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);

	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= n; j++)
		{
			mobj_t target = *actor, *mo;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct		= (float)i / (float)n;
			float j_pct		= (float)j / (float)n;
			float k_pct		= (float)numParticles / (float)args->num_particles;
			int yaw			= i_pct * INT_MAX;
			int pitch		= j_pct * INT_MAX;
			int yaw_cos		= finecosine[yaw >> ANGLETOFINESHIFT];
			int yaw_sin		= finesine[yaw >> ANGLETOFINESHIFT];
			int pitch_cos	= finecosine[pitch >> ANGLETOFINESHIFT];
			int pitch_sin	= finesine[pitch >> ANGLETOFINESHIFT];
			int radius		= maxWidth;

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:						// Box, even distribution
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = maxHeight;
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:						// Box, randomly distributed
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(-maxHeight, maxHeight, F_Random());
				break;
			case PARTICLESYSTEM_STYLE_SPHERE:						// Spherical
				dx = FixedMul(radius, yaw_cos);
				dy = FixedMul(radius, yaw_sin);
				dz = FixedMul(radius, pitch_sin);
				break;
			case PARTICLESYSTEM_STYLE_SPHERE_RANDOM:						// Spherical random
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

			dx << FRACBITS;						// Like theta += theta_delta
			dy << FRACBITS;						// Like pitch += pitch_delta

			dz << FRACBITS;						// Makes it way bigger

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);

            //mo->info->speed = 2*FRACUNIT;  // 2020
            //mo->info->mass = 1; // 2020
            mo->info->damage = 0; // 2020
			
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


void Founders_ParticleCloud0621(
	mobj_t *actor, particlesystem_args_t *args)
{
	int i, j, k;
	int x;

	const int maxWidthBaseline = 12000000;
	const int maxWidth = (float)maxWidthBaseline * args->scale_size;
	const int maxHeight = maxWidth;
	float scale_random_lo = 1.0 - args->scale_random;
	float scale_random_hi = 1.0 + args->scale_random;
	int theta, theta_delta;
	int pitch, pitch_delta;

	int n, numParticles;
	int maxInt = INT_MAX;

	if (!actor)
	    return;

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);

	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= n; j++)
		{
			mobj_t target = *actor, *mo;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct		= (float)i / (float)n;
			float j_pct		= (float)j / (float)n;
			float k_pct		= (float)numParticles / (float)args->num_particles;
			int yaw			= i_pct * INT_MAX;
			int pitch		= j_pct * INT_MAX;
			int yaw_cos		= finecosine[yaw >> ANGLETOFINESHIFT];
			int yaw_sin		= finesine[yaw >> ANGLETOFINESHIFT];
			int pitch_cos	= finecosine[pitch >> ANGLETOFINESHIFT];
			int pitch_sin	= finesine[pitch >> ANGLETOFINESHIFT];
			int radius		= maxWidth;

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:						// Box, even distribution
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = maxHeight;
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:						// Box, randomly distributed
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(-maxHeight, maxHeight, F_Random());
				break;
			case PARTICLESYSTEM_STYLE_SPHERE:						// Spherical
				dx = FixedMul(radius, yaw_cos);
				dy = FixedMul(radius, yaw_sin);
				dz = FixedMul(radius, pitch_sin);
				break;
			case PARTICLESYSTEM_STYLE_SPHERE_RANDOM:
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

			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			
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


void Founders_ParticleCloud0621_NU(
	mobj_t *actor, particlesystem_range_args_t *args)
{
	int i, j, k;
	int x;
	
	const int maxWidthBaseline = 12000000 / 2;
	float scale_random_lo = 1.0 - args->scale_random;
	float scale_random_hi = 1.0 + args->scale_random;
	int theta, theta_delta;
	int pitch, pitch_delta;

	int n, numParticles;
	int maxInt = INT_MAX;

	numParticles = 0;
	n = (int)sqrt((float)args->num_particles);

	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= n; j++)
		{
			mobj_t target = *actor, *mo;
			vec3_t scale_mom;
			int dx, dy, dz;
			int k = numParticles;
			float i_pct		= (float)i / (float)n;
			float j_pct		= (float)j / (float)n;
			float k_pct		= (float)numParticles / (float)args->num_particles;
			int yaw			= i_pct * INT_MAX;
			int pitch		= j_pct * INT_MAX;
			int yaw_cos		= finecosine[yaw >> ANGLETOFINESHIFT];
			int yaw_sin		= finesine[yaw >> ANGLETOFINESHIFT];
			int pitch_cos	= finecosine[pitch >> ANGLETOFINESHIFT];
			int pitch_sin	= finesine[pitch >> ANGLETOFINESHIFT];
			float scale_size = GetRandomFloatInRange(args->scale_size_lo, args->scale_size_hi);	    // Per-particle dynamic size
			float maxWidth	= (float)maxWidthBaseline * scale_size;
			float maxHeight = maxWidth;
			int radius		= maxWidth;

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = maxHeight;
				break;
			case PARTICLESYSTEM_STYLE_BOX_RANDOM:
				dx = fmix(-maxWidth, maxWidth, F_Random());
				dy = fmix(-maxWidth, maxWidth, F_Random());
				dz = fmix(-maxHeight, maxHeight, F_Random());
				break;
			case PARTICLESYSTEM_STYLE_SPHERE:
				dx = FixedMul(radius, yaw_cos);
				dy = FixedMul(radius, yaw_sin);
				dz = FixedMul(radius, pitch_sin);
				break;
			case PARTICLESYSTEM_STYLE_SPHERE_RANDOM:
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

			dx << FRACBITS;						// Like theta += theta_delta
			dy << FRACBITS;						// Like pitch += pitch_delta

			dz << FRACBITS;						// Makes it way bigger

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			
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

			//mo->flipsprite = Crispy_Random() & 1;   // To add more variation to its appearance, randomly flip every particle

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
	float scale_random_lo = 1.0 - args->scale_random;
	float scale_random_hi = 1.0 + args->scale_random;

	particle_t particles[MAX_PARTICLES];
	int n, numParticles;

	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT*4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT/2;

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
			float k_pct = 1.0;						// Original was probably a bug
			
			p = &particles[k];

			switch (args->style)
			{
			case PARTICLESYSTEM_STYLE_BOX_UNIFORM:
				// SHAPE: radial blast outwards
				dx = fmix(-maxWidth, maxWidth, i_pct);
				dy = fmix(-maxWidth, maxWidth, j_pct);
				dz = fmix(0, maxHeight, k_pct);
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

			dx << FRACBITS;
			dy << FRACBITS;
			dz << FRACBITS;

			p->x = dx;
			p->y = dy;
			p->z = dz;

			target.x += dx;
			target.y += dy;
			target.z += dz;

			mo = P_SpawnGib(actor, &target, args->type);
			mo->interp = true;
			
			mo->momx *= args->scale_mom.x;
			mo->momy *= args->scale_mom.y;
			mo->momz *= args->scale_mom.z;

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
}

//=============================================================================
//=============================================================================
void Founders_ParticleCloudDuo1031(mobj_t *actor, particlesystem_args_t *args,  mobjtype_t type1, mobjtype_t type2)
{
	mobjtype_t types[2] = { type1, type2 };
	Founders_ParticleCloudMulti1031(actor, args, types, 2);
}

//=============================================================================
//=============================================================================
void Founders_Mushroom1030(mobj_t *actor, mobjtype_t type)
{
	particlesystem_args_t args;
	
	initParticleSystemArgs(&args);
	Founders_ParticleCloud1030(actor, &args);
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
	args.scale_mom.z	= 2.0;
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
	args.scale_mom.z	= 2.0;
	Founders_ParticleCloud1030(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Blood(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);

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
	args.scale_mom.z	= 2.0;
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
	args.scale_size		= 1;
	args.scale_mom.x	= 0.8;
	args.scale_mom.y	= 0.8;
	args.scale_mom.z	= 0.1;
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
	args.scale_mom.z	= 2.0;
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
	args.scale_mom.x	= 1.0;
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
	args.scale_mom.x	= 0.2;
	args.scale_mom.y	= 0.2;
	args.scale_mom.z	= 0;
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
	args.scale_mom.x	= 0.1;
	args.scale_mom.y	= 0.1;
	args.scale_mom.z	= 0.1;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Blood1031(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);

	args.num_particles	= NUM_PARTICLES_BLOOD_MAX;
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= 1;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 1.0;
	args.scale_mom.y	*= 1.0;
	args.scale_mom.z	*= 1.0;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Smoke_Column1031(mobj_t *actor)
{
	particlesystem_args_t args;

	// THIS REQUIRES THAT mo's speed = 2*FRACUNIT, mass = 1 (info.c)

	initParticleSystemArgs(&args);
	args.num_particles		= GetRandomIntegerInRange(8, 32);
	args.type				= MT_PARTICLE_SMOKE;
	args.scale_size			= 0.5;
	args.scale_mom.z		*= 0.5;
	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;
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

	args.num_particles		= 512;
	args.type				= MT_PARTICLE_SMOKE;
	args.is_missile			= true;
	args.scale_size		    = 128;
	args.scale_mom.x	    = 0.2;
	args.scale_mom.y	    = 0.2;
	args.scale_mom.z	    = 0;
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
	args.scale_mom.x	= 1.0;
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= 1.0;
	args.is_random_dir	= true;
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

	args.scale_size		= 1;
	args.scale_mom.x	= 1.0;
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= 1.0;
	args.is_random_dir	= true;
	args.is_random_offset = false;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_Radial1210(mobj_t *actor)
{
	particlesystem_args_t args;
	const float c = 1.0;

	initParticleSystemArgs(&args);
	args.style			= PARTICLESYSTEM_STYLE_SPHERE;

	args.is_missile		= true;
	args.num_particles	= 1024;
	args.type			= MT_FATSHOT;

	args.scale_size		= 1;
	args.scale_mom.x	= c;
	args.scale_mom.y	= c;
	args.scale_mom.z	= c;
	args.is_random_dir	= true;
	args.is_random_offset = false;
	Founders_ParticleCloud1031(actor, &args);
}

//=============================================================================
//=============================================================================
void Founders_ParticleSystem_GreenBlueBurst1101(mobj_t *actor)
{
	mobjtype_t types[] = { MT_PARTICLE_BLUEPLASMA, MT_PARTICLE_BRUISERSHOT };
	particlesystem_args_t args;

	const float mom_scale = 1.0;
	initParticleSystemArgs(&args);
	args.style				= PARTICLESYSTEM_STYLE_BOX_UNIFORM;
	args.num_particles		= 1024;
	args.scale_size			= 1;
	args.is_missile			= false;

	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLUEPLASMA, MT_PARTICLE_BRUISERSHOT);
}

//=============================================================================
//=============================================================================
void BloodBurst(mobj_t *actor)  // Based on above function, just with BLOOD
{
	mobjtype_t types[] = { MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD };
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3); 
	int particles =  GetRandomIntegerInRange(32,NUM_PARTICLES_BLOOD_MAX);
	float scale = GetRandomFloatInRange(0.8, 2.0);

	const float mom_scale = scale;

	initParticleSystemArgs(&args);
	args.style = style;

	args.num_particles		= particles;
	args.scale_size			=  1.0;
	args.is_missile			= true;

	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD);
}

//=============================================================================
//=============================================================================
void CorpseGib(mobj_t *actor) 
{
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3);
	int particles = NUM_PARTICLES_BLOOD_MAX;
	float scale = GetRandomFloatInRange(0.8, 2.0); 

	const float mom_scale = scale;

	initParticleSystemArgs(&args);
	args.style = style;

	args.num_particles		= particles;
	args.scale_size			=  1.0;
	args.is_missile			= true;

	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD );
}

//=============================================================================
//=============================================================================
void XDeathSplat(mobj_t *actor) 
{
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3); 
	int particles =  NUM_PARTICLES_BLOOD_MAX;
	float scale = GetRandomFloatInRange(0.8, 2.0);

	const float mom_scale = scale;

	initParticleSystemArgs(&args);
	args.style = style;

	args.num_particles		= particles;
	args.scale_size			=  0.2;
	args.is_missile			= true;

	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_BLOOD);
}

//=============================================================================
//=============================================================================
void BrutalSplat(mobj_t *actor) 
{
	particlesystem_args_t args;

	int style = GetRandomIntegerInRange(2,3);
	int particles = NUM_PARTICLES_BLOOD_MAX;
	float scale = GetRandomFloatInRange(0.8, 2.0);

	const float mom_scale = scale;

	initParticleSystemArgs(&args);
	args.style = style;

	args.num_particles		= particles;
	args.scale_size			=  1.0;
	args.is_missile			= true;

	args.scale_mom.x *= mom_scale;
	args.scale_mom.y *= mom_scale;
	args.scale_mom.z *= mom_scale;
	Founders_ParticleCloudDuo1031(actor, &args, MT_PARTICLE_BLOOD, MT_PARTICLE_DARKBLOOD);
}

void ParticleFX_ChainsawBloodSplat(mobj_t *actor)
{
	particlesystem_range_args_t args;

	initParticleSystemRangeArgs(&args);

	args.num_particles	= NUM_PARTICLES_BLOOD_MAX / 8;  
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size_lo	= 2;
	args.scale_size_hi	= 5;
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
    Founders_ParticleSystem_GreenBlueBurst1101(actor);

//	Founders_Mushroom1028(actor, MT_FATSHOT);
//	Founders_Mushroom1030(actor, MT_FATSHOT);

//	Founders_ParticleSystem_Baseline(actor);
//	Founders_ParticleSystem_GreenBlueBurst(actor);
//	Founders_ParticleSystem_Nuclear(actor);					 // Awesome
//	Founders_ParticleSystem_VerticalBlastColumn(actor);
//	Founders_ParticleSystem_HorizontalBlast(actor);
//	Founders_ParticleSystem_Blood(actor);
//	Founders_ParticleSystem_Smoke(actor);
//	Founders_ParticleSystem_Smoke_Blast(actor);		
//	Founders_ParticleSystem_WIP(actor);
//	Founders_ParticleSystem_Saucer(actor);
//	Founders_ParticleSystem_Smoke_Fog_WIP(actor);

//	Founders_ParticleSystem_Baseline1031(actor);
//	Founders_ParticleSystem_Baseline1031(actor);			 // Cool FX when disabling << FRACBITS section
//	Founders_ParticleSystem_GreenBlueBurst1031(actor);
//	Founders_ParticleSystem_Nuclear1031(actor);
//	Founders_ParticleSystem_VerticalBlastColumn1031(actor);
//	Founders_ParticleSystem_Pillar1031(actor);
//	Founders_ParticleSystem_Wall1031(actor);
//	Founders_ParticleSystem_Smoke_Foggi1031(actor);          // This one crashed the game
//	Founders_ParticleSystem_Blood1031(actor);
//	Founders_ParticleSystem_Smoke_Column1031(actor);		// This one might be good for barrels

// 	Founders_ParticleSystem_Radial1031(actor);				// "Good..."
//	Founders_ParticleSystem_Radial1101(actor);
//	Founders_ParticleSystem_Radial1210(actor);
}


void ParticleFX_XDeath(mobj_t *actor)
{
	XDeathSplat(actor);
}


void BarrelFX_Test(mobj_t *actor)
{
	if (!barrel_fx)
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

	args.num_particles	= NUM_PARTICLES_BLOOD_MAX;
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= 1.0 ;
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 1.0;
	args.scale_mom.y	*= 1.0;
	args.scale_mom.z	*= 1.0;
	Founders_ParticleCloud1031(actor, &args);
}


void ParticleFX_SmallBloodSplat(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);

	args.num_particles	= 128;
	args.type			= MT_PARTICLE_BLOOD;
	args.scale_size		= GetRandomFloatInRange(1.0, 1.0);
	args.style			= PARTICLESYSTEM_STYLE_BOX_RANDOM;
	args.scale_mom.x	*= 0.5;
	args.scale_mom.y	*= 0.5;
	args.scale_mom.z	*= 0.5;
	Founders_ParticleCloud0621(actor, &args);
}


void ParticleFX_TinyBloodSplat(mobj_t *actor)
{
    particlesystem_args_t args;

    initParticleSystemArgs(&args);

    args.num_particles	= 128;
    args.type			= MT_PARTICLE_BLOOD;
    args.scale_size		= GetRandomFloatInRange(0.01, 0.1);
    args.style			= PARTICLESYSTEM_STYLE_BOX_UNIFORM;
    args.scale_mom.x	*= 0.5;
    args.scale_mom.y	*= 0.5;
    args.scale_mom.z	*= 0.5;
    Founders_ParticleCloud0621(actor, &args);
}


void ParticleFX_EXPBloodSplat(mobj_t *actor)
{
	particlesystem_range_args_t args;

	initParticleSystemRangeArgs(&args);

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
	Founders_ParticleSystem_Smoke_1101(actor);
}


void ParticleFX_Nuclear(mobj_t *actor)
{
	Founders_ParticleSystem_Nuclear(actor);
}


void ParticleFX_CyberdemonDeath(mobj_t *actor)
{
	Founders_ParticleSystem_Baseline1031(actor);
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
}


void ParticleFX_HorizontalBlast(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.type			= MT_ARACHPLAZ;
	args.scale_size		= 48;
	args.scale_h		= 4.0;
	args.scale_v		= 0.1;
	args.scale_mom.x	= 1.0;
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= 1.0;
	Founders_ParticleCloud1031(actor, &args);
}


void ParticleFX_SaucerBlast(mobj_t *actor)
{
	particlesystem_args_t args;

	initParticleSystemArgs(&args);
	args.num_particles	= 256;
	args.type			= MT_ARACHPLAZ;
	args.scale_size		= 0.5;
	args.scale_mom.x	= 1.0;
	args.scale_mom.y	= 1.0;
	args.scale_mom.z	= 0.1;
	Founders_ParticleCloud1030(actor, &args);
}


void ParticleFX_BabyDeath(mobj_t *actor)
{
    particlesystem_args_t args;
    int particle_type = MT_ARACHPLAZ;

    initParticleSystemArgs(&args);
    args.num_particles	= 64;
    args.type			= particle_type;
    args.scale_size		= 0.03;
    args.scale_mom.x	= 1.0;
    args.scale_mom.y	= 1.0;
    args.scale_mom.z	= 0.1;
    Founders_ParticleCloud1030(actor, &args);
}


void DoBossExplosions(mobj_t* actor)
{
   	if (!Marshmallow_EpicBossDeaths)		
	return;

	if (actor->type == MT_SPIDER)   // Spiderboss has a green/blue plasma blast
	{
		ParticleFX_SpiderDeath(actor);
		//ParticleFX_NewSpiderDeath(actor);
	}
	else if (actor->type == MT_CYBORG)   // Cyberdemon has a red/orange blastwave
	{
		ParticleFX_CyberdemonDeath(actor);
	}
}