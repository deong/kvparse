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
#include "boost/regex.hpp"

using std::string;
using std::list;
using std::vector;
using std::ostream;
using std::map;
using std::istringstream;

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

    // disable construction/copying
    parsimony();
    parsimony(const parsimony&);
    parsimony &operator=(const parsimony&);

    //! provide operator to convert from strings to Ts
    template <typename T>
    static T from_string(const string& val);

    static int add_value(const string &keyword,const string &value);
    static int remove_value(const string &keyword,const string &value);
    static list<string> values(const string &keyword);
    static string value(const string &keyword);

public:
    static void clear();
    static bool read_configuration_file(const string &fileName);
    static bool keyword_exists(const string &keyword);
    static bool has_unique_value(const string &keyword);
    static void dump_contents(ostream &ostr);

    template <typename T>
    static inline bool parameter_value(const string& keyword, T& value, bool required=true);

    template <typename T>
    static inline bool parameter_value(const string& keyword, vector<T>& value, bool required=true);

    template <typename T>
    static inline bool parameter_value(const string& keyword, list<T>& value, bool required=true);
};

/*!
 * \brief get the primary value as a string
 *
 * Double quotes are handled specially. If the string begins and ends with quotes, they
 * are removed. Otherwise, they are preserved.
 */
template <>
inline bool parsimony::parameter_value<string>(const string &keyword, string& res, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return false;
        }
    }

    if(!has_unique_value(keyword)) {
        throw ambiguous_keyword_error("keyword '"+keyword+"' is ambiguous; multiple values");
    }

    res=value(keyword);
	if(res.size() >= 1 && res[0] == '"' && res[res.size()-1] == '"') {
		res = res.substr(1, res.size()-2);
		
	}
    return true;
}

/*!
 * \brief get the primary value as an integer
 */
template <>
inline bool parsimony::parameter_value<int>(const string& keyword, int& res, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return false;
        }
    }
    if(!has_unique_value(keyword)) {
        throw ambiguous_keyword_error("keyword '"+keyword+"' is ambiguous; multiple values");
    }

	boost::regex int_check("[-+]?\\d+");
	if(!boost::regex_match(value(keyword), int_check)) {
		throw illegal_value_error(keyword);
	} else {
		res=atoi(value(keyword).c_str());
	}
	
    return true;
}

/*!
 * \brief get the primary value as an unsigned integer
 */
template <>
inline bool parsimony::parameter_value<unsigned int>(const string& keyword, unsigned int& res, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return false;
        }
    }
    if(!has_unique_value(keyword)) {
        throw ambiguous_keyword_error("keyword '"+keyword+"' is ambiguous; multiple values");
    }

	boost::regex uint_check("\\+?\\d+");
	if(!boost::regex_match(value(keyword), uint_check)) {
		throw illegal_value_error(keyword);
	}

    string temp=value(keyword);
	res = (unsigned int)atoi(temp.c_str());
    return true;
}

/*!
 * \brief get the primary value as an unsigned long
 */
template <>
inline bool parsimony::parameter_value<unsigned long>(const string& keyword, unsigned long& res, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return false;
        }
    }
    if(!has_unique_value(keyword)) {
        throw ambiguous_keyword_error("keyword '"+keyword+"' is ambiguous; multiple values");
    }

	boost::regex uint_check("\\+?\\d+");
	if(!boost::regex_match(value(keyword), uint_check)) {
		throw illegal_value_error(keyword);
	}

    string temp=value(keyword);
	res = (unsigned long)atol(temp.c_str());
    return true;
}

/*!
 * \brief get the primary value as a double
 */
template <>
inline bool parsimony::parameter_value<double>(const string& keyword, double& res, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return false;
        }
    }
    if(!has_unique_value(keyword)) {
        throw ambiguous_keyword_error("keyword '"+keyword+"' is ambiguous; multiple values");
    }

	boost::regex double_check("[-+]?\\d*\\.?\\d*");
	if(!boost::regex_match(value(keyword), double_check)) {
		throw illegal_value_error(keyword);
	}

    string temp=value(keyword);
    res=atof(temp.c_str());
    return true;
}

/*!
 * \brief get the primary value as a bool
 */
template <>
inline bool parsimony::parameter_value<bool>(const string& keyword, bool& res, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return false;
        }
    }
    if(!has_unique_value(keyword)) {
        throw ambiguous_keyword_error("keyword '"+keyword+"' is ambiguous; multiple values");
    }

    string temp = value(keyword);
    if(temp == "true" || temp == "yes" || temp == "TRUE" || temp == "YES" || temp == "1") {
        res = true;
    } else if(temp == "false" || temp == "no" || temp == "FALSE" || temp == "NO" || temp == "0") {
        res = false;
    } else {
		throw illegal_value_error("illegal value for keyword '"+keyword+"' specified. Must be one of 'yes','true','no','false','0','1'");
    }
    return true;
}

/*!
 * \brief retrieve parameter values as a list of the specified type
 */
template <typename T>
inline bool parsimony::parameter_value(const string& keyword, list<T>& res, bool required)
{
    if(keyword_exists(keyword)) {
        list<string> vals = values(keyword);
        res.clear();
        for(list<string>::iterator it=vals.begin(); it!=vals.end(); ++it) {
			res.push_back(from_string<T>(*it));
		}
        return true;
    } else {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return true;
        }
    }
    return false;
}

/*!
 * \brief retrieve parameter values as a vector of a specified type
 */
template <class T>
inline bool parsimony::parameter_value(const string& keyword, vector<T>& v, bool required)
{
    if(!keyword_exists(keyword)) {
        if(required) {
            throw missing_keyword_error("required keyword '"+keyword+"' not specified");
        } else {
            return true;
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
 * \brief convert from strings to integers
 */
template <>
inline int parsimony::from_string<int>(const string& val)
{
    return atoi(val.c_str());
}

/*!
 * \brief convert from strings to doubles
 */
template <>
inline double parsimony::from_string<double>(const string& val)
{
    return static_cast<double>(atof(val.c_str()));
}

/*!
 * \brief dummy converter from strings to strings
 */
template <>
inline string parsimony::from_string<string>(const string& val)
{
    return val;
}

#endif
