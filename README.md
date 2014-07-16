# nodamysql (nk-mysql)

A mostly simple, yet powerful C++ data integration toolset for nodakwaeri (nk) or other software(s) which would make use of it. 

Licensed under Apache v2.0, nodamysql features the MySQL C++ Connector from Oracle - which is licensed according to the terms provided 
under the [FOSS License Exception](http://www.mysql.com/about/legal/licensing/foss-exception/) when used with nodakwaeri (nk) or other 
Free and Open Source Software sporting acceptable license types. 

nodamysql (nk-mysql) is designed to use Prepared Statements - and to allow implicit asynchronous usage - in order to help keep your application secure and efficient.  


##Installation

Installation can again go one of two ways:

1. You are running Windows (32 or 64-bit) or Debian Wheezy or newer (64-bit), and do not wish to build (compile).  This could be the slightly quicker method of getting started.
  * <i>As of now, I have yet to include 32-bit Windows, or Debian binaries since the last few changes.</i>
  
2. You are running on Windows or Linux (Architecture Independent) and would like to maximize efficiency/performance and build yourself.
  * The automated build might initialize on Mac or SunOS, but probably won't work.
  * If you'd like to help support other systems please submit a pull request.  I do not own a Mac (not unfortunately :P), do not run SunOS, and will not run x86.

<b>Just a note:</b> Building is not difficult what-so-ever.  Yes there may be some pre-requisites but it's really not that bad.  In the end the best 
compatibility and performance can be reached on your machine (that you will be serving on) by letting all the tools be built by that system.  Furthermore, any 
host provider should support common build tools (especially Node hosting providers).  If you're developing yourself (as opposed to using Ghost for blogging or 
an application built on nodakwaeri or something), you should already have most of what you need prepared already.


### Prerequisites


##### Non-buildage

1. Node.js x64 (Windows and Linux) or x86 (Windows) & Git (Kinda need both to even be able to npm install <mod_name> eh?)
2. MySQL Community Server 5.6.x (x64 or x32).  If you're running Node.js x64, install MySQL Community Server x64, and so on.
3. You will need to add the path to the libmysql.lib & libmysql.dll files included with your MySQL distribution to your system PATH variable if not already set. If you do not do this, you will need to start your application from its respective node_modules/nk-mysql directory.
4. You might need to set the MYSQL_DIR environment variable on Windows.

Now you're ready to skip on down to the section that says 'The actual install' below.


##### Buildage

1. Node.js (x86 or x64) & Git (Kinda need both to even be able to npm install <mod_name> eh?)
2. Python (2.7.x x86 or x64)
3. node-gyp (npm install node-gyp -g)
4. MYSQL Community Server 5.5+ (if node.js is 64-bit, so should be your mysql server). At this time 5.6.x is recommended, build did not complete properly using 5.7.4-m14 dev release FYI.
5. Boost headers.  Simply grab the latest boost library from http://boost.org, and extract it someplace.  You are not required to build any boost libraries.
6. You probably need to set some environment variables or move some files around:

Windows | Linux | Mac | SunOS
--------|-------|-----|------
<b>MYSQL_DIR</b> needs to be set in order for the connector to build.  It should point to the directory which contains the '<b>include</b>' and '<b>lib</b>' folders. | Runs mysql_config for information instead.  Make sure to apt-get install libmysql-dev if mysql_config is not present on your system. | Same as Linux | Not sure here..
<b>BOOST_ROOT</b> needs to be set in order for the connector to build. It should point to the directory which contains the '<b>boost</b>' folder. | Copy the '<b>boost</b>' folder to your <i>/usr/lib</i> or <i>/usr/local/lib</i> directories (dont forget to ldconfig afterwards - and of course, you may have to refer to your favorite flav's documentation).  Alternatively you could apt-get install boost. | NA | NA

  * On Windows, there is no mysql_config binary - so we need MYSQL_DIR to be set.
  * On Debian/Linux or Mac, just ensure that mysql_config is present on your system (try <i>mysql_config</i> right in terminal)
  * On Windows, you must download the boost library, and set BOOST_ROOT so that we know where to find the boost/ folder containing all of the boost headers.

If it wasn't already apparent:
  * On Windows, you need Visual Studio.  To develop x64 you will need a paid version.  You must use VS201X x64 Native Command Tools in place of Command prompt to build 64-bit.  I recommend using VS201X x86 Native Command Tools even if you are building 32-bit as the Native Command Tools are properly configured for development - unlike Command Prompt.
  * On Debian/Linux, you need the build tools.  There are so many guides out there for how to install the build bundles I won't get into it. Start by searching how to install g++ on Linux. 

If you meet the above requirements, you're ready to build, and should move ahead to 'The actual install'.  That wasn't bad at all huh?


##### Some pre-emptive input
Keep in mind that if you're on a 64-bit system and want a 32-bit build; you should install node.js x86 and mysql x86.  If you want a 64-bit build, but do not have 64-bit build tools or are on a 32-bit system, then you're out of luck; install node.js x86 and mysql x86.  If you want a 64-bit build, are on a 64-bit system, and have the 64-bit build tools; install node.js x64 and mysql x64.


### The actual install

Just like with nodakwaeri, unless you are doing something very specific with nodamysql (nk-mysql), it is recommended that you install nodamysql via the [nk-mvc](http://github.com/mmod/nk-mvc) application, as it is a great starting place for any nodakwaeri application, and contains many working examples of usage throughout the project template.

With that said, if you are here to make sure you meet the conditions of the prerequisites as  part of the nk-mvc installation, then you should now return to the [nk-mvc documentation](http://github.com/mmod/nk-mvc) and continue from 'The actual install' on that page instead of continuing below.

To install (installing is how we build too!); open a terminal/shell/command tools/prompt and browse to the root of your application. Run the following command:

```
path_to_application/> npm install nk-mysql
```

If you are building, you should have no problems (If you verified that all of the requirements under prerequisites are met) and other than some potential warnings everything will build properly.


## Usage

At this time, many working examples of usage can be found in the [nk-mvc](http://github.com/mmod/nk-mvc) repository on Github - though I'll leave this documentation off with the following:


### Preparing our DBO

Including nodamysql into your project is relatively simple:

```node
// Require our type
var nkmysql = require( 'nk-mysql' ),
	driver = nkmysql.Driver;

// If you're running a select query or using the WHERE clause, you will need a model
/*
	var model = 
	{
		schema: 	// This is the table schema, if its a viewModel it may not have all fields in here - keep that in mind when running unprepared queries.
		{
			id: [ 'key', 'int', 'Record Id' ],
			title: [ false, 'text', 'Record Title' ],
			description: [ false, 'text', 'Record Description' ]
		}
	};
*/

// Create a config, which contains our connection details and/or model if needed.
var config =
{
  host: 'localhost',
  port: '3306',
  database: 'test',
  username: 'test',
  password: 'test',
  //'model': model
};

// And get us a dbo
var dbo = new driver( config );

```

### Unprepared Statements (Directly Executed Queries)

Unprepared Statements are Directly Executed Queries.  This is not recommended for situations where queries are reused with the same parameters containing only differing values.  There are several good reasons for this, all can be read up on here:  http://dev.mysql.com/doc/refman/5.7/en/c-api-prepared-statements.html

#### Simple Select
```node
// If you are running a select query, you must have passed a model to the DBO's constructor, so that we know how to interpret the returned datas
...

// Let's run a simple query then
var records = dbo.Query( 'SELECT * FROM tableName' ).Execute();

// And view our results.
for( var recs in records )
{
	console.log( 'Query results: ' + recs['id'] + ', ' + recs['title'] + ', ' + recs['description'] + '.' );
}
```

### Prepared Statements (Recommended)

Prepared statements are the recommended way to execute queries in any application where queries are reused over and over.  The link given above to MySQL explains why this is.


#### Select
```node
// If you are running a select query, you must have passed a model to the DBO's constructor, so that we know not only what to select, but how to interpret the returned datas as well.
...

// Let's run a simple select query
var records = dbo.Select( 'tableName' ).Execute();

// And view our results
for( var recs in records )
{
	console.log( 'Query results: ' + recs['id'] + ', ' + recs['title'] + ', ' + recs['description'] + '.' );
}
```

#### Advanced Select
```node
// Let's run an advanced select query, but first we need to set some bindings for our WHERE clause, and we can't forget a model is required here for 2 reasons
// 1.  Its a select query
// 2.  We're using a WHERE clause
...

var whereVals = 
{
	title: [ '=', 'text', '<titleValue>' ]	// You have to replace <titleValue>....
};

var records = dbo.Select( 'tableName' )
				.Join( 'table2Name' )
				.On( 'table2Name.id = tableName.id' )
				.Where( whereVals )
				.Execute();
				
...
```

#### Update
```node
// Let's run an update query, but first we need to set some bindings for our WHERE clause, and we cannot forget that a model is required here.
...

var updateVals = 
{
	description: 'Updating the description.'		// Our model tells the connector how to map the values.
},whereVals = 
{
	title: [ '=', 'text', '<titleValue>' ]			// You have to replace <titleValue>....
};

var records = dbo.Update( 'tableName' )				// Records contains the number of rows affected.
				.Values( insertVals )
				.Where( whereVals )
				.Execute();
				
...
```

#### Insert:
```node
// Let's run an insert query, but first we need to set some bindings for the values we will be inserting, and we cannot forget that a model is required here.
...

var insertVals = 
{
	title: 'titleVal',								
	description: 'Description.'						// Our model tells the connector how to map the values, and will ignore the id column, since we set it as primary key it auto-increments.
};

var records = dbo.Insert( 'tableName' )				// Records contains the number of rows affected.
				.Values( insertVals )
				.Where( whereVals )
				.Execute();
				
...
```

#### Delete:
```node
// Let's run an delete query, but first we must set some bindings for our WHERE clause, and we cannot forget that a model is required here.
...

var whereVals = 
{
	title: [ '=', 'text', '<titleValue>' ]			// You have to replace <titleValue>....	
};

var records = dbo.Delete( 'tableName' )				// Records contains the number of rows affected.
				.Where( whereVals )
				.Execute();
				
...
```

### Models

As you've seen thus far, models are important because they help to automate much of the process.  They are needed for mapping types of values to MySQL, and by other tools like nk.html - for generating labels for form fields, and much more.


## Development 

Feel free to fork the repository and submit pull requests. Browse any of our other repositories as well http://github.com/mmod.


### Created with:

[Eclipse Luna](https://www.eclipse.org/downloads/)

[Nodeclipse](https://github.com/Nodeclipse/nodeclipse-1)
 ([Eclipse Marketplace](http://marketplace.eclipse.org/content/nodeclipse), [site](http://www.nodeclipse.org))

[Node.js](http://nodejs.org)
