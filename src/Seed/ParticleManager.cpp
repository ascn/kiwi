#include "Seed/ParticleManager.h"

#include <algorithm>

namespace Kiwi::Seed {

const String PropertyNames::Position = "position";
const String PropertyNames::Velocity = "velocity";
const String PropertyNames::Acceleration = "acceleration";
const String PropertyNames::Force = "force";
const String PropertyNames::Mass = "mass";
const String PropertyNames::Color = "color";
const String PropertyNames::Lifetime = "lifetime";
const String PropertyNames::Valid = "valid";

ParticleManager::ParticleManager(unsigned int maxParticles_) :
	maxParticles(maxParticles_),
	properties(),
	propertyNames(),
	namesCached(false),
	freeList(maxParticles_),
	firstAvailable(0)
{
	// Add default properties
	addProperty<Vector3>(PropertyNames::Position);
	addProperty<Vector3>(PropertyNames::Velocity);
	addProperty<Vector3>(PropertyNames::Acceleration);
	addProperty<Vector3>(PropertyNames::Force);
	addProperty<float>(PropertyNames::Mass, 1.f);
	addProperty<Vector4>(PropertyNames::Color, Vector4(1.f));
	addProperty<float>(PropertyNames::Lifetime);
	addProperty<bool>(PropertyNames::Valid, false);

	// Initialize free list
	for (int i = 0; i < maxParticles_ - 1; ++i) {
		freeList[i] = i + 1;
	}
	freeList[maxParticles_ - 1] = -1;
}

const Vector<String> &ParticleManager::getPropertyNames() const {
	if (!namesCached) {
		for (const auto &entry : properties) {
			propertyNames.push_back(entry.first);
		}
	}
	namesCached = true;
	return propertyNames;
}

Vector3 ParticleManager::getPosition(unsigned int p) const {
	return get<Vector3>(PropertyNames::Position, p);
}

void ParticleManager::setPosition(unsigned int p, Vector3 pos) {
	set<Vector3>(PropertyNames::Position, p, pos);
}

Vector3 ParticleManager::getVelocity(unsigned int p) const {
	return get<Vector3>(PropertyNames::Velocity, p);
}

void ParticleManager::setVelocity(unsigned int p, Vector3 vel) {
	set<Vector3>(PropertyNames::Velocity, p, vel);
}

Vector3 ParticleManager::getAcceleration(unsigned int p) const {
	return get<Vector3>(PropertyNames::Acceleration, p);
}

void ParticleManager::setAcceleration(unsigned int p, Vector3 acc) {
	set<Vector3>(PropertyNames::Acceleration, p, acc);
}

Vector3 ParticleManager::getForce(unsigned int p) const {
	return get<Vector3>(PropertyNames::Force, p);
}

void ParticleManager::setForce(unsigned int p, Vector3 force) {
	set<Vector3>(PropertyNames::Force, p, force);
}

float ParticleManager::getMass(unsigned int p) const {
	return get<float>(PropertyNames::Mass, p);
}

void ParticleManager::setMass(unsigned int p, float mass) {
	set<float>(PropertyNames::Mass, p, mass);
}

Vector4 ParticleManager::getColor(unsigned int p) const {
	return get<Vector4>(PropertyNames::Color, p);
}

void ParticleManager::setColor(unsigned int p, Vector4 color) {
	set<Vector4>(PropertyNames::Color, p, color);
}

float ParticleManager::getLifetime(unsigned int p) const {
	return get<float>(PropertyNames::Lifetime, p);
}

void ParticleManager::setLifetime(unsigned int p, float lifetime) {
	set<float>(PropertyNames::Lifetime, p, lifetime);
}

bool ParticleManager::isParticleValid(unsigned int p) const {
	if (p >= maxParticles) {
		return false;
	}
	try {
		return std::any_cast<bool>(properties.at(PropertyNames::Valid)[p]);
	} catch (std::bad_any_cast &) {
		return false;
	}
}

void ParticleManager::setParticleValid(unsigned int p, bool valid) {
	set<bool>(PropertyNames::Valid, p, valid);
}

int ParticleManager::spawnParticle() {
	int ret = firstAvailable;
	if (firstAvailable >= 0) {
		firstAvailable = freeList[firstAvailable];
	}
	return ret;
}

void ParticleManager::despawnParticle(unsigned int i) {
	setParticleValid(i, false);
	freeList[i] = firstAvailable;
	firstAvailable = i;
}

}