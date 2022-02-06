//
// Created by a19828899 on 06.02.2022.
//

#ifndef WEBSERV_REPOSITORY_HPP
#define WEBSERV_REPOSITORY_HPP

#include "../interfaces/ICrudRepository.hpp"


class Repository : public ICrudRepository {
public:
    void save(IEntity* entity);
    void deleteById(int id);
};


#endif //WEBSERV_REPOSITORY_HPP
