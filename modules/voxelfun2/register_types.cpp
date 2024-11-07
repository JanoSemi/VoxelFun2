/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "entities/entity.h"
#include "entities/player/player.h"
#include "worldgen/voxel_generator_voxelfun2.h"

void register_voxelfun2_types() {
	// Worldgen
	ClassDB::register_class<VoxelGeneratorVoxelFun2>();
	// Entites
	ClassDB::register_virtual_class<Entity>();
	ClassDB::register_class<Player>();
}

void unregister_voxelfun2_types() {
}
