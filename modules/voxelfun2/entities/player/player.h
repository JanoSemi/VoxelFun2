#ifndef PLAYER_H
#define PLAYER_H

#include "../entity.h"
#include "core/os/input.h"
#include "core/os/input_event.h"
#include "modules/voxel/edition/voxel_raycast_result.h"
#include "scene/3d/camera.h"

class Player : public Entity {
	GDCLASS(Player, Entity)

public:
	Player();
	~Player();

	Ref<VoxelRaycastResult> get_pointed_result();

	// Setter/Getter
	void set_speed(float s);
	float get_speed() const;

	void set_jump_power(float p);
	float get_jump_power() const;

	void set_gravity(float g);
	float get_gravity() const;

	void set_horizontal_sensitivity(float hs);
	float get_horizontal_sensitivity() const;

	void set_vertical_sensitivity(float vs);
	float get_vertical_sensitivity() const;

	void set_camera_path(const NodePath cp);
	NodePath get_camera_path() const;

	Vector3 velocity;
	float speed = 4.0;
	float jump_power = 5.0;
	float gravity = 9.8;
	float horizontal_sensitivity = -0.01;
	float vertical_sensitivity = -0.01;
	bool grounded = false;
	NodePath camera_path;
	Camera *camera;
	Input *input = Input::get_singleton();

protected:
	void _physics_process(float delta) override;

	void _input(const Ref<InputEvent> &p_event);

	void _notification(int p_what) override;

	static void _bind_methods();
};

#endif // PLAYER_H