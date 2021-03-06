kvparse
=========

Efficient, type-safe template-driven parser for simple INI-like configuration files.


# Requirements

Requires [Boost.Regex](http://www.boost.org/). 

Building and running the tests requires the [gtest](https://code.google.com/p/googletest) unit testing library.


# Installation

kvparse is simply a handful of source files. Just include kvparse.h, kvparse.cpp, and kvparse_except.h into your project and go.

If you wish to run the unit tests, simply type "make" from the checkout directory, and then run the generated "run_tests" program. Note that currently some failures are expected and correspond to corner cases that have not yet been implemented. See the section at the end of this document for known issues.

Note that the Makefile used to generate the tests is extremely simple, but it doesn't make any attempt to guess the correct setup for your system, so you may need to edit it to specify the location of the boost and gtest libraries. In particular, the boost regex library is sometimes installed as "libboost_regex-mt" rather than "libboost_regex", so you may need to handle this variation for your system.

kvparse should be quite portable, but has been tested primarily on Linux and Mac OS X under gcc-4.8.


# Usage

## Reading configuration files

kvparse's structure is based on a singleton object that is statically available throughout the code that uses it. Sometime just after your program starts, call

    kvparse::read_configuration_file("filename.cfg");

one or more times. If you read multiple files through multiple calls, they behave as though they were concatenated into a single file and loaded. Ordering is preserved.


## Retrieving parameter values

Then from any location in the code, you can retrieve the value of a parameter using the kvparse::parameter_value functions.

    int an_integer;
    kvparse::parameter_value("keyword1", an_integer);

kvparse uses template specialization to select the correct specialization based on the type of the second argument. In the above example, an_integer is declared to be an int, and thus the function called is

    template <>
    bool kvparse::parameter_value(const string& keyword, int& value, bool required=false);

The currently supported types are

* int
* unsigned int
* unsigned long
* double
* bool
* string
* list<T>
* vector<T>

For the list and vector versions, the template parameter T may be any of the supported scalar types.

### Optional vs. required parameters

By default, calling `parameter_value("keyword", value)` will throw an exception  of type `missing_keyword_error` if "keyword" is not specified in at least one of the loaded configuration files. The function takes an optional third argument which can be used to suppress this behavior.

    int x = 42;
    kvparse::parameter_value("keyword", x, false);

will set x to whatever value was specified in the file if "keyword" exists, but will silently return without modifying the value of x if "keyword" is not specified.


## Other methods

* `void kvparse::clear()` -- deletes all read configuration information
* `bool kvparse::keyword_exists(const string& keyword)` -- checks to see if a keyword has been specified
* `bool kvparse::has_unique_value(const string& keyword)` -- checks to see if a keyword has exactly one associated value
* `void kvparse::dump_contents(ostream& ostr)` -- writes out the read configuration information for debugging


## Exceptions 

Aside from the `missing_keyword_error`, kvparse will throw a few other exceptions. 

* If there is a syntax error in the configuration file, it throws a simple `std::runtime_error` with the file and line number of the error. 
* If you ask for a scalar value type for a keyword that has been repeated in the configuration files, kvparse will throw an `ambiguous_keyword_error`. Keywords may be repeated, but if so you must use one of the vector or list specializations to fetch the associated values.
* kvparse does some very basic checks on the types requested and will throw an `illegal_value_error` if you violate these checks in a configuration file. Currently the checks performed are
* ints must be specified as [+-]?\d+
* unsigned ints or unsigned longs must be \+?\d+
* doubles must be [+-]?\d*\.?\d*  -- note this allows forms like "-." or just ".", which isn't perfect.
* bools must be one of "yes", "YES", "no", "NO", "true", "TRUE", "false", "FALSE", 0, or 1.


# Configuration file format

The format of files expected by kvparse is very simple. The basic structure is simply keyword/value pairs. Each pair must be specified on a single line. The delimiter may be either ":" or "=" characters. Anything from "#" to the end of the line is considered to be a comment. Whitespace is generally ignored, but whitespace within a keyword is a syntax error, and whitespace within a value is treated as a vector or list. A sample is shown below.

    # this is a sample configuration file
    encoding: permutation
    fitness_type: unsigned_long
    randomizer: permutation

    optimizer: simple_ga
    population_size: 100
    selection_operator: tournament
    tournament_size: 2
    crossover_operator: cycle
    crossover_rate: 0.95
    replacement_operator: steady_state
    mutation_operator: swap
    mutation_rate: 0.025

    problem: qap
    problem_data: ../prob/qap/gar60uni1.dat   # path to datafile 

    comparator: single_objective
    objective_number: 0
    reference_point: 0 0     # reference point can be read as a vector<int> directly
    terminator: evaluation_limit
    max_evaluations: 100000

    # note that repeating keywords is allowed; this creates a list of values associated with the keyword "metric"
    metric: evaluation_counter
    metric: generation_counter
    metric: best_found

    trials: 1
    processors: 4


# Known issues

* Quotes are currently not handled well. kvparse makes some attempt to handle quotes intelligently (e.g., you can quote an integer value and retrieve it as an int, strings quoted in a configuration file are not returned with quotes surrounding them when fetched), but this support is currently incomplete and a bit buggy.
* The checking for illegal values for a given type is also incomplete, but most of the most useful cases are handled. The missing bits are mostly corner cases.
* You must ensure that your configuration files end in a newline, else the last line will be ignored.
* The tests are incomplete for the time being, but this is being actively worked on.
