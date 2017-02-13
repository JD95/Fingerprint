#pragma once

#include <vector>
#include <algorithm>

typedef long long object_id;

/*!
	An implementation of a SlotMap based on:
	http://seanmiddleditch.com/data-structures-for-game-developers-the-slot-map/
*/
template <class T>
struct SlotMap
{
	static constexpr size_t chunk_size = 256;
	std::vector<T*> object_table;
	std::vector<int> free_list; /**< Contains list of free id's*/

	SlotMap() {}
	~SlotMap() {}

	/*!
		Returns the last free id.
	*/
	int last_free_id() { return (object_table.size() * chunk_size) - 1; }

	/*!
		Checks if an id is free or not.
	*/
	bool is_free_id(int id) { return std::find(free_list.begin(), free_list.end(),(id)) != free_list.end(); }

	/*!
		Assigns an object from the pool to an id
		and returns a pointer to that object;
	*/
	object_id SlotMap::create_object() {
		if (free_list.empty()) {
			T* chunk = new T[chunk_size];
			for (int i = chunk_size - 1; i >= 0; --i) {
				chunk[i].id = object_table.size() * chunk_size + i;
				free_list.push_back(object_table.size() * chunk_size + i);
			}
			object_table.push_back(chunk);
		}

		int free = free_list.back();
		free_list.pop_back();
		return object_table[free / chunk_size][free % chunk_size].id;
	}

	/*!
		Attempts to retrieve an id from the pool.
		If the id's version does not match the stored
		object, then it has been destroyed and will
		return nullptr.
	*/
	T* SlotMap::get_object(object_id id) {
		T* obj = object_table[(id & 0xFFFFFFFF) / chunk_size] + ((id & 0xFFFFFFFF) % chunk_size);
		return obj->id != id ? nullptr : obj;
	}

	/*!
		Frees an object to be recycled by in the pool.
	*/
	void SlotMap::destroy_object(object_id id) {
		T* obj = get_object(id);
		obj->id = (obj->id & 0xFFFFFFFF) | (((obj->id >> 32) + 1) << 32);
		free_list.push_back(id & 0xFFFFFFFF);
	}
};

