/*
    open source routing machine
    Copyright (C) Dennis Luxen, 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU AFFERO General Public License as published by
the Free Software Foundation; either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
or see http://www.gnu.org/licenses/agpl.txt.
 */

#ifndef BASECONFIGURATION_H_
#define BASECONFIGURATION_H_

#include "OSRMException.h"
#include "../DataStructures/HashTable.h"

#include <boost/algorithm/string.hpp>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

class BaseConfiguration {
public:
    BaseConfiguration(const char * configFile) {
        std::ifstream config( configFile );
        if(!config) {
            throw OSRMException("[config] .ini not found");
        }

        std::string line;
        if (config.is_open()) {
            while ( config.good() )  {
                getline (config,line);
                std::vector<std::string> tokens;
                Tokenize(line, tokens);
                if(2 == tokens.size() )
                    parameters.insert(std::make_pair(tokens[0], tokens[1]));
            }
            config.close();
        }
    }

    std::string GetParameter(const std::string & key){
         return parameters.Find(key);
    }

    void SetParameter(const char* key, const char* value) {
        SetParameter(std::string(key), std::string(value));
    }

    void SetParameter(const std::string key, std::string value) {
        parameters[key] = value;
    }

private:
    void Tokenize(
        const std::string& str,
        std::vector<std::string>& tokens,
        const std::string& delimiters = "="
    ) {
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

        while (std::string::npos != pos || std::string::npos != lastPos) {
            std::string temp = str.substr(lastPos, pos - lastPos);
            boost::trim(temp);
            tokens.push_back( temp );
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }

    HashTable<std::string, std::string> parameters;
};

#endif /* BASECONFIGURATION_H_ */
