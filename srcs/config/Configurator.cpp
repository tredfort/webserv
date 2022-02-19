//
// Created by a19828899 on 06.02.2022.
//

#include "Configurator.hpp"

Config* Configurator::createConfig(string path)
{
	if (path.empty())
		NULL;
	return new Config();
}
