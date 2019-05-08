#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "util.h"
#include "Core/Object.h"
#include <unordered_map>
#include <any>

namespace Kiwi::Seed {

class PropertyNames {
public:
	static const String Position;
	static const String Velocity;
	static const String Acceleration;
	static const String Force;
	static const String Mass;
	static const String Color;
	static const String Lifetime;
	static const String Valid;
};

/**
 * Manages the properties of all the particles in an emitter.
 * By default, the ParticleManager tracks these properties:
 *   - position	(Vector3)
 *   - velocity (Vector3)
 *   - acceleration (Vector3)
 *   - force (Vector3)
 *   - mass (float)
 *   - color (Vector4)
 *   - lifetime (float)
 *   - valid (bool)
 */
class ParticleManager : public Object {
public:
	ParticleManager(unsigned int maxParticles_);

	template <typename T>
	void addProperty(const String &name, T initialValue = T());

	template <typename T>
	void resetPropertyValues(const String &name, T resetValue = T());

	const Vector<String> &getPropertyNames() const;

	template <typename T>
	T get(const String &name, unsigned int p) const;

	template <typename T>
	void getActive(const String &name, Vector<T> &values) const;

	template <typename T>
	void set(const String &name, unsigned int p, T &value);

	// Convenience methods for setting/getting default properties
	Vector3 getPosition(unsigned int p) const;
	void setPosition(unsigned int p, Vector3 pos);

	Vector3 getVelocity(unsigned int p) const;
	void setVelocity(unsigned int p, Vector3 vel);

	Vector3 getAcceleration(unsigned int p) const;
	void setAcceleration(unsigned int p, Vector3 acc);

	Vector3 getForce(unsigned int p) const;
	void setForce(unsigned int p, Vector3 force);

	float getMass(unsigned int p) const;
	void setMass(unsigned int p, float mass);

	Vector4 getColor(unsigned int p) const;
	void setColor(unsigned int p, Vector4 color);

	float getLifetime(unsigned int p) const;
	void setLifetime(unsigned int p, float lifetime);

	bool isParticleValid(unsigned int p) const;
	void setParticleValid(unsigned int p, bool valid);

	/**
	 * Gets the next available particle index.
	 * @return the next available particle index, if there are
	 *         already the maximum number of particles allowed,
	 *         -1 is returned
	 */
	int spawnParticle();

	void despawnParticle(unsigned int i);

	unsigned int maxParticles;

	friend class Module;

private:
	std::unordered_map<String, Vector<std::any>> properties;
	mutable Vector<String> propertyNames;
	mutable bool namesCached;

	Vector<int> freeList;
	int firstAvailable;
};

template <typename T>
void ParticleManager::addProperty(const String &name, T initialValue) {
	properties.emplace(name, Vector<std::any>());
	Vector<std::any> &propVec = properties[name];
	propVec.resize(maxParticles);
	std::fill(propVec.begin(), propVec.end(), initialValue);
}

template <typename T>
void ParticleManager::resetPropertyValues(const String &name, T resetValue) {
	Vector<std::any> &propVec = properties[name];
	std::fill(propVec.begin(), propVec.end(), resetValue);
}

template <typename T>
T ParticleManager::get(const String &name, unsigned int p) const {
	if (p >= maxParticles) {
		throw new std::out_of_range("no particle of index " + std::to_string(p));
	}
	const Vector<std::any> &propVec = properties.at(name);
	const std::any &propAny = propVec[p];
	return std::any_cast<T>(propAny);
}

template <typename T>
void ParticleManager::getActive(const String &name, Vector<T> &values) const {
	//values.reserve(maxParticles);
	const Vector<std::any> &propVec = properties.at(name);

	for (unsigned int i = 0; i < maxParticles; ++i) {
		if (isParticleValid(i)) {
			values.push_back(std::any_cast<T>(propVec[i]));
		}
	}
}

template <typename T>
void ParticleManager::set(const String &name, unsigned int p, T &value) {
	if (p >= maxParticles) {
		throw new std::out_of_range("no particle of index " + std::to_string(p));
	}
	properties.at(name)[p] = value;
}

}

#endif // __PARTICLEMANAGER_H__