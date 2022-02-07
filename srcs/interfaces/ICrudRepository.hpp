//
// Created by a19828899 on 06.02.2022.
//

#ifndef ICRUDREPOSITORY_HPP
#define ICRUDREPOSITORY_HPP

#include "IEntity.hpp"

class ICrudRepository {
public:
    virtual void save(int id, IEntity *entity) = 0;

    virtual IEntity *findById(int id) = 0;

    virtual void deleteById(int id) = 0;

    virtual ~ICrudRepository() {};
};

#endif
