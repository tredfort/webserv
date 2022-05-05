//
// Created by a19828899 on 06.02.2022.
//

#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

#include <iostream>
#include <map>
#include "../model/WebClient.hpp"

using std::map;

class Repository {
private:
	map<int, WebClient*> entities;

public:
	Repository();

	~Repository();

	void save(WebClient* client);

	WebClient* findById(int id);

private:
	Repository(const Repository&);

	Repository& operator=(const Repository&);
};

#endif
