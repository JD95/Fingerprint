#pragma once

#include <vector>
#include <algorithm>

struct object_id {
	long version;
	long index;

	friend bool operator==(const object_id& a, const object_id& b) {
		return (a.index == b.index)
			&& (a.version == b.version);
	}
};

/*!
	An implementation of a SlotMap based on:
	http://seanmiddleditch.com/data-structures-for-game-developers-the-slot-map/
*/
template <class T>
struct SlotMap
{
	static constexpr size_t chunk_size = 256;

	/*!
		A vector which points to array chunks of entities.
	*/
	std::vector<T*> object_table;

	/*!
		A list of ids which contains two halves: in use and free ids.

		| In Use		| Free			   |
		| ------------: | ---------------: |
		|0,1,2,3,4,5,6,7|8,9,10,11,12,13,14|

		When an object is created, first_free_index is incremented.

		| In Use		  | Free		   |
		| --------------: | -------------: |
		|0,1,2,3,4,5,6,7,8|9,10,11,12,13,14|

		When an object is destroyed it's id is then swapped with the largest
		in use id and first_free_index is decremented.

		Eg. `destroy_object(5)`

		The active id is moved to the end

		| In Use		  | Free		   |
		| --------------: | -------------: |
		|0,1,2,3,4,8,6,7,5|9,10,11,12,13,14|
		
		first_free_index is decremented

		| In Use		| Free			   |
		| ------------: | ---------------: |
		|0,1,2,3,4,8,6,7|5,9,10,11,12,13,14|

		The in-use portion is sorted to allow for binary searches

		| In Use		| Free			   |
		| ------------: | ---------------: |
		|0,1,2,3,4,6,7,8|5,9,10,11,12,13,14|

	*/
	std::vector<int> id_list;

	int first_free_index; /**< Divides the free ids from the in use ones.*/

	SlotMap() 
		: first_free_index(0) {}

	~SlotMap() {}

	/*!
		Returns the last free id.
	*/
	int last_free_id() { return (object_table.size() * chunk_size) - 1; }

	/*!
		Checks if an id is free or not.
	*/
	bool is_free_id(int id) { return std::find(id_list.begin(), id_list.end(),(id)) != id_list.end(); }

	/*!
		Assigns an object from the pool to an id
		and returns a pointer to that object;
	*/
	object_id SlotMap::create_object() {
		if (id_list.empty()) {
			T* chunk = new T[chunk_size];
			for (int i = 0; i < chunk_size; i++) {
				chunk[i].id.version = 1;
				chunk[i].id.index = object_table.size() * chunk_size + i;
				id_list.push_back(object_table.size() * chunk_size + i);
			}
			object_table.push_back(chunk);
		}

		if (first_free_index > 1) {
			// Keep in use section sorted
			auto x = id_list[first_free_index-2];
			auto y = id_list[first_free_index-1];
			if (x > y) {
				std::sort(id_list.begin(), id_list.begin() + first_free_index);
			}
		}
		
		auto free = first_free_index;
		first_free_index++;

		return object_table[free / chunk_size][free % chunk_size].id;
	}

	/*!
		Attempts to retrieve an id from the pool.
		If the id's version does not match the stored
		object, then it has been destroyed and will
		return nullptr.
	*/
	T* SlotMap::get_object(object_id id) {
		T* obj = object_table[id.index / chunk_size] + (id.index % chunk_size);
		return obj->id.version != id.version ? nullptr : obj;
	}

	/*!
		Frees an object to be recycled by in the pool.
	*/
	void SlotMap::destroy_object(object_id id) {
		T* obj = get_object(id);
		
		obj->id.version += 1;

		auto active_id = std::lower_bound(id_list.begin(), id_list.begin() + first_free_index, obj->id.index);

		std::rotate(active_id, active_id + 1, id_list.begin() + first_free_index);
		
		first_free_index--;
	}
};

