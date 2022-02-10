//
// Created by a19828899 on 06.02.2022.
//

#include "Repository.hpp"

Repository::Repository() {}

Repository::~Repository() {}

void Repository::save(int id, IEntity *entity) {
    entities[id] = entity;
}

IEntity *Repository::findById(int id) {
    map<int, IEntity *>::iterator it = entities.find(id);
    if (it != entities.end())
        return it->second;
    return NULL;
}

void Repository::deleteById(int id) {
    map<int, IEntity *>::iterator it = entities.find(id);
    if (it != entities.end()) {
        entities.erase(it);
        delete it->second;
    }
}
