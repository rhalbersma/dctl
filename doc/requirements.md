
Requirements
------------

### Platforms

The DCTL is a cross-platform library

* **Supported:** 
    * **tested:** current development takes place on Linux Mint 14.1 64-bit and Microsoft Windows 7 64-bit.
    * **untested:** successful builds should be possible for the Google Android platform (using the [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html) with a supported compiler).
* **Unsupported:** 
    * the Apple Mac OS X and iOS platforms are currently unsupported.

### Compilers

The DCTL is a modern [C++](http://isocpp.org) library that is dependent on many of the features that have come available with the new [C++11 Standard](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3376.pdf). The C++11 features that the DCTL currently depends on consist of `auto`, initializer lists, lambda expressions, `nullptr`, range-based `for`-loop, rvalue references, `static_assert`, and variadic templates. Unfortunately, feature support for C++11 [differs across compiler vendors](http://wiki.apache.org/stdcxx/C%2B%2B0xCompilerSupport). 

* **Supported**:
    * **tested:** the recommended and actively tested compilers are [g++](http://gcc.gnu.org/projects/cxx0x.html) and [Clang](http://clang.llvm.org/cxx_status.html). Current development takes place with g++ 4.7.2 and Clang 3.3 (on Linux), and MinGW g++ 4.8.0 (on Windows).
    * **untested:** the minimally required but untested compilers are [Intel](http://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler) 13.0, g++ 4.6.3, and Clang 3.1. However, it is strongly recommended to use the most recently available version of the compiler vendor of your choice.
* **Unsupported:** 
    * the [Microsoft Visual C++ 2012](http://msdn.microsoft.com/en-us/library/vstudio/hh567368.aspx) compiler, even with its [November CTP](http://blogs.msdn.com/b/vcblog/archive/2012/11/02/visual-c-c-11-and-the-future-of-c.aspx), currently has insufficient(ly stable) C++11 support to reliably compile the DCTL test-suite. This could be subject to change when new updates come available.

### Boost headers

The DCTL uses several of the popular [Boost C++ libraries](http://www.boost.org). Current development takes place with Boost 1.53.0, and the minimum required version is 1.49.0 (untested). Boost is a collection of header-only libraries, and you simply have to point your compiler to the Boost include directory. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_53_0/more/getting_started/index.html) on how to do this on your system. After that, you can continue to use your regular build process.

### Boost libraries

The test-suite uses [Boost.Test](http://www.boost.org/doc/libs/1_53_0/libs/test/doc/html/index.html). In order to build and run the test-suite (see below), you need to compile Boost.Test into a dynamic library and point your linker to its location. Consult the [Boost documentation](http://www.boost.org/doc/libs/1_53_0/more/getting_started/index.html) on how to do this on your system.

* **NOTE:** compilation of the Boost libraries is **ONLY** a requirement for running the test-suite, and **NOT** for using the DCTL headers with your application.  

### CMake

The test-suite is built using the [CMake](http://www.cmake.org/) cross-platform build system. Most development environments can run the CMake generated Makefiles. To generate native build solutions for your own development environment, consult the [CMake documentation](http://www.cmake.org/cmake/help/runningcmake.html).

* **NOTE:** CMake is **ONLY** a requirement for building the test-suite, and **NOT** for using the DCTL headers with your application. 

### Mercurial

Development is being tracked with the [Mercurial](http://mercurial.selenic.com/) distributed version control system, the [hg flow](https://bitbucket.org/yujiewu/hgflow/wiki/Home) extension, and the [BitBucket](https://bitbucket.org) hosting service. The default branch is the currently stable version and the commits along this branch are tagged with versions. Development takes place on the develop branch, with features being developed in so-called feature branches.

* **NOTE**: Mercurial is **ONLY** a requirement for contributing to the DCTL, and **NOT** for using the DCTL headers with your application. Downloading and unpacking the latest zipped source archive works equally well.