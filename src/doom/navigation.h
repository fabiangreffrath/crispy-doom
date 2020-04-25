#ifndef __MARSHMALLOW_NAV__
#define __MARSHMALLOW_NAV__


typedef enum {

	MAX_NODES = 100000,
	MAX_DEATHNODES = 256,
	NODE_TIMEOUT = 5,
	NODE_SPACING_DISTACE = 500000, // was 50000,
	NODE_TOUCH_RADIUS = 5000000, // usually 5000000
	NODE_START_OFFSET = 5,
	END_OF_CHAIN_OFFSET = 6,
	NODE_CHAIN_START = 1

} path_defaults_t;


typedef struct navnode_s {

	fixed_t		x;
	fixed_t		y;
	fixed_t		z;

	angle_t		angle;

	boolean		death_node;
	boolean		sprint_node;
	boolean		use_node;
	boolean		teleport_node;

	mobj_t*		marker;

} navnode_t;


typedef struct navpath_s {

	navnode_t	node[MAX_NODES];  // TODO:  replace below variables with this

	fixed_t		x[MAX_NODES];
	fixed_t		y[MAX_NODES];
	fixed_t		z[MAX_NODES];

	angle_t		angle[MAX_NODES];

	int			death_node;	// TODO: make this work like the arrays below
	int			dm_deathnode[MAX_NODES]; 
	
	boolean		sprint_node[MAX_NODES];
	boolean		use_node[MAX_NODES];
	boolean		teleport_node[MAX_NODES];

	boolean		is_active;  // in use?

	int			total_nodes;
	int			node_timer;

	mobj_t*		marker[MAX_NODES];
	boolean		show_markers;  // in use?

} navpath_t;

navpath_t main_path;


typedef struct testpath_s {

	fixed_t		x[MAX_NODES];
	fixed_t		y[MAX_NODES];
	fixed_t		z[MAX_NODES];

	int			total_nodes;

	mobj_t*		marker[MAX_NODES];

} testpath_t;

testpath_t test_path;


// Function prototypes for navigation.c
void Path_Plot(navpath_t* path, mobj_t* actor);
void Path_Follow(navpath_t* path, mobj_t* actor);
int Path_DropNode(navpath_t* path, mobj_t* actor);
int Path_SearchForNodes(navpath_t* path, mobj_t* actor);
int Path_DetermineNextNode(navpath_t* path, mobj_t* actor);
void Path_HandleDeathNode();
void Path_ClearDeathNode();
navpath_t Path_GenerateSegment(mobj_t* actor);
boolean Path_CheckNodeSpacing(navpath_t* path, mobj_t* actor, int node);
boolean Path_FoundNode(navpath_t* path, mobj_t* actor, int node);
boolean Path_FoundDeathNode(navpath_t* path, mobj_t* actor);
boolean Path_CloseToEndReverse(navpath_t* path, mobj_t* actor);
boolean Path_CloseToEnd(navpath_t* path, mobj_t* actor);
boolean Path_IsNodeVisible(int node, navpath_t* path, mobj_t* actor);
boolean IsNodeWalkable(mobj_t *actor);

#endif