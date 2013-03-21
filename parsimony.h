// Copyright 2013 Deon Garrett <deon@iiim.is>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _PARSIMONY_H_
#define _PARSIMONY_H_

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "parsimony_except.h"

using namespace std;

/*!
 * \class parsimony
 *
 * Implements an option=value parser for simple INI-like configuration
 * files.
 *
 * Uses templates heavily to provide type-safe access to the values.
 */
class parsimony
{
private:
    // my current collection of configuration parameters,
    // represented as keyword,value pairs.
    static map<string,list<string> > db_;

    // disable copying for singleton classes
    parsimony();
    parsimony(const parsimony&);
    parsimony &operator=(const parsimony&);

public:
    virtual ~parsimony();

    static void clear();
    static bool read_configuration_file(const string &fileName);
    static int add_value(const string &keyword,const string &value);
    static int remove_value(const string &keyword,const string &value);
    static bool keyword_exists(const string &keyword);
    static bool has_unique_value(const string &keyword);
    static list<string> values(const string &keyword);
    static string value(const string &keyword);

    static bool string_parameter(const string &keyword, string& value, bool required = false);
    static bool integer_parameter(const string &keyword, int& value, bool required = false);
    static bool unsigned_integer_parameter(const string &keyword, unsigned int& value, bool required = false);
    static bool double_parameter(const string &keyword, double& value, bool required = false);
    static bool boolean_parameter(const string& keyword, bool& value, bool required = false);
    static bool list_parameter(const string& keyword, list<string>& value, bool required = false);

    /*!
     * \brief retrieve parameter values as a vector of a specified type
     * \param keyword the option name
     * \param v output parameter to hold the retrieved option values
     * \param required (optional) whether the keyword is required
     */
    template <class T>
    static bool vector_parameter(const string& keyword, vector<T>& v, bool required = false) {
        if(!keyword_exists(keyword)) {
            if(required) {
                throw missing_keyword_error(keyword);
            } else {
				return false;
            }
        } else {
            v.clear();
            string vecvals = value(keyword);
            istringstream istr(vecvals);
            while(!istr.eof()) {
                T x;
                istr >> x;
                v.push_back(x);
            }
        }
        return true;
    }

	/*!
	 * \brief print the entire collection of options and values for debugging
	 */
    static void dump_contents(ostream &ostr);
};

#endif
