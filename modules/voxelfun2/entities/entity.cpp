#include "entity.h"

Entity::Entity() {
}

Entity::~Entity() {
}

void Entity::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY:
			set_terrain_path(terrain_path);
			break;
		case Node::NOTIFICATION_PHYSICS_PROCESS:
			if (!engine->is_editor_hint()) {
				_physics_process(get_physics_process_delta_time());
			}
			break;
	}
}

void Entity::_physics_process(float delta) {}

Vector3 Entity::move_and_collide(Vector3 velocity) {
	float delta = get_physics_process_delta_time();
	Vector3 origin = get_translation();
	Vector3 fd_velocity = velocity * delta;
	fd_velocity = terrain_collision.get_motion(origin, fd_velocity, hitbox, terrain);
	origin += fd_velocity;
	set_translation(origin);
	return fd_velocity / delta;
}

void Entity::set_hitbox(AABB hb) {
	hitbox = hb;
}
AABB Entity::get_hitbox() const {
	return hitbox;
}

void Entity::set_collision_mask(uint32_t cm) {
	collision_mask = cm;
	terrain_collision.set_collision_mask(collision_mask);
}
uint32_t Entity::get_collision_mask() const {
	return collision_mask;
}

void Entity::set_terrain_path(NodePath tp) {
	terrain_path = tp;
	if (!has_node(terrain_path)) {
		return;
	}
	VoxelTerrain *t = Object::cast_to<VoxelTerrain>(get_node(tp));
	if (t) {
		terrain = t;
		tool = terrain->get_voxel_tool();
	}
}
NodePath Entity::get_terrain_path() const {
	return terrain_path;
}

void Entity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_hitbox", "hb"), &Entity::set_hitbox);
	ClassDB::bind_method(D_METHOD("get_hitbox"), &Entity::get_hitbox);
	ClassDB::bind_method(D_METHOD("set_collision_mask", "cm"), &Entity::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &Entity::get_collision_mask);
	ClassDB::bind_method(D_METHOD("set_terrain"), &Entity::set_terrain_path);
	ClassDB::bind_method(D_METHOD("get_terrain"), &Entity::get_terrain_path);

	ADD_GROUP("Collision", "collision_");
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "collision_hitbox"), "set_hitbox", "get_hitbox");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_GROUP("Nodes", "node_");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_terrain", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "VoxelTerrain"), "set_terrain", "get_terrain");
}
