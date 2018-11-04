#ifndef __TYPE_MAP_H__
#define __TYPE_MAP_H__

#include "util.h"

namespace Kiwi {

/**
 * A class containing mappings of String to type_index and
 * type_index to String.
 */
class TypeMap {
public:
	TypeMap();

	/**
	 * Gets the String representing the type indicdated by the key
	 * argument. If no type corresponding to the key argument has
	 * been registered, an out_of_range exception is thrown.
	 * 
	 * @param  key the type to return the String reference of
	 * @return     a reference to the String representing the type
	 * @throw  std::out_of_range if the key does not exist
	 */
	const String &get(const std::type_index &key);

	/**
	 * Register the type_index to String pair in the TypeMap.
	 */
	void put(const std::type_index &key, const String &value);

	/**
	 * Gets the type_index corresponding to the String indicdated
	 * by the key argument. If no String matching the key has been
	 * registered, an out_of_range exception is thrown.
	 * 
	 * @param  key the type to return the String reference of
	 * @return     a reference to the String representing the type
	 * @throw  std::out_of_range if the key does not exist
	 */
	const std::type_index &get(const String &key);

	/**
	 * Register the String to type_index pair in the TypeMap.
	 */
	void put(const String &key, const std::type_index &value);

private:
	std::unordered_map<std::type_index, String> typeToString;
	std::unordered_map<String, std::type_index> stringToType;
};

}

#endif // __TYPE_MAP_H__