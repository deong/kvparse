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

#ifndef _KVPARSE_EXCEPT_H_
#define _KVPARSE_EXCEPT_H_

#include <stdexcept>
#include <string>

/*!
 * \class missing_keyword
 * \brief exception type for required keyword that is not specified
 */
class missing_keyword_error : public std::runtime_error 
{
public:
	missing_keyword_error(const std::string& msg) :
		std::runtime_error(msg) {
	}
};


/*!
 * \class duplicate_values
 * \brief exception type for requesting scalar value with multiple values
 */
class ambiguous_keyword_error : public std::runtime_error 
{
public:
	ambiguous_keyword_error(const std::string& msg) :
		std::runtime_error(msg)
		{
		}
};


/*!
 * \class illegal_value_error
 * \brief exception thrown for invalid value assigned to a given keyword
 */
class illegal_value_error : public std::runtime_error 
{
public:
	illegal_value_error(const std::string& msg) :
		std::runtime_error(msg)
		{
		}
};

/*!
 * \class syntax_error
 * \brief exception thrown for parse errors in the data file
 */
class syntax_error : public std::runtime_error
{
public:
	syntax_error(const std::string& msg) :
		std::runtime_error(msg)
		{
		}
};

#endif
