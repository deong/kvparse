/*!
 * \file parsimony.cpp
 *
 * Stores the configuration information for a given run of the GA.
 * Uses a singleton pattern to control access to a single, statically
 * available hash table mapping keywords to values.
 *
 * Deon Garrett
 * University of Memphis
 * deong@acm.org
 *
 */

#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <regex>
#include "parsimony.h"
#include "parsimony_except.h"

using namespace std;

//! stores the internal configuration data
map<string,list<string> > parsimony::db_;

/*!
 * \brief constructor
 */
parsimony::parsimony()
{
}

/*!
 * \brief destructor
 *
 * \author deong
 * \date 05/08/2007
 *
 * \code
 * Modification History
 * MM/DD/YYYY	DESCRIPTION
 * \endcode
 */
parsimony::~parsimony()
{
    // no dynamic memory to worry about
}

/*!
 * \brief erase all stored configuration data
 *
 * \author deong
 * \date 05/08/2007
 *
 * \code
 * Modification History
 * MM/DD/YYYY	DESCRIPTION
 * \endcode
 */
void parsimony::clear()
{
    db_.erase(db_.begin(), db_.end());
}

/*!
 * \brief parse a given configuration file
 *
 * \author deong
 * \date 05/08/2007
 *
 * \code
 * Modification History
 * MM/DD/YYYY	DESCRIPTION
 * \endcode
 */
bool parsimony::read_configuration_file(const string& filename)
{
    ifstream in(filename.c_str());
    if(!in) {
        throw runtime_error("failed to open configuration file: " + filename);
    }

    //bool lengthFound = false;
    int lineno=0;
    string line;
    while(!getline(in, line).eof()) {
        // update the line number
        lineno++;

        // remove any comments
        int hashpos = (int)line.find("#", 0);
        if(hashpos != (int)string::npos) {
            line = line.erase(hashpos);
        }

        // if the line is (now) blank, just go to the next line
        if(line == "") {
            continue;
        }
      
        // parse the line into keyword and values
        // first, find the delimiter
        int delimiterpos = (int)line.find(":", 0);

        // if there was no ":" delimiter, try an "="
        if(delimiterpos == (int)string::npos) {
            delimiterpos = (int)line.find("=", 0);
        }

        // if still no delimiter, declare an error
        if(delimiterpos == (int)string::npos) {
            ostringstream mystr;
            mystr << "syntax error in " << filename << " (" << lineno <<  "): "
                  << line << endl;
            throw runtime_error(mystr.str());
        }

        // now we have the left hand side and right hand side
        string thekeyword = line.substr(0, delimiterpos);
        string thevalue   = line.substr(delimiterpos+1);

        // trim any leading or trailing spaces from the keyword
        int first_non_space;
        int last_non_space;
        first_non_space = (int)thekeyword.find_first_not_of(" \t");
        last_non_space = (int)thekeyword.find_last_not_of(" \t");
        int tokenlen = last_non_space - first_non_space + 1;
        thekeyword = thekeyword.substr(first_non_space, tokenlen);

        // trim any leading or trailing spaces from the value
        first_non_space = (int)thevalue.find_first_not_of(" \t");
        last_non_space = (int)thevalue.find_last_not_of(" \t");
        tokenlen = last_non_space - first_non_space + 1;
        thevalue = thevalue.substr(first_non_space, tokenlen);

        // add the mapping to the database
        add_value(thekeyword, thevalue);
    }
    return true;
}

/*!
 * \brief add a new keyword/value pair
 *
 * if keyword already exists, add the value onto the keyword's list
 */
int parsimony::add_value(const string &keyword,const string &value)
{
    if(!keyword_exists(keyword)) {
        list<string> valueList;
        valueList.push_back(value);
        db_[keyword]=valueList;
        return 1;
    } else {
        map<string,list<string> >::iterator mapIter;
        mapIter=db_.find(keyword);      
        ((*mapIter).second).push_back(value);
        return (int)((*mapIter).second).size();
    }
}

/*!
 * \brief remove a given keyword/value pair
 *
 * if the keyword does not exist, do nothing.  if removing the
 * value from the keyword results in an empty value list, remove
 * the keyword entry from the database
 */
int parsimony::remove_value(const string &keyword,const string &value)
{
    assert(keyword_exists(keyword));

    map<string,list<string> >::iterator mapIter;
    mapIter=db_.find(keyword);

    list<string> &valueList=(*mapIter).second;
    list<string>::iterator valueIter;

    valueIter=find(valueList.begin(),valueList.end(),value);
    if(valueIter==valueList.end()) {
        return 0;
    }
    (void)valueList.erase(valueIter);

    if(valueList.size()==0) {
        db_.erase(mapIter);
        return 0;
    } else {
        return (int)valueList.size();
    }
}

/*!
 * \brief check to see if a keyword is in the database
 */
bool parsimony::keyword_exists(const string &keyword)
{
    map<string,list<string> >::const_iterator iter;
    iter=db_.find(keyword);
    if(iter==db_.end()) {
        return false;
    }
    return true;
}

/*!
 * \brief check to see if a keyword is mapped to a single unique value
 */
bool parsimony::has_unique_value(const string &keyword)
{
    assert(keyword_exists(keyword));

    map<string,list<string> >::const_iterator iter;
    iter=db_.find(keyword);

    const list<string> &valueList=(*iter).second;
    if(valueList.size()!=1) {
        return false;
    }

    return true;  
}

/*!
 * \brief return the list of values associated with a keyword
 */
list<string> parsimony::values(const string &keyword)
{
    assert(keyword_exists(keyword));

    map<string,list<string> >::const_iterator iter;
    iter=db_.find(keyword);

    return (*iter).second;
}

/*!
 * \brief return the first value associated with a keyword
 */
string parsimony::value(const string &keyword)
{
    assert(keyword_exists(keyword));

    map<string,list<string> >::const_iterator iter;
    iter=db_.find(keyword);

    const list<string> &values=(*iter).second;

    if(values.size()!=1) {
        return string();
    }

    return values.front();
}

/*!
 * \brief get the primary value as a string
 */
bool parsimony::string_parameter(const string &keyword, string& res, bool required)
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
    return true;
}

/*!
 * \brief get the primary value as an integer
 */
bool parsimony::integer_parameter(const string& keyword, int& res, bool required)
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

	// regex int_check("[-+]?\\d+");
	// if(!regex_match(value(keyword), int_check)) {
	// 	throw illegal_value_error(keyword);
	// } else {
		res=atoi(value(keyword).c_str());
	// }
	
    return true;
}

/*!
 * \brief get the primary value as an unsigned integer
 */
bool parsimony::unsigned_integer_parameter(const string& keyword, unsigned int& res, bool required)
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

    string temp=value(keyword);
    res=(unsigned int)atoi(temp.c_str());
    return true;
}

/*!
 * \brief get the primary value as a double
 */
bool parsimony::double_parameter(const string& keyword, double& res, bool required)
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

    string temp=value(keyword);
    res=atof(temp.c_str());
    return true;
}

/*!
 * \brief get the primary value as a bool
 */
bool parsimony::boolean_parameter(const string& keyword, bool& res, bool required)
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
    if(temp == "true" || temp == "yes") {
        res = true;
    } else if(temp == "false" || temp == "no") {
        res = true;
    } else {
		throw illegal_value_error("illegal value for keyword '"+keyword+"' specified. Must be one of 'yes','true','no','false'");
    }
    return true;
}

/*!
 * \brief get the list of all values (as strings)
 */
bool parsimony::list_parameter(const string& keyword, list<string>& res, bool required)
{
    if(keyword_exists(keyword)) {
        res = parsimony::values(keyword);
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
 * \brief display the contents of the configuration database
 */
void parsimony::dump_contents(ostream &ostr)
{
    map<string,list<string> >::const_iterator mapIter;  
    for(mapIter=db_.begin(); mapIter!=db_.end(); mapIter++)
    {
        ostr << "Keyword: " << (*mapIter).first << "  |  ";
        const list<string> &values=(*mapIter).second;
        list<string>::const_iterator valueIter;
        ostr << "Values: ";
        for(valueIter=values.begin();
            valueIter!=values.end();
            valueIter++)
        {
            ostr << *valueIter << " ";
        }
        ostr << endl;
    }
}

