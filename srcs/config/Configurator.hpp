//
// Created by a19828899 on 06.02.2022.
//

#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include "Config.hpp"
#include <iostream>

using std::string;

class Configurator {
public:
	Config* createConfig(string path);
};

#endif
