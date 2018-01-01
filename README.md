# xxGA
Genetic Algorithm implementation.

It is a GA implementation to minimize functions.

Makefile automatically created by Eclipse.


## Dependencies

Unit tests libcppunit-1.14 http://www.yolinux.com/TUTORIALS/CppUnit.html:

```
apt-get install libcppunit-doc libcppunit-dev
```

Logging using log4cxx 0.10 https://logging.apache.org/log4cxx/latest_stable/:

```
apt-get install liblog4cxx-dev 
```
OpenCL 1.2.

The project uses C++14:
https://gcc.gnu.org/gcc-6/changes.html

It was created with:
gcc (Debian 6.4.0-1) 6.4.0 20170704

## Compile

Project generated with eclipse, makefile is auto generated by eclipse. Be careful the binary that is into the release folder was compiled with the option -march=native so it is not portable to others machines.

```
cd Release
make
```

## Run
After compile.

```
xxGA  <conf_log_file> <conf_file>
```

The conf_file and the conf_log_file can be found into the root folder.

By default de program is configured to resolve a function optimization problem. The problem is configured into
its own file. The problem configuration file is specified into conf_file.

To change the problem the main.cpp must be changed and a new compilation is needed.

There are other two problem implemented but need to be activated programatically:
- Generic functions optimization:
This problem load function from its configuration file.
This http://warp.povusers.org/FunctionParser/ library was used to parse functions from a configuration file. 

- The bin packing problem.

For each problem there are a example config file.
 
## Configuration

### etc/config

A place to configure the algorithm. Not all the options are aviable yet. The file is selfdescribed.

### etc/function.conf

The configuration of the function problem. The path of this file is configured into the main etc/config file.

### log.conf
The log configuration file. By default to INFO.
