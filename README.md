# nodamysql (nk-mysql)

The MySQL C++ Connector, and data integration tool for nodakwaeri.  

Currently, nodamysql is in a pre-release state and is still under development.  It should install and build on Windows( Only precompiled for windows x64, using Node.jsx64 and MySQL x64 ), but not all the clauses are implemented properly quite yet and the execute() method only works for a very specific query used for testing.  It will be finished very soon, at which point I'll get the build script working properly for Linux (Using Debian 7.X x64 to create precompiled binaries as well).  I do not own a Macintosh or a SunOS machine, nor am I interested in installing copies of all Linux flavors; so if anyone is interested in helping to support those platforms: by all means! 


##Installation

Installation can be a bit tricky, let's try to break this down a little bit:

### Prerequisites

1. You obviously need Node.js and Python installed, along with Node-Gyp.
1. You need Git installed (grab the latest version of Git from http://git-scm.com/ ).
  * Be sure that the path to your Git installation's bin directory is listed in your system environment's PATH variable (Guide for that should be linked soon).
2. You need MYSQL Community Server 5.5+ installed (At this time 5.6.19 is recommended, build did not complete properly using 5.7.4-m14 dev release FYI).
  * Be sure that MYSQL_DIR is set in your system environment variables, and points to your MySQL installation directory (Guide for that should be linked soon).
  * Be sure that you install the architecture that matches your system and your node.js installation.
3. You need the Boost headers on your machine.  Simply grab the latest boost library from http://boost.org, and extract it someplace.  
  * Be sure to set the path to the root of the boost package as BOOST_ROOT in your system environment.  The root directory holds the non-versioned 'boost' folder containing all the headers.

### Buildage (This is optional on windows...sorta.)

To build on windows you will need to have installed Visual Studio (Paid versions for 64-bit buildage, or the 'Desktop' variant of the Express releases for 32-bit only).  Furthermore, you will want to be using VS201X x86 or x64 Native Tools Command Prompt (as opposed to a normal command prompt) to invoke the below build commands without error (as all environment variables are set properly).  If you use the x86 version only 32-bit variants can be built, if you use the x64 variant only 64-bit can be built.  Rather simple right?

To build on Linux... As long as you followed the Prerequisites you will be fine - You'll want GCC (g++) 4.7+ and the other build tools, in place of Visual Studio (More detailed write up once the linux build is finished). 

Now, to get started; open a terminal/shell/command prompt and browse to the root of your application. Install nk-mysql:

```
path_to_application/> npm install nk-mysql
```

If you are building, you should have no problems and other than some potential warnings everything will build properly.

If you do not have any build tools installed (Windows, anyone?), luckily for you there should have been a fallback configuration to use a pre-compiled variant of the MySQL C++ Connector and Data Integration Tools for nodakwaeri.  In other words, you can move on to 'Usage'.


## Usage

At this time, all examples can be found within the app/ directory of the source repository.  


## Development

Feel free to fork the repository and submit pull requests. Browse any of our other repositories as well http://github.com/mmod.


### Created with:

[Eclipse Kepler](https://www.eclipse.org/downloads/)

[Nodeclipse](https://github.com/Nodeclipse/nodeclipse-1)
 ([Eclipse Marketplace](http://marketplace.eclipse.org/content/nodeclipse), [site](http://www.nodeclipse.org))

[Node.js](http://nodejs.org)

The above software(s) are all free, open-source projects which grow with each new contribution.
