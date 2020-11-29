// [marshmallow] For storing inventory info inside the player's dropped backpack
typedef struct {

    int items[7];
    int weapons[NUMWEAPONS];
    int ammo[NUMAMMO];

    boolean backpack;

} backpack_s;