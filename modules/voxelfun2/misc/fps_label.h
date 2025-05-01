#ifndef FPS_LABEL_H
#define FPS_LABEL_H

#include "core/ustring.h"
#include "scene/gui/label.h"

class FPSLabel : public Label {
	GDCLASS(FPSLabel, Label);

public:
	FPSLabel();
	~FPSLabel();

	void set_template_string(String t);
	String get_template_string() const;

protected:
	void _notification(int p_what);

	static void _bind_methods();

private:
	String template_string;
};

#endif // FPS_LABEL_H
