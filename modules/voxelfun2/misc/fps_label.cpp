#include "fps_label.h"
#include "core/engine.h"

FPSLabel::FPSLabel() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		set_process(true);
	}
}
FPSLabel::~FPSLabel() {}

void FPSLabel::set_template_string(String t) {
	template_string = t;
}
String FPSLabel::get_template_string() const {
	return template_string;
}

void FPSLabel::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PROCESS:
			set_text(template_string.replace("$fps", itos(Engine::get_singleton()->get_frames_per_second())));
			break;
	}
}

void FPSLabel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_template_string", "t"), &FPSLabel::set_template_string);
	ClassDB::bind_method(D_METHOD("get_template_string"), &FPSLabel::get_template_string);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "template_string"), "set_template_string", "get_template_string");
}
