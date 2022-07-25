# JSON Encoding Library (JEL)
Library used to encode file in JSON format to TLV (Tag-Lengh-Value)

# Encode algorithm
The input will contain records in JSON format. Each record is separated by a new line (LF). The input will only contain simple types, complex and nested data structures will not be present (i.e. map and array).

Input example:
{"key1":"value", "key2":42, "key3":true}
{"sadsf":"dsewtew", "dsre":3221, "sdfds":"dsfewew"}

Basically a record is a list of KVPs (key-value pairs).

The library encodes the input so that the keys are put into a dictionary. The dictionary maps the keys (strings) to an integer. The key strings are then replaced with a number.

The dictionary would be as follows for the first record:
"key1":1, "key2":2, "key3":3
This record is then represented as:
{1:"value", 2:42, 3:true}

Types supported by JSON must be handled and preserved (i.e. integer, string, bool).  The output will be dumped to a file in a binary form using TLV encoding instead of the above text-based representation. This binary TLV encoding is more compact, e.g. a bool value can be encoded in two bytes (or even less).
 At the end of the process the dictionary must be also written to a file using the same encoding method for the string-integer pairs.

The input file can be arbitrary large, make sure to use streaming instead of reading everything into memory. You can assume that the key space (number of distinct key strings) is small and regular in-memory structures (i.e. hashtable) can be used for the dictionary.


# Further Improvements

 - Add decoding support
 - Add complex type support
 - Enhance error logging by using messages from the helper libraries
 - Add big/little endian support
 - Add in-place TLV construction
 - Add unit tests for other TLV tags (similar to what is done for JEL_TLV_ELEMENT_BOOL)
 - Add unit tests for encoding functions.

# Environment setup
Following tools are required for successful build and execution of the JEL library.

 - GCC and G++.
 - CMake (version 3.18 or higher)
 - GoogleTest
 - doxygen

# External Dependencies
In JEL following external libraries are used:
 - Apache Portable Runtime v1.7 (https://apr.apache.org/).
Used to work with hashtables and file IO.
 - YAJL v2.1.1 (https://github.com/lloyd/yajl)
Used for parsing JSON files

Both these libraries are precompiled and stored under `external` folder in the repository.

# How To Build
 - To build the library with all tests and samples `./build.sh` command should be executed. 
 - To generate doxygen documentation `./doc_gen.sh` should be executed.
 - To clean all temporary generated file `./clean.sh` should be executed.

# How To Run
After succesful build:
 - `sample` console application will be available in `./build/test/sample` folder. It is expecting the input JSON entries to be provided in the `input` file next to it. And will generate the output in the `output` binary file.
 - `jel_test` console application will be available in `./build/test/gtes` folder. It will execute JEL unit tests.
