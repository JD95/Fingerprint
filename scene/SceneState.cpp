#include "SceneState.h"

SceneState::SceneState()
	: physics(20, 20, 0.0f, -9.81f)
{
	int x = 0;
	time = 0;
	auto test = (end - start);
}


SceneState::~SceneState()
{
	for (auto pair : models) {
		delete pair.second;
	}
}

Entity* SceneState::spawn(Model model, Transform transform)
{
	auto new_entity_id = entities.create_object();
	auto new_entity = entities.get_object(new_entity_id);
	new_entity->transform = transform;

	if (models.find(model.model_name) == models.end()) {
		auto new_model = new Polygon(model.model_name, 1, 1, {}, model.tex_coords);
		models.emplace(model.model_name, new_model);
	}

	new_entity->model = models[model.model_name];

	return new_entity;
}

Entity* SceneState::spawn(AnimatedModel model, Transform transform)
{
	auto new_entity_id = entities.create_object();
	auto new_entity = entities.get_object(new_entity_id);
	new_entity->transform = transform;

	if (models.find(model.model_name) == models.end()) {
		auto new_model = new Polygon(model.model_name, model.rows, model.cols, model.animations, model.tex_coords);
		models.emplace(model.model_name, new_model);
	}

	new_entity->model = models[model.model_name];

	return new_entity;
}

Entity* SceneState::spawn(Model model, Transform transform, PhysObj body)
{
	auto new_entity = spawn(model, transform);

	new_entity->body = physics.add_object(body);

	new_entity->body.v->entity_ID = new_entity->id;

	return new_entity;
}

Entity* SceneState::spawn(AnimatedModel model, Transform transform, PhysObj body)
{
	auto new_entity = spawn(model, transform);

	new_entity->body = physics.add_object(body);

	new_entity->body.v->entity_ID = new_entity->id;

	return new_entity;
}

Entity* SceneState::spawn_body(Model model, float layer, float x, float y, float width, float height, float mass)
{
	auto model_width = (width * 0.5f);
	auto model_height = (height * 0.5f);
	return spawn(model,
		Transform(glm::vec3(x + model_width , y + model_height, layer), glm::vec3(model_width, model_height, 1.0f)),
		PhysObj(x + model_width, y + model_height, mass, width, height, world_step));
}

Entity* SceneState::spawn_body(AnimatedModel model, float layer, float x, float y, float width, float height, float mass)
{
	auto model_width = (width * 0.5f);
	auto model_height = (height * 0.5f);
	return spawn(model,
		Transform(glm::vec3(x + model_width, y + model_height, layer), glm::vec3(model_width, model_height, 1.0f)),
		PhysObj(x + model_width, y + model_height, mass, width, height, world_step));
}

Entity* SceneState::spawn_massless(Model model, float layer, float x, float y, float width, float height){
	auto model_width = (width / 2.0f);
	auto model_height = (height / 2.0f);
	return spawn(model,
		Transform(glm::vec3(x + model_width, y + model_height, layer), glm::vec3(model_width, model_height, 1.0f)));
}

Entity* SceneState::spawn_massless(AnimatedModel model, float layer, float x, float y, float width, float height) {
	auto model_width = (width / 2.0f);
	auto model_height = (height / 2.0f);
	return spawn(model,
		Transform(glm::vec3(x + model_width, y + model_height, layer), glm::vec3(model_width, model_height, 1.0f)));
}

Entity* SceneState::gui_spawn(Model model, float x, float y, float width, float height)
{
	auto new_entity_id = gui_entities.create_object();
	auto new_entity = gui_entities.get_object(new_entity_id);
	new_entity->transform = Transform(glm::vec3(x,y,0), glm::vec3(width, height, 1.0f));

	if (models.find(model.model_name) == models.end()) {
		auto new_model = new Polygon(model.model_name, 1, 1, {}, model.tex_coords);
		models.emplace(model.model_name, new_model);
	}

	new_entity->model = models[model.model_name];

	return new_entity;
}

Sound * SceneState::load_sound(std::string path)
{
	auto id = sounds.create_object();
	auto sound = sounds.get_object(id);
	sound->load_from_file(path);
	return sound;
}



void SceneState::render_scene(Camera camera)
{
	physics.step();
	time.v++;
	auto chunks = entities.object_table.size();

	// Renders all entities which have in-use ids
	for (int i = entities.id_list[0]; i < entities.first_free_index; i++){

			auto entity = entities.object_table[i / entities.chunk_size][i % entities.chunk_size];

			auto mvp = camera.perspective_projection(45.0f, 1080.0f/720.0f, 0.10f)
					 * camera.view_matrix()
					 * entity.transform.model_matrix();

			entity.model->render(mvp);
	}

	// Renders all entities which have in-use ids
	for (int i = gui_entities.id_list[0]; i < gui_entities.first_free_index; i++) {

		auto entity = gui_entities.object_table[i / gui_entities.chunk_size][i % gui_entities.chunk_size];

		// Don't apply camera transforms
		auto mvp = entity.transform.model_matrix();

		entity.model->render(mvp);
	}
}

glm::vec2 SceneState::get_grav_norm()
{
	glm::vec2 temp = physics.gravity_acc;
	if (glm::abs(temp.x) > glm::abs(temp.y))
		if (temp.x > 0.0f)
			return glm::vec2(1, 0);
		else
			return glm::vec2(-1, 0);
	else
		if (temp.y > 0.0f)
			return glm::vec2(0, 1);
		else
			return glm::vec2(0, -1);
}
