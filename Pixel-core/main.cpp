
#include <time.h>

#include "src/maths/maths.h"
#include "src/utils/timer.h"

#include "src/graphics/batchrenderer2d.h"
#include "src/graphics/label.h"
#include "src/graphics/shader.h"
#include "src/graphics/sprite.h"
#include "src/graphics/texture.h"
#include "src/graphics/window.h"
#include "src/graphics/layers/flatlayer.h"
#include "src/graphics/layers/group.h"


int main() {
	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 1600, 900);

	Shader* shader = new Shader("src/shaders/basic.vs", "src/shaders/basic.fs");

	FlatLayer layer(shader);

	float pad = 2.0f;
	float size = 2.0f;
	float gap = 0.1f;

	Texture* textures[] = {
		new Texture("test.png"),
		new Texture("test_white.png"),
		new Texture("test_red.png"),
		new Texture("test_green.png"),
		new Texture("test_blue.png"),
	};

	for (float y = -9.0f + pad; y < 9.0f - pad; y += size) {
		for (float x = -16.0f + pad; x < 16.0f - pad; x += size) {
			float value = rand() % 1000 / 1000.0f;
			//layer.add(new Sprite(x, y, size - gap, size - gap, vec4f(value, value, value, 1)));
			layer.add(new Sprite(x, y, size - gap, size - gap, textures[rand() % 5]));
		}
	}

	Group* group = new Group(maths::mat4::translation(vec3f(-16.0f, 9.0f, 0.0f)));
	Label* fps = new Label("", 0.5f, -1.0f, 0xFF00FF00);
	group->add(fps);

	layer.add(group);

	GLint textureIds[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	shader->enable();
	shader->setUniform1iv("textures", textureIds, 10);
	shader->setUniformMat4("pr_matrix", mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

	std::cout << layer.count() << std::endl;


	double mx, my;

	Timer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed()) {
		window.clear();
		window.getMousePos(mx, my);

		//mat4 mat = mat4::translation(vec3f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f), 0));
		//mat4 mat = mat4::rotation(time.elapsed() * 100.0f, vec3f(0, 0, 1));
		//mat *= mat4::translation(vec3f(-(float)((mx - 800.0f) * 16.0f / 800.0f), -(float)(9.0f - my * 9.0f / 450.0f), 0));

		shader->setUniform2f("light_pos", vec2f((float)(mx * 32.0f / window.getWidth() - 16.0f), (float)(9.0f - my * 18.0f / window.getHeight())));
		layer.render();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f) {
			timer += 1.0f;
			fps->text = std::to_string(frames) + " fps";
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	return 0;
}