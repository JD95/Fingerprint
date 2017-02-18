#include "SceneState.h"




SceneState::SceneState()
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

void SceneState::render_scene(Camera camera)
{
	auto chunks = entities.object_table.size();

	// Renders all entities which have in-use ids
	for (int i = entities.id_list[0]; i < entities.first_free_index; i++){

			auto entity = entities.object_table[i / entities.chunk_size][i % entities.chunk_size];

			auto mvp = camera.perspective_projection(45.0f, 1.0f, 0.5f)
					 * camera.view_matrix()
					 * entity.transform.model_matrix();

			entity.model->render(mvp);
	}
}