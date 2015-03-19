# nodamysql ( nk-mysql )

A mostly simple, yet powerful C++ data integration toolset for nodakwaeri (nk) or other software(s) which would make use of it. 

Licensed under Apache v2.0, nodamysql features the MySQL C++ Connector from Oracle - which is licensed according to the terms provided 
under the [FOSS License Exception](http://www.mysql.com/about/legal/licensing/foss-exception/) when used with nodakwaeri (nk) or other 
Free and Open Source Software sporting acceptable license types. 

nodamysql (nk-mysql) is designed to use Prepared Statements - and to allow implicit asynchronous usage - in order to help keep your application secure and efficient. 


#### Continuous Integration

###### Build Status

**v0.12.x:**  [![Build Status](https://travis-ci.org/mmod/nodamysql.svg?branch=development)](https://travis-ci.org/mmod/nodamysql)

**v0.10.x:**  [![Build Status](https://travis-ci.org/mmod/nodamysql.svg?branch=development)](https://travis-ci.org/mmod/nodamysql) 

Regular builds and testing provided by https://travis-ci.org.


#### News

- Binaries are not supplied to Windows users for Node.js v0.12.x , you must build this package at this time (Binaries will be provided at a later date). [1]
- If for any reason you have an issue when utilizing Node.js v0.10.x, beyond submitting a bug report; please try installing nk-mysql using the following: `npm install nk-mysql@node-v10`. [2]
- MySQL Connector C++ is now at v1.1.5. Changes since v1.1.3 included a forced static binding to libmysql for the connector.
- nk-mysql may also be used with mysql-cluster.
- MySQL Cluster now supports both traditional interfacing, as well as noSQL. Oracle has been working on a Memcached entry point for MySQL Cluster, which boasts the speed and efficiency found with noSQL based database systems; yet for both traditional and noSQL based transactions.
- In other MMOD project; namely nk-mvc and nk-xrm - we are expanding upon the data tools to enable support for both developer and designer. Our aim is to introduce a truly innovative approach, not only to agile development via our original [MESSSE Code Methodology](https://www.linkedin.com/pulse/mmod-messse-code-richard-winters?trk=pulse-det-nav_art) - but also to product development in general.

###### Notes:
###### [1] Binaries are supplied to Windows users utilizing Node.js v0.10.x, and are enabled by default for Windows users who do not have build tools installed (i.e. node-gyp).
###### [2] Support for Node.js v0.12.x is now included for all platforms, and this singular package is backwards-compatible with Node.js v0.10.x as well.


## Installation

Full and complete installation documentation can be found at the [nodakwaeri wiki @ Github](http://github.com/mmod/nodakwaeri/wiki/nk-mysql)

#### The Quick Version

There are a few  prerequisites that need to be in place in order to properly build nodamysql.  On Windows, a precompiled binary is usually available but not always at this point.  If you are familiar with nodakwaeri, and are sure you meet the requirements to build then:

To install (installing is how we build too!); open a terminal/shell/command tools/prompt and browse to the root of your application. Run the following command:

```
path_to_application/> npm install nk-mysql
```

If you are building, you should have no problems (If you verified that all of the prerequisites are met) and other than some potential warnings everything will build properly.

If you're not building (meaning node-gyp and python are not installed), but meet the requirements for a precompiled add-on; a fall-back solution should be loaded for you.


## Usage

At this time, examples may be found in the [nodakwaeri wiki @ Github](https//github.com/mmod/nodakwaeri/wiki/nk-mysql) and [nodamysql repository @ Github](http://github.com/mmod/nodamysql/blob/master/test/features.js), though I'll leave this documentation with a copy of the wiki content:


### Preparing our DBO

Including nodamysql into your project is relatively simple:

```node
// Require our type
var nkmysql = require( 'nk-mysql' ),
    driver = nkmysql.driver;
...
```

nodamysql supports the use of models when using prepared statements.  Models help to automate select and insert clause statement creation, and are most commonly used with - and provided by - [nk-mvc](http://github.com/mmod/nk-mvc)'s model system. Models cannot be used with regular statements (unprepared), or with UPDATE, DELETE, CREATE, and advanced SELECT queries when using prepared statements.

```node
/*
If you are opting to use a model, you would define one like the one below.  

Models are not required at all. The purpose of specifying the type and whether 
a column is a key or required, is to provide necessary information to some 
advanced tools coming soon, such as the migration and code-first support tools 
for nodamysql and nk-mvc.
*/
var fakeModel = 
{
    id: [ 'key', 'type', 'Record Id' ],
    title: [ false, 'type', 'Record Title' ],
    description: [ false,'type', 'Record Description' ]
};

// Create a config, which contains our connection details and/or model if needed 
// (you can leave the model property out altogether).
var config =
{
  host: 'localhost',
  port: '3306',
  database: 'nktest',
  username: 'nktestadm',
  password: '^NKTestPass777$',
  //model: fakeModel
};

// And get us a dbo
var db = new driver( config );
...
```

### Regular Statements (Unprepared)

Unprepared Statements are Directly Executed Queries.  This is not recommended for situations where queries are reused with the same parameters containing only differing values.  There are several good reasons for this, all can be read up on here:  http://dev.mysql.com/doc/refman/5.7/en/c-api-prepared-statements.html

#### Select
```node
...

// Let's run a simple query then
var records = db.query( 'SELECT * FROM tableName' ).execute();

// And view our results.
for( var recs in records )
{
    console.log( 'Query results: ' + recs['id'] + ', ' + recs['title'] + ', ' + recs['description'] + '.' );
}
```

#### Insert, Update, Delete (No resultsets)
```node
// If we do not want to instantiate another db object after running a query, we can reset the one we have once 
// we're done so that we can reuse it:
db.reset();
...
// Let's run another simple query then
var affected = db.query( "UPDATE tableName SET columnName='value' WHERE id='1'" ).execute();

// And view our results.
console.log( 'Affected Rows: ' + affected );
}
```

Keep in mind that Create and Drop queries will always return 0, unless - and only in the case of a Drop query - there are records in the table at the time it is dropped, you may see the number of rows instead as a return result.

More examples of regular statements can be seen in the test file [here](http://github.com/mmod/nodamysql/blob/test/features.js).

### Prepared Statements

Prepared statements are the recommended way to execute queries in any application when queries are reused over and over as they may provide a performance benefit as well as a security benefit.  The link given above under Regular Statements to MySQL explains why this is.

#### Select

##### Simple Select
```node
/* 
If you are running a select query, you might have passed a model to the driver's constructor, so that we know
ahead of time what to select. You can set a new model at any time by invoking .reset( newModel );  
Alternatively, you can invoke .reset( false ) to clear the currently set model, or .reset() to leave the model
situation as is.
*/
db.reset( modelForExample );
...

// Let's run a simple select query
var records = db.select( 'tableName' ).execute();
```

Otherwise, if no model we would specify the selection
```node
db.reset( false );
...
var records = dbo.select( 'col1, col2, col3 from tableName' ).execute();

// And view our results
for( var recs in records )
{
    console.log( 'Query results: ' + recs['id'] + ', ' + recs['title'] + ', ' + recs['description'] + '.' );
}
```

##### A more involved Select
```node
// Let's run a more involved select query, we're going to use a model so we do not need to write out the full
// selection, and add a join and where clause as well.
db.reset( modelForExample );
...

var whereVals = 
{
    title: [ '=', '<titleValue>' ]  // You can replace the operator and <titleValue>....
};

var records = db.select( 'tableName' )
        .join( 'table2Name' )
        .on( 'table2Name.id = tableName.id' )
        .where( whereVals )
        .execute();     
...
```

##### An advanced example
```node
// Let's run an advanced select query, we're not going to use a model so we do need to write out the full
// selection
db.reset( false );
...

var whereVals = 
{
    title: [ '=', '<titleValue>' ]
};

var records = db.select( 'col1, col2, col3, table2Name.col1 as fakeCol from tableName' )
        .join( 'table2Name' )
        .on( 'table2Name.id = tableName.id' )
        .where( whereVals )
        .execute();     
...
```

#### Update

##### The basic example
```node
// Let's run an update query, no model is used here
db.reset( false );
...

var updateVals = 
{
        accessLevel: 3,
        description: 'Updating the description.'
},whereVals = 
{
    title: [ '=', '<titleValue>' ]      // You should replace the operator and <titleValue>....
};

var records = db.update( 'tableName' )      // Records contains the number of rows affected.
        .values( updateVals )
        .where( whereVals )
        .execute();                     
...
```

#### Insert:

##### The simple example
```node
// Let's run an insert query, no model is used here
...

var insertVals = 
{
    title: 'titleVal',                              
    description: 'Description.'
};

var records = dbo.insert( 'tableName' )     // Records contains the number of rows affected.
        .values( insertVals )
        .execute();         
...
```

##### The advanced example
```node
// Let's run an insert query, no model is used here and we're inserting multiple values at once
db.reset( false );
...

var records = db.insert( 'tableName' )      // Records contains the number of rows affected.
        .values
                ([
                    { accessLevel: 3, description: 'Inserting value 1' },
                    { accessLevel: 3, description: 'Inserting value 2' },
                    { accessLevel: 3, description: 'Inserting value 3' },
                ])
        .execute();

// The query was just sent to the server once, and we sent all the sets of parameters separately 
// in a loop via binary transfer to be executed.  This is part of the beauty of prepared statements.
...
```

##### The alternate advanced example
```node
// Let's run an insert query, no model is used here and we're inserting multiple values at once, but doing so
// in a way that if not for fun; would otherwise seem pointless and counter-productive
db.reset( false );
...

var records = db.insert( 'tableName' )      // Records contains the number of rows affected.
        .values( { accessLevel: 3, description: 'Inserting value 1' } )
        .values( { accessLevel: 3, description: 'Inserting value 2' } )
        .values( { accessLevel: 3, description: 'Inserting value 3' } )
        .execute();

// Although not as efficient with the entire process as the last example, like above we only sent the query to 
// the server once - and then sent all the sets of parameters separately one at a time in a loop via
// binary transfer to be executed. This is part of the beauty of prepared statements.
...
```

#### Delete:

##### The simple example
```node
// Let's run an delete query, no model is used here.
db.reset( false );
...

var whereVals = 
{
    title: [ '=', '<titleValue>' ]      // You should replace the operator and  <titleValue>....    
};

var records = db.delete( 'tableName' )      // Records contains the number of rows affected.
            .where( whereVals )
            .execute();
                
...
```

### Models

As you've seen thus far, models are used only to automate <b><i>select</i></b> and <b><i>insert</i></b> clause creation as far as nk-mysql is concerned - and are never required.  They are used a bit more extensively by nodakwaeri, and are planned to be used for more advanced data tools as well.

### Additional Notes & Troubleshooting

Aside from keeping in mind that Limit clauses should always be at the end of a query statement, and that you should invoke members of the database object in the order you would typically write out a query manually; there are some tools in place to help out:

##### Logical Implementation

For example, you cannot invoke.join() before setting a clause (such as .select, .insert, or .delete)
* .on(), .where(), .values(), .order(), .limit(), .execute(), .executeQuery() all follow similar common-sense rules as .join() to help avoid SQL errors.

##### Getting Connection Info
```node
...
console.log( db.getConnection() );
...
```

##### Reviewing the Query
```node
...
console.log( db.getQuery() );
...
```

## Development 

Feel free to fork the repository and submit pull requests. Browse any of our other repositories as well http://github.com/mmod.


### Created with:

[Eclipse Luna](https://www.eclipse.org/downloads/)

[Nodeclipse](https://github.com/Nodeclipse/nodeclipse-1)
 ([Eclipse Marketplace](http://marketplace.eclipse.org/content/nodeclipse), [site](http://www.nodeclipse.org))

[Node.js](http://nodejs.org)
