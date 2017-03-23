#include "SceneState.h"




SceneState::SceneState()
	: physics(20, 20, 0.0f, -9.81f)
{
	int x = 0;
}


SceneState::~SceneState()
{
	for (auto pair : models) {
		delete pair.second;
	}
}

Entity * SceneState::spawn(Model model, Transform transform)
{
	auto new_entity_id = entities.create_object();
	auto new_entity = entities.get_object(new_entity_id);
	new_entity->transform = transform;

	if (models.find(model.model_name) == models.end()) {
		auto new_model = new Polygon(model.model_name, model.tex_coords);
		models.emplace(model.model_name, new_model);
	}

	new_entity->model = models[model.model_name];

	return new_entity;
}

Entity * SceneState::spawn(Model model, Transform transform, PhysObj body)
{
	auto new_entity = spawn(model, transform);

	new_entity->body = physics.add_object(body);

	return new_entity;
}

Entity * SceneState::spawn_body(Model model, float layer, float x, float y, float width, float height, float mass)
{
	auto model_width = (width / 2.0f);
	auto model_height =(height / 2.0f);
	return spawn(model,
		Transform(glm::vec3(x + model_width , y + model_height, layer), glm::vec3(model_width, model_height, 1.0f)),
		PhysObj(x, y, mass, height, width, world_step));
}

Entity * SceneState::spawn_massless(Model model, float layer, float x, float y, float width, float height){
	auto model_width = (width / 2.0f);
	auto model_height = (height / 2.0f);
	return spawn(model,
		Transform(glm::vec3(x + model_width, y + model_height, layer), glm::vec3(model_width, model_height, 1.0f)));
}

void SceneState::render_scene(Camera camera)
{
	physics.step();
	auto chunks = entities.object_table.size();

	// Renders all entities which have in-use ids
	for (int i = entities.id_list[0]; i < entities.first_free_index; i++){

			auto entity = entities.object_table[i / entities.chunk_size][i % entities.chunk_size];

			auto mvp = camera.perspective_projection(45.0f, 1080.0f/720.0f, 0.10f)
					 * camera.view_matrix()
					 * entity.transform.model_matrix();

			entity.model->render(mvp);
	}
}
