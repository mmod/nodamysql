/**
 * package: nodamysql
 * sub-package: test/sqltest
 * author:  Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik At MMOGP</a>
 * copyright: 2011-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


 // DEPS
var nkmysql = require( '../build/Release/nodamysql.node' ),
    driver = nkmysql.driver;

// Get an instance of our database object
var db = new driver
(
    {
    client: 'mysql',
    host: 'localhost',
    port: '3306',
    db: 'nktest',
    user: 'nktestadm',
    password: '^NKTestPass777$'
    }
);

// Prepare a result array for our tests
var result = [],
    status = 0,
    tresult = "";

console.log( "nodamysql (nk-mysql) tests.  Author: Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik @ MMOGP<a/>\n" );
console.log( "Starting test series: Statements (Unprepared)" );
console.log();

// 0 is test to create the users table using unprepared statements
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists users( id int(11) not null auto_increment primary key, first varchar(50), last varchar(50) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create users table." );
tresult = ( result[0] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 1: ' + result[0] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 1 is test to create the ages table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists ages( id int(11) not null auto_increment primary key, user int(11), age int(11) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create ages table." );
tresult = ( result[1] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 2: ' + result[1] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 2 is test to create the colors table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists colors( id int(11) not null auto_increment primary key, name varchar(50) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create colors table." );
tresult = ( result[2] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 3: ' + result[2] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 3 is test to create the favcolors table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists favcolors( id int(11) not null auto_increment primary key, user int(11), color int(11) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create favcolors table." );
tresult = ( result[3] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 4: ' + result[3] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 4 is test to insert users using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "insert into users ( first, last ) values ( 'upstfirst1', 'upstlast' ), ( 'upstfirst2', 'upstlast2' ), ( 'upstfirst3', 'upstlast3' )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 3 users, first with the wrong first name" );
tresult = ( result[4] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 5: ' + result[4] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 5 is test to select the users we just inserted using unprepared statements
db = db.reset();
var user = db.query( "select * from users" )
           .execute();
result.push
(
    JSON.stringify
    (
        user
    )
);  // We should get an array with an object for each row selected from (only 3 of course)
console.log( '------' );
console.log( "Selecte 3 users, first with the wrong first name." );
tresult = ( result[5] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 6: ' + result[5] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 6 is a continuation for updating existing record in a table
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "update users set first='upstfirst' where id='" + user[0].id + "'" )
        .execute()
    )
);  // We should get the number of rows affected (should be 1)
console.log( '------' );
console.log( "Update 1 user, the one with the wrong first name." );
tresult = ( result[6] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 7: ' + result[6] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 7 is a continuation for inserting another table so we can perform an advanced select using joins
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "insert into ages ( user, age ) values ( '" + user[0].id + "', '87' ), ( '" + user[1].id + "', '56' ), ( '" + user[2].id + "', '63' )" )
        .execute()
    )
);  // We should get the number of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 3 ages." );
tresult = ( result[7] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 8: ' + result[7] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 8 is a continuation for inserting another table so we can perform an advanced select using joins
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "insert into colors ( name ) values ( 'red' ), ( 'white' ), ( 'blue' )" )
        .execute()
    )
);  // We should get the number of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 3 colors." );
tresult = ( result[8] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 9: ' + result[8] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 9 is a continuation for inserting another table so we can perform an advanced select using joins
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "insert into favcolors( user, color ) values ( '" + user[0].id + "', 1 ), ( '" + user[1].id + "', 2 ), ( '" + user[0].id + "', 3 )" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 3 favcolors." );
tresult = ( result[9] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 10: ' + result[9] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 10 is a continuation for actually testing an advanced select using joins
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "select first, last, age, colors.name from users join ages on ages.user = users.id join favcolors on favcolors.user = users.id join colors on colors.id = favcolors.color" )
        .execute()
    )
);  // We should get an array with an object for a selected record at each index (should be 3 this time around)
console.log( '------' );
console.log( "Select with joins." );
tresult = ( result[10] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 11: ' + result[10] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 11 is a continuation for actually testing an advanced select using joins but with a where clause as well
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "select first, last, age, colors.name from users join ages on ages.user = users.id join favcolors on favcolors.user = users.id join colors on colors.id = favcolors.color where users.first='upstfirst' and users.last='upstlast'" )
        .execute()
    )
);  // We should get an array with an object for a selected record at each index (should be 1 this time around)
console.log( '------' );
console.log( "Select with joins and a where clause." );
tresult = ( result[11] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 12: ' + result[11] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 12 is cleaning up some of the data we just generated using a where clause
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "delete from users where first='upstfirst'" )
        .execute()
    )
);  // We should ge tthe number of rows affected (should be 1)
console.log( '------' );
console.log( "Delete with a where clause." );
tresult = ( result[12] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 13: ' + result[12] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 13 is a continuation for cleaning up all the data we just generated;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "delete from users" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 2)
console.log( '------' );
console.log( "Delete on the users table." );
tresult = ( result[13] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 14: ' + result[13] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 14 is a continuation for cleaning up all the data we just generated;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "delete from ages" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 3)
console.log( '------' );
console.log( "Delete on the ages table." );
tresult = ( result[14] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 15: ' + result[14] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 15 is a continuation for cleaning up all the data we just generated;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "delete from colors" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 3)
console.log( '------' );
console.log( "Delete on the colors table." );
tresult = ( result[15] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 16: ' + result[15] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 16 is a continuation for cleaning up all the data we just generated;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "delete from favcolors" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 3)
console.log( '------' );
console.log( "Delete on the favcolors table." );
tresult = ( result[16] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 17: ' + result[16] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 17 is for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table users" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the users table." );
tresult = ( result[17] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 18: ' + result[17] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 18 is a continuation for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table ages" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the ages table." );
tresult = ( result[18] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 19: ' + result[18] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 19 is a continuation for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table colors" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the colors table." );
tresult = ( result[19] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 20: ' + result[19] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 20 is a continuation for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table favcolors" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the favcolors table." );
tresult = ( result[20] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 21: ' + result[20] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

console.log( "End of Statement (Unprepared) tests...continuing..." );
console.log( "Starting test series: Prepared Statements & Models" );
console.log();

/**
 We can stop testing unprepared statements now -> we didn't even need to do anything more than a simple select and insert to test unprepared statements effectively

 Next we'll create a couple models to help us along the way.  Models simply provide an easy method for invoking select or insert clauses, and allow you a few
 more goodies in frameworks like nodakwaeri
**/

var userModel =
{
    first: [ true, 'text', 'First name' ],  // In nk keys are column names, values an array containing: 1. bool for required or 'key', 2. type string, 3. text label
    last: [ true, 'text', 'Last name' ]
}, userWithIdModel =
{
    id: [ true, 'int', 'Record Id' ],
    first: [ true, 'text', 'First name' ],
    last: [ true, 'text', 'Last name' ]
}, ageModel =
{
    user: [ true, 'int', 'User Id' ],
    age: [ true, 'int', 'Age' ]
}, colorModel =
{
   name: [ true, 'text', 'Color' ]
}, favcolorModel =
{
    user: [ true, 'int', 'User Id' ],
    color: [ true, 'int', 'Color Id' ]
};

// And now we'll test using prepared statements with and with-out models, but first recreate the tables using unprepared statements

// 21 is test to create the users table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists users( id int(11) not null auto_increment primary key, first varchar(50), last varchar(50) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create users table." );
tresult = ( result[21] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 22: ' + result[21] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 22 is test to create the ages table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists ages( id int(11) not null auto_increment primary key, user int(11), age int(11) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create ages table." );
tresult = ( result[22] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 23: ' + result[22] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 23 is test to create the colors table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists colors( id int(11) not null auto_increment primary key, name varchar(50) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create colors table." );
tresult = ( result[23] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 24: ' + result[23] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 24 is test to create the favcolors table using unprepared statements
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "create table if not exists favcolors( id int(11) not null auto_increment primary key, user int(11), color int(11) )" )
        .execute()
    )
);  // We should get a count of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Create favcolors table." );
tresult = ( result[24] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 25: ' + result[24] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 25 This first bit shows adding multiple records by invoking values() over and overdb.insert( 'users' )
db = db.reset( userModel );
result.push
(
    JSON.stringify
    (
        db.insert( 'users' )
        .values( { first: 'pstfirst1' , last: 'pstlast' } )
        .values( { first: 'pstfirst2' , last: 'pstlast2' } )
        .values( { first: 'pstfirst3' , last: 'pstlast3' } )
        .execute()
    )
);  // We should get a count of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 3 users into the users table using model and consecutive .values() invocations." );
tresult = ( result[25] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 26: ' + result[25] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 26 This second bit shows adding multiple records by feeding an array of objects to values() one time
db = db.reset( userModel );
result.push
(
    JSON.stringify
    (
        db.insert( 'users' )
        .values( [
            { first: 'pstfirst4', last: 'pstlast4' },
            { first: 'pstfirst5', last: 'pstlast5' },
            { first: 'pstfirst6', last: 'pstlast6' }
           ] )
        .execute()
    )
);  // We should get a count of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 3 users into the users table using model and array of objects passed to single .values() invocation." );
tresult = ( result[26] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 27: ' + result[26] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 27 is test to select the user(s) we just inserted using prepared statements and models
db = db.reset( userWithIdModel );
var users = db.select( 'users' )
           .execute();
result.push
(
    JSON.stringify( users )
);  // We should get an array with an object for each row selected from (only 6 of course)
console.log( '------' );
console.log( "Select 6 users from the users table using a model." );
tresult = ( result[27] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 28: ' + result[27] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 28 is a continuation for updating existing record in a table
db = db.reset( false ); // Anything other than an object removes an existing model from the dbo if defined
result.push
(
    JSON.stringify
    (
        db.update( 'users' )
        .values( { first: 'pstfirst' } )
        .where( { id: [ '=', users[0].id ], first: [ '=', 'pstfirst1' ] } )
        .execute()
    )
);  // We should get the number of rows affected (should be 1)
console.log( '------' );
console.log( "Update the first user in the users table (The one with the wrong first name). No model, with where clause" );
tresult = ( result[28] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 29: ' + result[28] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 29 is a continuation for inserting another table so we can perform an advanced select using joins.  Prepared, no models
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.insert( "into ages ( user, age ) " )
        .values( [
            { user: users[0].id, age: 87 },
            { user: users[1].id, age: 56 },
            { user: users[2].id, age: 63 },
            { user: users[3].id, age: 47 },
            { user: users[4].id, age: 36 },
            { user: users[5].id, age: 53 }
           ] )
        .execute()
    )
);  // We should get the number of rows affected (should be 6)
console.log( '------' );
console.log( "Insert 6 ages into the ages table. No model, with array of objects passed to single invocation to .values()" );
tresult = ( result[29] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 30: ' + result[29] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 30 is a continuation for inserting another table so we can perform an advanced select using joins.  Prepared, no models
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.insert( "into colors ( name ) " )
        .values( [
            { color: 'red' },
            { color: 'white' },
            { color: 'blue' },
            { color: 'yellow' },
            { color: 'green' },
            { color: 'purple' }
           ] )
        .execute()
    )
);  // We should get the number of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 6 colors into the colors table. No model, with array of objects passed to single invocation to .values()" );
tresult = ( result[30] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 31: ' + result[30] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 31 is a continuation for inserting another table so we can perform an advanced select using joins
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.insert( "into favcolors ( user, color ) " )
        .values( [
            { user: users[0].id, color: 6 },
            { user: users[1].id, color: 5 },
            { user: users[2].id, color: 4 },
            { user: users[3].id, color: 3 },
            { user: users[4].id, color: 2 },
            { user: users[5].id, color: 1 }
           ] )
        .execute()
    )
);  // We should ge the number of rows affected (should be 3)
console.log( '------' );
console.log( "Insert 6 color ids into the favcolors table. No model, with array of objects passed to single invocation to .values()" );
tresult = ( result[31] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 32: ' + result[31] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 32 is a continuation for actually testing an advanced select using joins. Prepared, no model
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.select( "first, last, age, colors.name as favcolor from users" )
        .join( "ages" )
        .on( "ages.user = users.id" )
        .join( "favcolors" )
        .on( "favcolors.user = users.id" )
        .join( "colors" )
        .on( "colors.id = favcolors.color" )
        .execute()
    )
);  // We should get an array with an object for a selected record at each index (should be 3 this time around)
console.log( '------' );
console.log( "Select a bunch of info from the users table, with joins on ages, favcolors and colors. No model." );
tresult = ( result[32] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 33: ' + result[32] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 33 is a continuation for actually testing an advanced select using joins but with a where clause as well.  Prepared, no model
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.select( "first, last, age, colors.name as favcolor from users" )
        .join( "ages" )
        .on( "ages.user = users.id" )
        .join( "favcolors" )
        .on( "favcolors.user = users.id" )
        .join( "colors" )
        .on( "colors.id = favcolors.color" )
        .where( { first: [ '=', 'pstfirst' ], last: [ '=', 'pstlast' ] } )  // 'first' and 'last' only exist in users, from amongst the tables we're joining.
        .execute()
    )
);  // We should get an array with an object for a selected record at each index (should be 1 this time around)
console.log( '------' );
console.log( "Select a bunch of info from the users table, with joins again like before - but with a where clause as well. No model." );
tresult = ( result[33] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 34: ' + result[33] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 34 is a continuation for cleaning up all the data we just generated;
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.delete( "users" )
        .where( { first: [ '=', 'pstfirst' ] } )
        .execute()
    )
);  // We should ge the number of rows affected (should be 1)
console.log( '------' );
console.log( "Delete the first record from the users table." );
tresult = ( result[34] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 35: ' + result[34] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 35 is a continuation for cleaning up all the data we just generated in the users table;
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.delete( "users" )
        .execute()
    )
);  // We should get the number of rows affected (should be 5)
console.log( '------' );
console.log( "Delete all the records from the users table." );
tresult = ( result[35] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 36: ' + result[35] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 36 is a continuation for cleaning up all the data we just generated in the ages table;
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.delete( "ages" )
        .execute()
    )
);  // We should get the number of rows affected (should be 6)
console.log( '------' );
console.log( "Delete all the records from the ages table." );
tresult = ( result[36] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 37: ' + result[36] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 37 is a continuation for cleaning up all the data we just generated in the colors table;
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.delete( "colors" )
        .execute()
    )
);  // We should get the number of rows affected (should be 6)
console.log( '------' );
console.log( "Delete all the records from the colors table." );
tresult = ( result[37] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 38: ' + result[37] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 38 is a continuation for cleaning up all the data we just generated in the favcolors table;
db = db.reset( false );
result.push
(
    JSON.stringify
    (
        db.delete( "favcolors" )
        .execute()
    )
);  // We should get the number of rows affected (should be 6)
console.log( '------' );
console.log( "Delete all the records from the favcolors table." );
tresult = ( result[38] != 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 39: ' + result[38] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 39 is for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table users" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the users table." );
tresult = ( result[39] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 40: ' + result[39] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 40 is a continuation for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table ages" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the ages table." );
tresult = ( result[40] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 41: ' + result[40] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 41 is a continuation for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table colors" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the colors table." );
tresult = ( result[41] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 42: ' + result[41] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

// 42 is a continuation for cleaning up all the tables we created;
db = db.reset();
result.push
(
    JSON.stringify
    (
        db.query( "drop table favcolors" )
        .execute()
    )
);  // We should ge the number of rows affected (should be 0 or Undefined)
console.log( '------' );
console.log( "Drop of the favcolors table." );
tresult = ( result[42] == 0 ) ? 'Passed' : 'Failed';
console.log( 'Result 43: ' + result[42] + '...' + tresult );
console.log( '------' );
console.log();
if( tresult === 'Failed' ){ status = 1; }

console.log( "End of Statement (Unprepared) tests...continuing..." );
console.log( "All tests complete." );
console.log();
console.log( "Follow Up/Repeat:\nIf you just installed nodamysql and the test failed, ensure you have the test database setup and configured.  Either replace the values at the top of this script to reflect the proper database connection information, or setup a database, user, and password, based on the following:\n");
console.log( "mysql_prompt/> create database nktest;");
console.log( "mysql_prompt/> create user 'nktestadm'@'localhost' identified by '^NKTestPass777$';");
console.log( "mysql_prompt/> grant all privileges on nktest.* to 'nktestadm'@'localhost';\n");
console.log( "Specifics:\nLook through the test results in your console. If you see no errors and no zeros (other than for create/drop table queries), all the tests passed.\n");

process.exit( status );