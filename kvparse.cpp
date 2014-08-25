/*!
 * \file kvparse.cpp
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
#include <exception>
#include <boost/regex.hpp>
#include "kvparse.h"
#include "kvparse_except.h"

using namespace std;

//! stores the internal configuration data
map<string,list<string> > kvparse::db_;

/*!
 * \brief erase all stored configuration data
 */
void kvparse::clear()
{
    db_.erase(db_.begin(), db_.end());
}

/*!
 * \brief parse a given configuration file
 * \param filename the name of the configuration file to parse
 * \return true -- throws exception on errors
 */
bool kvparse::read_configuration_file(const string& filename)
{
    ifstream in(filename.c_str());
    if(!in) {
        throw runtime_error("failed to open configuration file: " + filename);
    }

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
		boost::regex wsre("^[[:space:]]*$");
		if(boost::regex_match(line, wsre)) {
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
            throw syntax_error(mystr.str());
        }

		try {
			// now we have the left hand side and right hand side
			string thekeyword = line.substr(0, delimiterpos);
			string thevalue   = line.substr(delimiterpos+1);
			
			// trim any leading or trailing spaces from the keyword
			int first_non_space;
			int last_non_space;
			first_non_space = (int)thekeyword.find_first_not_of(" \r\t");
			last_non_space = (int)thekeyword.find_last_not_of(" \r\t");
			int tokenlen = last_non_space - first_non_space + 1;
			thekeyword = thekeyword.substr(first_non_space, tokenlen);
			
			// make sure the keyword has no illegal characters
			boost::regex re_identifier("^[A-Za-z_][A-Za-z0-9_.-]*'*");
			if(!boost::regex_match(thekeyword, re_identifier)) {
				throw syntax_error("syntax error");
			}
			
			// trim any leading or trailing spaces from the value
			first_non_space = (int)thevalue.find_first_not_of(" \r\t");
			last_non_space = (int)thevalue.find_last_not_of(" \r\t");
			tokenlen = last_non_space - first_non_space + 1;
			thevalue = thevalue.substr(first_non_space, tokenlen);
			
			// add the mapping to the database
			add_value(thekeyword, thevalue);
		} catch(exception&) {
			ostringstream mystr;
			mystr << "syntax error in " << filename << " (" << lineno << "): "
				  << line << endl;
			throw syntax_error(mystr.str());
		}
    }
    return true;
}

/*!
 * \brief add a new keyword/value pair
 * \param keyword
 * \param value
 * 
 * if keyword already exists, add the value onto the keyword's list
 *
 * Note that all values are stored as strings. Type conversion is done
 * on requesting a value.
 */
int kvparse::add_value(const string &keyword, const string &value)
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
 * \param keyword
 * \param value
 * \return 0 if the updated database does not contain a value for keyword, 1 if it does
 *
 * if the keyword does not exist, do nothing. if removing the value
 * from the keyword results in an empty value list, remove the keyword
 * entry from the database
 */
int kvparse::remove_value(const string &keyword,const string &value)
{
	if(!keyword_exists(keyword)) {
		return 0;
	}
	
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
 * \param keyword
 * \return true if the keyword exists in the database, false otherwise
 */
bool kvparse::keyword_exists(const string &keyword)
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
 * \param keyword
 * \return true if the keyword exists and has a single specified value; false otherwise
 */
bool kvparse::has_unique_value(const string &keyword)
{
    if(!keyword_exists(keyword)) {
		return false;
	}
	
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
 * \param keyword
 * \return the list of values associated with the keyword
 *
 * The keyword must exist in the database.
 */
list<string> kvparse::values(const string &keyword)
{
    assert(keyword_exists(keyword));

    map<string,list<string> >::const_iterator iter;
    iter=db_.find(keyword);

    return (*iter).second;
}

/*!
 * \brief return the first value associated with a keyword
 * \param keyword
 * \return the first value associated with a keyword as a string
 *
 * The keyword must exist in the database.
 */
string kvparse::value(const string &keyword)
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
 * \brief display the contents of the configuration database
 */
void kvparse::dump_contents(ostream &ostr)
{
    map<string,list<string> >::const_iterator mapIter;  
    for(mapIter=db_.begin(); mapIter!=db_.end(); mapIter++) {
        ostr << "Keyword: " << (*mapIter).first << "  |  ";
        const list<string> &values=(*mapIter).second;
        list<string>::const_iterator valueIter;
        ostr << "Values: ";
        for(valueIter=values.begin(); valueIter!=values.end(); valueIter++) {
            ostr << *valueIter << " ";
        }
        ostr << endl;
    }
}

