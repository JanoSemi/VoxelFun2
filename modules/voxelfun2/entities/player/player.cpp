#include "player.h"

Player::Player() {
	if (engine->is_editor_hint()) {
		return;
	}
	set_physics_process(true);
	set_process_input(true);
	input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
}

Player::~Player() {
}

void Player::_physics_process(float delta) {
	Transform t = get_transform();
	if (tool.is_valid()) {
		grounded = tool->raycast(t.origin - Vector3(0.0f, 0.9f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), 0.1f, 1).is_valid();
	}
	// Movement
	Vector2 dir = input->get_vector("left", "right", "forward", "backward") * speed;
	velocity.x = 0;
	velocity.z = 0;
	velocity += t.basis.get_axis(Vector3::AXIS_X) * dir.x;
	velocity += t.basis.get_axis(Vector3::AXIS_Z) * dir.y;
	// Jump
	if (input->is_action_pressed("jump") && grounded) {
		velocity.y = 5;
	}
	// Gravity
	velocity.y -= gravity * delta;
	// Apply
	velocity = move_and_collide(velocity);
}

void Player::_input(const Ref<InputEvent> &p_event) {
	if (input->get_mouse_mode() != Input::MOUSE_MODE_CAPTURED || engine->is_editor_hint()) {
		return;
	}
	const InputEventMouseMotion *mme = Object::cast_to<InputEventMouseMotion>(*p_event);
	if (mme) {
		rotate_y(mme->get_relative().x * horizontal_sensitivity);
		if (camera) {
			camera->set_rotation(Vector3(clamp<float>(camera->get_rotation().x + mme->get_relative().y * vertical_sensitivity, -Math_PI / 2, Math_PI / 2), 0.0f, 0.0f));
		}
	}
	Ref<VoxelRaycastResult> pointed_voxel = get_pointed_result();
	if (pointed_voxel.is_valid()) {
		if (p_event->is_action_pressed("break_block")) {
			tool->set_voxel(pointed_voxel->position, 0);
		}
		if (p_event->is_action_pressed("place_block")) {
			tool->set_voxel(pointed_voxel->previous_position, 1);
		}
	}
}

void Player::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY:
			set_camera_path(camera_path);
			break;
	}
}

Ref<VoxelRaycastResult> Player::get_pointed_result() {
	return tool->raycast(camera->get_global_transform().origin, -camera->get_global_transform().basis.get_axis(Vector3::AXIS_Z), 10.0, 1);
}

// Setter/Getter
void Player::set_speed(float s) { speed = s; };
float Player::get_speed() const { return speed; };

void Player::set_jump_power(float p) { jump_power = p; };
float Player::get_jump_power() const { return jump_power; };

void Player::set_gravity(float g) { gravity = g; };
float Player::get_gravity() const { return gravity; };

void Player::set_horizontal_sensitivity(float hs) { horizontal_sensitivity = hs / -100; };
float Player::get_horizontal_sensitivity() const { return horizontal_sensitivity * -100; };

void Player::set_vertical_sensitivity(float vs) { vertical_sensitivity = vs / -100; };
float Player::get_vertical_sensitivity() const { return vertical_sensitivity * -100; };

void Player::set_camera_path(const NodePath cp) {
	camera_path = cp;
	if (!has_node(camera_path)) {
		return;
	}
	Camera *c = Object::cast_to<Camera>(get_node(camera_path));
	if (c) {
		camera = c;
	}
}
NodePath Player::get_camera_path() const { return camera_path; }

void Player::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_speed", "s"), &Player::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &Player::get_speed);
	ClassDB::bind_method(D_METHOD("set_jump_power", "p"), &Player::set_jump_power);
	ClassDB::bind_method(D_METHOD("get_jump_power"), &Player::get_jump_power);
	ClassDB::bind_method(D_METHOD("set_gravity", "g"), &Player::set_gravity);
	ClassDB::bind_method(D_METHOD("get_gravity"), &Player::get_gravity);
	ClassDB::bind_method(D_METHOD("set_horizontal_sensitivity", "hs"), &Player::set_horizontal_sensitivity);
	ClassDB::bind_method(D_METHOD("get_horizontal_sensitivity"), &Player::get_horizontal_sensitivity);
	ClassDB::bind_method(D_METHOD("set_vertical_sensitivity", "vs"), &Player::set_vertical_sensitivity);
	ClassDB::bind_method(D_METHOD("get_vertical_sensitivity"), &Player::get_vertical_sensitivity);
	ClassDB::bind_method(D_METHOD("set_camera"), &Player::set_camera_path);
	ClassDB::bind_method(D_METHOD("get_camera"), &Player::get_camera_path);
	ClassDB::bind_method(D_METHOD("_input"), &Player::_input);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "speed"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "jump_power"), "set_jump_power", "get_jump_power");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "gravity"), "set_gravity", "get_gravity");
	ADD_GROUP("Sensitivity", "sensitivity_");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "sensitivity_horizontal"), "set_horizontal_sensitivity", "get_horizontal_sensitivity");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "sensitivity_vertical"), "set_vertical_sensitivity", "get_vertical_sensitivity");
	ADD_GROUP("Nodes", "node_");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_camera", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Camera"), "set_camera", "get_camera");
}
