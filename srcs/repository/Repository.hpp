//
// Created by a19828899 on 06.02.2022.
//

#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

#include "../interfaces/ICrudRepository.hpp"
#include <iostream>
#include <map>

using std::map;

class Repository : public ICrudRepository {
private:
	map<int, IEntity*> entities;

public:
	Repository();

	~Repository();

	void save(int id, IEntity* entity);

	IEntity* findById(int id);

	void deleteById(int id);

private:
	Repository(const Repository&);

	Repository& operator=(const Repository&);
};

#endif
