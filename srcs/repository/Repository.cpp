//
// Created by a19828899 on 06.02.2022.
//

#include "Repository.hpp"

Repository::Repository() { }

Repository::~Repository() { }

void Repository::save(WebClient* client) { entities[client->getFd()] = client; }

WebClient* Repository::findById(int id)
{
	map<int, WebClient*>::iterator it = entities.find(id);
	if (it != entities.end())
		return it->second;
	return NULL;
}
