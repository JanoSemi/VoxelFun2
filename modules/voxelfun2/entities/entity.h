#ifndef ENTITY_H
#define ENTITY_H

#include "core/engine.h"
#include "core/math/vector3.h"
#include "core/node_path.h"
#include "modules/voxel/edition/voxel_tool_terrain.h"
#include "modules/voxel/terrain/voxel_box_mover.h"
#include "modules/voxel/terrain/voxel_terrain.h"
#include "scene/3d/spatial.h"

class Entity : public Spatial {
	GDCLASS(Entity, Spatial)
public:
	Entity();
	~Entity();

	virtual void _physics_process(float delta);

	Vector3 move_and_collide(Vector3 velocity);

	void set_hitbox(AABB hb);
	AABB get_hitbox() const;

	void set_collision_mask(uint32_t cm);
	uint32_t get_collision_mask() const;

	void set_terrain_path(NodePath tp);
	NodePath get_terrain_path() const;

protected:
	virtual void _notification(int p_what);
	static void _bind_methods();

	AABB hitbox;
	uint32_t collision_mask = 0xffffffff;
	NodePath terrain_path;
	VoxelTerrain *terrain;
	Ref<VoxelTool> tool;
	VoxelBoxMover terrain_collision;
	Engine *engine = Engine::get_singleton();
};

#endif // ENTITY_H