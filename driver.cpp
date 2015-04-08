/**
 * package: nodamysql
 * sub-package: driver/code
 * author:  Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik At MMOGP</a>
 * copyright: 2011-2015 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// INCLUDES
#include "driver.h"

// MySQL Includes ( Also targets Boost C++ Headers )
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/parameter_metadata.h>

using namespace v8;


// PROTOTYPES
Persistent<Function> Driver::constructor;   // Default constructor prototype


/**
 * Overloaded Constructor
 *
 * @return void
 *
 * @since 0.0.1
 */
Driver::Driver( nkDriverConstructorDefinitionBlockA ) : nkDriverConstructorDefinitionBlockB
{
}


/**
 * Destructor
 *
 * @return void
 *
 * @since 0.0.1
 */
Driver::~Driver()
{
}


/**
 * Initializes the Driver object and exports it to v8 to be wrapped for Node.js
 *
 * @param Handle<Object>    exports     Defines a reference to an exports object (Syntactically taken)
 *
 * @return void
 *
 * @since 0.0.1
 */
void Driver::Init( Handle<Object> exports )
{
    nkIsolationOS

    // Prepare constructor template
    Local<FunctionTemplate> tpl = nkFunctionTemplate( New );
    tpl->SetClassName( nkNewV8SString( "driver" ) );
    tpl->InstanceTemplate()->SetInternalFieldCount( 12 );

    // Prototype(s) of our methods for v8
    nkSetPrototypeMethod( tpl, "query", Query )
    nkSetPrototypeMethod( tpl, "select", Select )
    nkSetPrototypeMethod( tpl, "insert", Insert )
    nkSetPrototypeMethod( tpl, "values", Values )
    nkSetPrototypeMethod( tpl, "update", Update )
    nkSetPrototypeMethod( tpl, "delete", Delete )
    nkSetPrototypeMethod( tpl, "where", Where )
    nkSetPrototypeMethod( tpl, "join", Join )
    nkSetPrototypeMethod( tpl, "on", On )
    nkSetPrototypeMethod( tpl, "limit", Limit )
    nkSetPrototypeMethod( tpl, "order", Order )
    nkSetPrototypeMethod( tpl, "execute", Execute )
    nkSetPrototypeMethod( tpl, "executeQuery", ExecuteQuery )
    nkSetPrototypeMethod( tpl, "getConnection", GetConnection )
    nkSetPrototypeMethod( tpl, "getQuery", GetQuery )
    nkSetPrototypeMethod( tpl, "reset", Reset )

    nkSetConstructor( tpl )
    nkSetExports( "driver", constructor, tpl )
}


/**
 * Returns a new instance of the Driver object
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return      Driver      New instance of a Driver object
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::New( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    if( args.IsConstructCall() )
    {
        // Invoked as constructor: 'new nodamysql(...)'
        Local<Object> config;

        if( args[0]->IsUndefined() )
        {
            config = Object::New( nkIsolate );
            config->Set( nkNewV8SString( "host" ), nkNewV8String( "localhost" ) );
            config->Set( nkNewV8SString( "port" ), nkNewV8String( "3306" ) );
            config->Set( nkNewV8SString( "db" ), nkNewV8String( "test" ) );
            config->Set( nkNewV8SString( "user" ), nkNewV8String( "test" ) );
            config->Set( nkNewV8SString( "password" ), nkNewV8String( "testpass" ) );
        }
        else
        {
            config = args[0]->ToObject();
        }

        Driver* dvr;
        Local<Array> phmap = Array::New( nkIsolate );
        Local<Array> phspec = Array::New( nkIsolate );
        phspec->Set( 0, Integer::New( nkPreIsolated  0 ) );
        phspec->Set( 1, Integer::New( nkPreIsolated  0 ) );

        // A model doesn't have to be passed or used
        if( config->Get( nkNewV8String( "model" ) )->IsUndefined() )
        {
            Local<String> host = config->Get( nkNewV8SString( "host" ) )->ToString();
            Local<String> port = config->Get( nkNewV8SString( "port" ) )->ToString();
            Local<String> db = config->Get( nkNewV8SString( "db" ) )->ToString();
            Local<String> user = config->Get( nkNewV8SString( "user" ) )->ToString();
            Local<String> pass = config->Get( nkNewV8SString( "password" ) )->ToString();
            dvr = new Driver(
                nkNewV8PString( host ),
                nkNewV8PString( port ),
                nkNewV8PString( db ),
                nkNewV8PString( user ),
                nkNewV8PString( pass ),
                nkNewV8PObject( Object::New( nkIsolate ) ),
                nkNewV8PBoolean( v8::False( nkIsolate ) ),
                nkNewV8PInteger( Integer::New( nkPreIsolated  0 ) ),
                nkNewV8PInteger( Integer::New( nkPreIsolated  0 ) ),
                nkNewV8PArray( Array::New( nkIsolate ) ),
                nkNewV8PBoolean( v8::False( nkIsolate ) ),
                nkNewV8PString( nkNewV8String( "" ) )
            );
        }
        else
        {
            dvr = new Driver(
                    nkNewV8PString( config->Get( nkNewV8String( "host" ) )->ToString() ),
                    nkNewV8PString( config->Get( nkNewV8String( "port" ) )->ToString() ),
                    nkNewV8PString( config->Get( nkNewV8String( "db" ) )->ToString() ),
                    nkNewV8PString( config->Get( nkNewV8String( "user" ) )->ToString() ),
                    nkNewV8PString( config->Get( nkNewV8String( "password" ) )->ToString() ),
                    nkNewV8PObject( config->Get( nkNewV8String( "model" ) )->ToObject() ),
                    nkNewV8PBoolean( v8::False( nkIsolate ) ),
                    nkNewV8PInteger( Integer::New( nkPreIsolated  0 ) ),
                    nkNewV8PInteger( Integer::New( nkPreIsolated  0 ) ),
                    nkNewV8PArray( Array::New( nkIsolate ) ),
                    nkNewV8PBoolean( v8::False( nkIsolate ) ),
                    nkNewV8PString( nkNewV8String( "" )->ToString() )
            );
        }

        phmap->Set( 0, Local<Array>::New( nkPreIsolated phspec ) );
        nkResetV8PArray( dvr->phmap_, phmap );

        dvr->Wrap( args.This() );

        nkReturn( args.This() )
    }
    else
    {
        // Invoked as plain function 'nodamysql(...)', turn it into a construct call
        const int argc = 12;

        Local<Object> config;

        if( args[0]->IsUndefined() )
        {
            config = Object::New( nkIsolate ) ;
            config->Set( nkNewV8SString( "host" ), nkNewV8String( "localhost" ) );
            config->Set( nkNewV8SString( "port" ), nkNewV8String( "3306" ) );
            config->Set( nkNewV8SString( "db" ), nkNewV8String( "test" ) );
            config->Set( nkNewV8SString( "user" ), nkNewV8String( "test" ) );
            config->Set( nkNewV8SString( "password" ), nkNewV8String( "testpass" ) );
        }
        else
        {
            config = args[0]->ToObject();
        }

        Local<Array> phspec = Array::New( nkIsolate );
        phspec->Set( 0, Integer::New( nkPreIsolated  0 ) );
        phspec->Set( 1, Integer::New( nkPreIsolated  0 ) );

        Local<Value> argv[argc] = {
            config->Get( nkNewV8String( "host" ) ),
            config->Get( nkNewV8String( "port" ) ),
            config->Get( nkNewV8String( "db" ) ),
            config->Get( nkNewV8String( "user" ) ),
            config->Get( nkNewV8String( "pass" ) ),
            config->Get( nkNewV8String( "model" ) ),
            Local<Boolean>::New( nkPreIsolated v8::False( nkIsolate )  ),
            Integer::New( nkPreIsolated  0 ),
            Integer::New( nkPreIsolated  0 ),
            phspec,
            Local<Boolean>::New( nkPreIsolated v8::False( nkIsolate )  ),
            nkNewV8String( "" )
        };

        nkNewConstructor
        nkReturn( nkNewFromConstructor )
    }
}


/**
 * Sets a supplied query statement (Not prepared!)
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Query( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<Integer> prepared = Local<Integer>::New( nkPreIsolated dvr->prepared_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || ( type->IntegerValue() != ( KWAERI_EMPTY || KWAERI_NOPREP ) && prepared->IntegerValue() != KWAERI_NOT_PREP ) )
    {
        std::cout << "Error: Escaped .Query" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), args[0]->ToString() )->ToString() );
    nkResetV8PInteger( dvr->type_, KWAERI_NOPREP );
    nkResetV8PInteger( dvr->prepared_, KWAERI_NOT_PREP );

    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Builds a SELECT clause
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Select( const nkMethodArgumentType& args )
{
    // Always declare isolate and HandleScope
    nkIsolation( args )
    nkScope

    // Unwrap the object
    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<Boolean> modeled = Local<Boolean>::New( nkPreIsolated dvr->modeled_ );
    Local<Object> model = Local<Object>::New( nkPreIsolated dvr->model_ );

    // Fetch our arguments and define default behavior for incorrect arguments or no arguments
    if( args[0]->IsUndefined() || type->IntegerValue() != KWAERI_EMPTY )
    {
        std::cout << "Error: Escaped .Select" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // This is how we convert a JavaScript string to a std::string (c++)

    nkV8StringValueType av( args[0] );
    std::string select = *av;

    // Build our query string part
    std::string qp = std::string( "SELECT " );

    // If we're using models, we build our select in the proper manner
    if( modeled->BooleanValue() )
    {
        // The supplied model let's us know which columns to select - let's write it out
        Local<Array> keys = model->GetOwnPropertyNames();     // Gets us all the keys in an array
        for( int i = 0, l = keys->Length(); i < l; i++ )
        {
            // Get the keys to build the select clause
            Local<String> k = keys->Get( i )->ToString();

            nkV8StringValueType kav( k );
            std::string kv = *kav;

            if( i != 0 )
            {
                qp += ", ";
            }

            qp += kv;
        }

        qp += " FROM " + select;
    }
    else
    {   // Otherwise we tack on the user provided string to the query part to build the select clause
        qp += select;
    }

    // Set our query type and the query string.
    nkResetV8PStringUtf8( dvr->query_, qp.c_str() );
    nkResetV8PInteger( dvr->type_, KWAERI_SELECT );
    nkResetV8PInteger( dvr->prepared_, KWAERI_CLAUSE_PREP );

    // Return the entire object to allow chaining, results can be checked there
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Builds an INSERT clause
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Insert( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<Boolean> modeled = Local<Boolean>::New( nkPreIsolated dvr->modeled_ );
    Local<Object> model = Local<Object>::New( nkPreIsolated dvr->model_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || type->IntegerValue() != KWAERI_EMPTY )
    {
        std::cout << "Error: Escaped .Insert" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Fetch the user input
    nkV8StringValueType av( args[0] );
    std::string insert = *av;
    std::string qp;

    // If we're using models, the supplied model let's us know which columns to provide values for
    if( modeled->BooleanValue() )
    {
        // Build our query string part
        qp = std::string( "INSERT INTO " );
        qp += insert;
        qp += " ( ";

        Local<Array> keys = model->GetOwnPropertyNames();     // Gets us all the keys in an array
        for( int i = 0, l = keys->Length(); i < l; i++ )
        {
            // Get the keys to build the select clause
            Local<String> k = keys->Get( i )->ToString();

            nkV8StringValueType kav( k );
            std::string kv = *kav;

            if( i != 0 )
            {
                qp += ", ";
            }

            qp += kv;
        }

        qp += " ) ";
    }
    else
    {   // Otherwise it's in the user supplied string
        qp = "INSERT " + insert;
    }

    // Convert the std string to a v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // And concatenate it with the existing query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );
    nkResetV8PInteger( dvr->type_, KWAERI_INSERT );
    nkResetV8PInteger( dvr->prepared_, KWAERI_CLAUSE_PREP );

    // Return the entire object to allow chaining, results can be checked there
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Builds upon an INSERT or UPDATE clause
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Values( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<Integer> prepared = Local<Integer>::New( nkPreIsolated dvr->prepared_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );
    Local<Array> phmap = Local<Array>::New( nkPreIsolated dvr->phmap_ );

    if( args[0]->IsUndefined() || prepared->IntegerValue() == KWAERI_VALS_PREP_END || ( prepared->IntegerValue() != KWAERI_CLAUSE_PREP && prepared->IntegerValue() != KWAERI_VALS_PREP_START ) || ( type->IntegerValue() != KWAERI_INSERT && type->IntegerValue() != KWAERI_UPDATE ) )
    {
        std::cout << "Error: Escaped .Values" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Get some controls in place
    std::string qp;
    bool multi = false, adding = false;
    if( args[0]->IsArray() )
    {
        multi = true;
    }
    else if( prepared->IntegerValue() == KWAERI_VALS_PREP_START )
    {
        adding = true;
    }

    // If it's an insert query:
    if( type->IntegerValue() == KWAERI_INSERT )
    {   // We could pass an array of objects which contain values for multiple records to be inserted
        if( !adding )
        {
            qp = std::string( "VALUES ( " );
        }

        // If the argument is an array, we're inserting multiple records
        if( multi )
        {   // Add on to the insert clause, providing values
            Local<Array> mvals = Local<Array>::Cast( args[0] );
            Local<Array> phspec = Array::New( nkIsolate );

            // We'll set the phspec index to an empty array so that the length is 1 :)
            phmap->Set( 0, Array::New( nkIsolate ) );

            // This tells us how many times to loop when executing the prepared statement (we only need to do this once once);
            phspec->Set( 0, Integer::New( nkPreIsolated  mvals->Length() ) );
            bool prepped = false;
            for( int i = 0, l = mvals->Length(); i < l; i++ )
            {   //  Here we take each object in our array and create the parenthesized record
                Handle<Object> vals = mvals->Get( i )->ToObject();
                Local<Array> keys = vals->GetOwnPropertyNames();        // Gets us all the keys in an array
                for( int ii = 0, ll = keys->Length(); ii < ll; ii++ )
                {
                    // Get the keys to build the VALUES clause, and replace values with question mark
                    Local<String> k = keys->Get( ii )->ToString();
                    Local<Array> kvpair = Array::New( nkIsolate );              // Stores the key value pair for the place-holder map

                    // Because we need all place-holder values to contain a key and value to work with, and because we cannot allow place-holders to be overwritten;
                    kvpair->Set( 0, k );
                    kvpair->Set( 1, vals->Get( k )->ToString() );

                    // Store the key/value pair for the place-holder in the order they are added to the clause.
                    phmap->Set( phmap->Length(), kvpair );      // Using the Length() property ensures that we always set the next index in the array.

                    if( !prepped )
                    {
                        if( ii != 0 )
                        {
                            qp += ", ";
                        }

                        qp += "?";
                    }

                }

                if( !prepped )
                {
                    qp += " ) ";

                    prepped = true;
                }

                if( i == 0 )
                {
                    // This tells us how many parameters are included in each loop (again, only need to do this once)
                    phspec->Set( 1, Integer::New( nkPreIsolated  keys->Length() ) );
                }
            }

            // Set the phmap metadata in the first index
            phmap->Set( 0, phspec );
        }
        else
        {   // Add on to the insert clause, providing values
            Handle<Object> vals = args[0]->ToObject();
            Local<Array> keys = vals->GetOwnPropertyNames();        // Gets us all the keys in an array
            Local<Array> phspec = Array::New( nkIsolate );

            // This tells us how many times to loop when executing the prepared statement (we only need to do this once once);
            int lcount = 1;
            if( adding )
            {
                phspec = Local<Array>::Cast( phmap->Get( 0 ) );
                lcount += phspec->Get( 0 )->IntegerValue();

                phspec->Set( 0, Integer::New( nkPreIsolated  lcount ) );
            }
            else
            {
                phspec->Set( 0, Integer::New( nkPreIsolated  lcount ) );
                phspec->Set( 1, Integer::New( nkPreIsolated  keys->Length() ) );    // We only need this the first time on an insert statement that's being looped
            }

            phmap->Set( 0, phspec );

            for( int i = 0, l = keys->Length(); i < l; i++ )
            {
                // Get the keys to build the VALUES clause, and replace values with question mark
                Local<String> k = keys->Get( i )->ToString();
                Local<Array> kvpair = Array::New( nkIsolate );              // Stores the key value pair for the place-holder map

                // Create the key/value pair
                kvpair->Set( 0, k );
                kvpair->Set( 1, vals->Get( k )->ToString() );

                // Store the key/value pair for the place-holder in the order they are added to the clause.
                phmap->Set( phmap->Length(), kvpair );      // Using the Length() property ensures that we always set the next index in the array.

                if( !adding )
                {
                    if( i != 0 )
                    {
                        qp += ", ";
                    }

                    qp += "?";
                }
            }

            if( !adding )
            {
                qp += " )";
            }
        }
    }
    else if( type->IntegerValue() == KWAERI_UPDATE )
    {
        // Add on to the update clause, providing values
        Handle<Object> vals = args[0]->ToObject();
        Local<Array> keys = vals->GetOwnPropertyNames();        // Gets us all the keys in an array
        Local<Array> phspec = Array::New( nkIsolate );

        // This tells us how many times to loop when executing the prepared statement (we only need to do this once once);
        int lcount = 1;
        if( adding )
        {
            phspec = Local<Array>::Cast( phmap->Get( 0 ) );
            lcount += phspec->Get( 0 )->IntegerValue();

            phspec->Set( KWAERI_PH_KEY, Integer::New( nkPreIsolated  lcount ) );
        }
        else
        {
            phspec->Set( KWAERI_PH_KEY, Integer::New( nkPreIsolated  lcount ) );
            phspec->Set( KWAERI_PH_VALUE, Integer::New( nkPreIsolated  keys->Length() ) );  // We only need this the first time on an insert statement that's being looped
            qp = std::string( "SET " );
        }

        phmap->Set( KWAERI_PH_KEY, phspec );

        for( int i = 0, l = keys->Length(); i < l; i++ )
        {
            // Get the keys to build the where clause, and replace values with question mark
            Local<String> k = keys->Get( i )->ToString();
            Local<Array> kvpair = Array::New( nkIsolate );              // Stores the key value pair for the place-holder map

            // Create the key/value pair
            kvpair->Set( KWAERI_PH_KEY, k );
            kvpair->Set( KWAERI_PH_VALUE, vals->Get( k ) );

            // Store the values and conditions for the placeholders in the order they are added to the clause
            phmap->Set( phmap->Length(), kvpair );

            nkV8StringValueType kav( k );
            std::string kv = *kav;

            if( !adding )
            {
                if( i != 0 )
                {
                    qp += ", ";
                }

                qp += kv + "=? ";
            }
        }
    }

    // Convert the std string to a v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Concatenate the new query part with the existing query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );

    if( !multi )
    {
        nkResetV8PInteger( dvr->prepared_, KWAERI_VALS_PREP_START );
    }
    else
    {
        nkResetV8PInteger( dvr->prepared_, KWAERI_VALS_PREP_END );
    }

    // Return the entire object to allow chaining.
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Builds a UPDATE clause
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Update( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || ( type->IntegerValue() != KWAERI_EMPTY ) )
    {
        std::cout << "Error: Escaped .Update" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Get the name of the database to update and convert it to a std string
    nkV8StringValueType av( args[0] );
    std::string update = *av;

    // Build our query string part
    std::string qp = std::string( "UPDATE " );
    qp += update + " ";

    // Convert the query part from std string to v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Update the query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );
    nkResetV8PInteger( dvr->type_, KWAERI_UPDATE );
    nkResetV8PInteger( dvr->prepared_, KWAERI_CLAUSE_PREP );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Builds a DELETE clause
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Delete( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || type->IntegerValue() != KWAERI_EMPTY )
    {
        std::cout << "Error: Escaped .Delete" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Fetch the user input (database name) and convert it to a std string
    nkV8StringValueType av( args[0] );
    std::string remove = *av;

    // Build our query string part
    std::string qp = std::string( "DELETE FROM " );
    qp += remove;

    // Convert the std string to a v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Update the query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );
    nkResetV8PInteger( dvr->type_, KWAERI_DELETE );
    nkResetV8PInteger( dvr->prepared_, KWAERI_CLAUSE_PREP );

    // Return the entire object to allow for chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Builds a WHERE clause
 *
 * @param const ARGUMENTS&      ARGUMENTS passed during invocation
 *
 * @return String       The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Where( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<Integer> prepared = Local<Integer>::New( nkPreIsolated dvr->prepared_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );
    Local<Array> phmap = Local<Array>::New( nkPreIsolated dvr->phmap_ );

    // The supplied argument should be an object with arrays defining operators, types, and values for each where clause argument
    if( args[0]->IsUndefined() || !args[0]->IsObject() || prepared->IntegerValue() < KWAERI_CLAUSE_PREP || type->IntegerValue() == KWAERI_INSERT || type->IntegerValue() == KWAERI_NOPREP )
    {
        std::cout << "Error: Escaped .Where" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // We'll need to make sure that the phmap has its specs set so Length() returns at least 1
    if( phmap->Length() < 1 )
    {
        Local<Array> phspec = Array::New( nkIsolate );
        phspec->Set( 0, Integer::New( nkPreIsolated  1 ) );
        phspec->Set( 1, Integer::New( nkPreIsolated  0 ) );
        phmap->Set( 0, phspec );
    }

    // Now prepare to modify the phspec array as needed
    Local<Array> ophspec = Local<Array>::Cast( phmap->Get( 0 ) );
    Local<Array> phspec = Array::New( nkIsolate );
    phspec->Set( 0, ophspec->Get( 0 ) );    // This flag could sometimes increase from the use of this method, such as from a DELETE statement, but we won't mess with that quite yet
    int cindex = 0;             // Holds the total number of parameters that are being added to the phmap for this clause

    // Begin building our where clause
    std::string qp = " WHERE ( ";

    // We loop through the where object instead of setting it as the value of phmap_ because Update/Set/Where also uses the placeholder map in Set and Where at times.
    Handle<Object> where = args[0]->ToObject();
    Local<Array> keys = where->GetOwnPropertyNames();       // Gets us all the keys for an object inside of an array
    bool prevor = false;                    // A flag for determining if additional where clause arguments should be and'd or or'd
    for( int i = 0, l = keys->Length(); i < l; i++ )
    {
        // Get the key for this where clause argument, as well as the data array which defines the operator to use, the type to set, and the value to set.
        Local<String> k = keys->Get( i )->ToString();
        Local<Array> data = Local<Array>::Cast( where->Get( k ) );

        // Convert the key and the operator to std:string
        nkV8StringValueType kav( k ), oav( data->Get( KWAERI_OPERATOR ) );
        std::string kv = *kav, ov = *oav;

        // Build upon the query string
        if( i != 0 && !prevor )
        {
            qp += " AND ";
        }
        else if( i != 0 && prevor )
        {
            qp += " OR ";
            prevor = false;                 // Don't forget to reset this flag!
        }

        // Add the key and operator value to the query string
        qp += kv + " " + ov;

        int acount = data->Length();
        // The 'where' array could have more than 2 elements if - for instance - they are using a BETWEEN or providing an OR operator.
        if( acount > 2 )
        {   // We need to know our operator to determine how to build the string
            if( ov == std::string( "BETWEEN" ) )
            {   // Fancy up our string
                qp += "( ";
                // Start with 1 (cuz 0 is the operator)
                for( int ii = KWAERI_WHERE_VALUE; ii < acount; ii++ )
                {   // After the first iteration we'll need to AND or OR the additional values, respectively
                    if( ii > 1 )
                    {
                        qp += " AND ";
                    }

                    Local<Array> kvpair = Array::New( nkIsolate );              // Stores the key value pair for the place-holder map

                    // Create the key/value pair
                    kvpair->Set( KWAERI_PH_KEY, k );
                    kvpair->Set( KWAERI_PH_VALUE, data->Get( ii ) );

                    // Store the values and conditions for the placeholders in the order they are added to the clause
                    phmap->Set( phmap->Length(), kvpair );
                    cindex++;

                    qp += "?";
                }

                // Finish fancying up our string
                qp += " ) ";
            }
            else
            {   // If there are more than two elements, but it's not BETWEEN for the operator, the user is adding an OR statement for the current condition, or an OR statement for the next condition
                nkV8StringValueType waav( data->Get( KWAERI_WHERE_ADD ) );
                std::string wav = *waav;
                if( wav == std::string( "OR" ) || wav == std::string( "or" ) )
                {   // If the user is OR'ing for the current condition, they'll have actually supplied 4 or more elements..
                    if( acount > 3 )
                    {   // Fancy up our string
                        qp += "( ";
                        for( int ii = 1; ii < acount; ii++ )
                        {
                            if( ii > 1 )
                            {
                                qp += " OR ";
                            }

                            Local<Array> kvpair = Array::New( nkIsolate );              // Stores the key value pair for the place-holder map

                            // Create the key/value pair
                            kvpair->Set( KWAERI_PH_KEY, k );
                            kvpair->Set( KWAERI_PH_VALUE, data->Get( ii ) );

                            // Store the values and conditions for the placeholders in the order they are added to the clause
                            phmap->Set( phmap->Length(), kvpair );
                            cindex++;
                            qp += "?";
                        }

                        // Finish fancying up our string
                        qp += " ) ";
                    }
                    else
                    {   // If the user is OR'ing for the next condition, we just need to set prevor to true
                        prevor = true;
                    }
                }
            }
        }
        else
        {
            // Otherwise we set a single placeholder
            Local<Array> kvpair = Array::New( nkIsolate );              // Stores the key value pair for the place-holder map

            // Create the key/value pair
            kvpair->Set( KWAERI_PH_KEY, k );
            kvpair->Set( KWAERI_PH_VALUE, data->Get( KWAERI_WHERE_VALUE ) );

            // Store the values and conditions for the placeholders in the order they are added to the clause
            phmap->Set( phmap->Length(), kvpair );
            cindex++;
            qp += "?";
        }
    }

    // Add on the new part.
    qp += " )";

    // Let's set the new parameter count to be that of the existing plus the new amount from this clause.  Remember we are not allowing chaining yet.
    phspec->Set( 1, Integer::New( nkPreIsolated  ophspec->Get( 1 )->IntegerValue() + cindex ) );
    phmap->Set( 0, phspec );

    // Convert the query string part from std string to v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Concatenate the new query part with the existing query
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );

    // Concatenate the new query part with the existing query
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );
    nkResetV8PInteger( dvr->prepared_, KWAERI_WHERE_PREP_START );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Sets tables to join to the query
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Join( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> prepared = Local<Integer>::New( nkPreIsolated dvr->prepared_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || prepared->IntegerValue() < KWAERI_CLAUSE_PREP )
    {
        std::cout << "Error: Escaped .Join" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Get the database name for which we are joining
    nkV8StringValueType av( args[0] );
    std::string db = *av;

    // Build our query string part
    std::string qp = std::string( " JOIN " );
    qp += db;

    // Convert the query string part from std string to v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Concatenate the new query part with the existing query
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );
    nkResetV8PInteger( dvr->prepared_, KWAERI_JOIN_PREP );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Sets the 'ON' clause for joining tables
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::On( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> prepared = Local<Integer>::New( nkPreIsolated dvr->prepared_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || prepared->IntegerValue() != KWAERI_JOIN_PREP )
    {
        std::cout << "Error: Escaped .On" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Get user input and convert it to a std string
    nkV8StringValueType av( args[0] );
    std::string on = *av;

    // Build our query string part
    std::string qp = std::string( " ON ( " );
    qp += on;
    qp += std::string( " )" );

    // Convert the query string part from std string to v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Concatenate the new query part with the existing query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Sets the LIMIT clause
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Limit( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() )
    {
        std::cout << "Error: Escaped .Limit" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // We allow the invoker to specify 1 or two arguments for ease of use
    std::string from, to;
    if( args[1]->IsUndefined() )
    {
        // This is how we convert a JavaScript string to a std::string (c++)
        nkV8StringValueType avto( args[0] );
        from = "0";
        to = *avto;
    }
    else
    {
        nkV8StringValueType avfrom( args[0] ), avto( args[1] );
        from = *avfrom;
        to = *avto;
    }

    // Build our query string part
    std::string qp = std::string( " LIMIT " );
    qp += from;
    qp += std::string( ", " );
    qp += to;

    // Convert the query string part from std string to v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Concatenate the query part with the existing query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );
    nkResetV8PInteger( dvr->prepared_, KWAERI_LIMIT_PREP );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Sets the ORDERBY clause
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Order( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> prepared = Local<Integer>::New( nkPreIsolated dvr->prepared_ );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( args[0]->IsUndefined() || prepared->IntegerValue() == KWAERI_LIMIT_PREP )
    {
        std::cout << "Error: Escaped .Order" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // Fetch user input and convert it to a std string
    std::string order;
    nkV8StringValueType avorder( args[0] );
    order = *avorder;

    // Build our query string part
    std::string qp = std::string( " ORDER BY " );
    qp += order;

    // Convert the query string part from std string to v8 string
    Handle<Value> qph = nkNewV8String( qp.c_str() );
    nkV8StringValueType qpav( qph );
    Local<String> qps = nkNewV8String( *qpav );

    // Concatenate the query part with the existing query string
    nkResetV8PString( dvr->query_, String::Concat( query->ToString(), qps->ToString() )->ToString() );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}


/**
 * Executes the prepared query
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Execute( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<Array> phmap = Local<Array>::New( nkPreIsolated dvr->phmap_ );
    Local<String> _host = Local<String>::New( nkPreIsolated dvr->host_ );
    Local<String> _db = Local<String>::New( nkPreIsolated dvr->db_ );
    Local<String> _user = Local<String>::New( nkPreIsolated dvr->user_ );
    Local<String> _password = Local<String>::New( nkPreIsolated dvr->password_ );
    Local<String> _query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( type->IntegerValue() == KWAERI_EMPTY )
    {
        std::cout << "Error: Escaped .Execute" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    if( type->IntegerValue() == KWAERI_NOPREP /* && dvr->prepared_->IntegerValue() == KWAERI_NOT_PREP */ )
    {
        return( Driver::ExecuteQuery( args ) );
    }

    sql::Driver* driver;
    sql::Connection* con;
    sql::ResultSet* rset;
    sql::ResultSetMetaData* rsmeta;
    sql::PreparedStatement* pstmt;
    sql::ParameterMetaData* pmeta;

    try
    {
        nkV8StringValueType hav( _host ), dbav( _db ), userav( _user ), passav( _password ), queryav( _query );
        std::string host = *hav, db = *dbav, user = *userav, pass = *passav, query = *queryav, tresult;
        bool result = true;
        int affectedRows = 0;

        // Create a connection
        driver = get_driver_instance();
        con = driver->connect( host, user, pass );
        con->setSchema( db );
        pstmt = con->prepareStatement( query );

        if( pstmt )
        {
            std::cout << "Statement object created" << std::endl;
        }
        else
        {
            std::cout << "Statement object not created" << std::endl;
        }

        // If any values are set in the placeholder map we need to prepare them.
        if( phmap->Length() > 1 )
        {
            // Let's see how many loops we'll need to do
            Local<Array> phspec = Local<Array>::Cast( phmap->Get( 0 ) );
            int loops = phspec->Get( 0 )->IntegerValue();
            int acount = phspec->Get( 1 )->IntegerValue();

            //std::cout << "Loops: " << loops << " - Params per loop: " << acount << std::endl;

            if( loops > 1 && ( type->IntegerValue() == KWAERI_INSERT || type->IntegerValue() == KWAERI_UPDATE ) )
            {
                int cindex = 1;
                for( int c = 0; c < loops; c++ )
                {
                    for( int i = 1, l = acount + 1; i < l; i++ )
                    {
                        // Get the key/value pair for which to set the place-holder's values
                        Local<Array> kvp = Local<Array>::Cast( phmap->Get( cindex )->ToObject() );

                        // Now discern the type of the place-holder's value so we can set it
                        if( !kvp->Get( KWAERI_PH_VALUE )->IsUndefined() )
                        {
                            if( kvp->Get( KWAERI_PH_VALUE )->IsString() || kvp->Get( KWAERI_PH_VALUE )->IsDate() || kvp->Get( KWAERI_PH_VALUE )->IsNull() )
                            {
                                nkV8StringValueType strvalav( kvp->Get( KWAERI_PH_VALUE )->ToString() );
                                std::string strval = *strvalav;
                                if( strval.length() > 255 )
                                {   // We'll set this using setBlob
                                    std::istringstream stream( strval );
                                    pstmt->setBlob( i, &stream );
                                }
                                else
                                {   // Set using set string
                                    if( kvp->Get( KWAERI_PH_VALUE )->IsNull() )
                                    {
                                        pstmt->setString( i, std::string( "" ) );
                                    }
                                    else
                                    {
                                        pstmt->setString( i, strval );
                                    }
                                }
                            }
                            else if( kvp->Get( KWAERI_PH_VALUE )->IsNumber() )  //
                            {
                                if( kvp->Get( KWAERI_PH_VALUE )->IsInt32() )    // int32_t (probably checks if is 64-bit on 64-bit systems)
                                {
                                    pstmt->setInt( i, kvp->Get( KWAERI_PH_VALUE )->ToInteger()->Value() );
                                }
                                else if( kvp->Get( KWAERI_PH_VALUE )->IsUint32() )  // uint32_t (same as above)
                                {
                                    pstmt->setUInt( i, kvp->Get( KWAERI_PH_VALUE )->ToUint32()->Value() );
                                }
                                else
                                {
                                    pstmt->setDouble( i, kvp->Get( KWAERI_PH_VALUE )->ToNumber()->Value() );
                                }
                            }
                            else if( kvp->Get( KWAERI_PH_VALUE )->IsBoolean() )
                            {
                                pstmt->setBoolean( i, kvp->Get( KWAERI_PH_VALUE )->ToBoolean()->Value() );
                            }
                            else
                            {
                                nkV8StringValueType strvalav( kvp->Get( KWAERI_PH_VALUE )->ToString() );
                                std::string strval = *strvalav;
                                std::cout << "Error: Unrecongnized or unsupported type:" << strval << std::endl;
                            }
                        }
                        cindex++;
                    }

                    // Now we run our looped prepared statement
                    affectedRows += pstmt->executeUpdate();
                }

                //std::cout << "We affected " << affectedRows << " rows.  It was an INSERT UPDATE or DELETE" << std::endl;

                delete con;
                delete pstmt;

                nkReturn( Integer::New( nkPreIsolated  affectedRows ) )
            }
            else
            {
                for( int i = 1, l = acount + 1; i < l; i++ )
                {
                    // Get the key/value pair for which to set the place-holder's values
                    Local<Array> kvp = Local<Array>::Cast( phmap->Get( i ) );
                    // Local<String> k = kvp->Get( KWAERI_PH_KEY )->ToString();

                    // Now discern the type of the place-holder's value so we can set it
                    if( !kvp->Get( KWAERI_PH_VALUE )->IsUndefined() )
                    {
                        if( kvp->Get( KWAERI_PH_VALUE )->IsString() || kvp->Get( KWAERI_PH_VALUE )->IsDate() || kvp->Get( KWAERI_PH_VALUE )->IsNull() )
                        {
                            nkV8StringValueType strvalav( kvp->Get( KWAERI_PH_VALUE )->ToString() );
                            std::string strval = *strvalav;
                            if( strval.length() > 255 )
                            {   // We'll set this using setBlob
                                std::istringstream stream( strval );
                                pstmt->setBlob( i, &stream );
                            }
                            else
                            {   // Set using set string
                                if( kvp->Get( KWAERI_PH_VALUE )->IsNull() )
                                {
                                    pstmt->setString( i, std::string( "" ) );
                                }
                                else
                                {
                                    pstmt->setString( i, strval );
                                }
                            }
                        }
                        else if( kvp->Get( KWAERI_PH_VALUE )->IsNumber() )  //
                        {
                            if( kvp->Get( KWAERI_PH_VALUE )->IsInt32() )    // int32_t (probably checks if is 64-bit on 64-bit systems)
                            {
                                pstmt->setInt( i, kvp->Get( KWAERI_PH_VALUE )->ToInteger()->Value() );
                            }
                            else if( kvp->Get( KWAERI_PH_VALUE )->IsUint32() )  // uint32_t (same as above)
                            {
                                pstmt->setUInt( i, kvp->Get( KWAERI_PH_VALUE )->ToUint32()->Value() );
                            }
                            else
                            {
                                pstmt->setDouble( i, kvp->Get( KWAERI_PH_VALUE )->ToNumber()->Value() );
                            }
                        }
                        else if( kvp->Get( KWAERI_PH_VALUE )->IsBoolean() )
                        {
                            pstmt->setBoolean( i, kvp->Get( KWAERI_PH_VALUE )->ToBoolean()->Value() );
                        }
                        else
                        {
                            nkV8StringValueType strvalav( kvp->Get( KWAERI_PH_VALUE )->ToString() );
                            std::string strval = *strvalav;
                            std::cout << "Error: Unrecongnized or unsupported type:" << strval << std::endl;
                        }
                    }
                }
            }
        }

        if( type->IntegerValue() != KWAERI_SELECT )
        {
            // Let's execute the query and return how many rows were affected
            affectedRows = pstmt->executeUpdate();

            //std::cout << "We affected " << affectedRows << " rows.  It was an INSERT UPDATE or DELETE" << std::endl;

            delete con;
            delete pstmt;

            nkReturn( Integer::New( nkPreIsolated  affectedRows ) )
        }
        else
        {   // We're fetching a resultset
            // Let's go ahead and execute our prepared statement
            bool res = false;
            res = pstmt->execute();

            // Fetch our results
            if( res )
            {
                rset = pstmt->getResultSet();
                rsmeta = rset->getMetaData();
                int colcount = rsmeta->getColumnCount();
                Local<Array> records = Array::New( nkIsolate );
                int rec = 0;

                while( rset->next() )
                {
                    //std::cout << "We gots results! It was a SELECT." << std::endl;
                    // Each iteration brings another record, or set of columns present in the model which were specified to be selected.  What
                    // we want to do is allocate an array, each index containing an object which holds all the columns of this record.
                    Local<Object> record = Object::New( nkIsolate ) ;

                    for( int cc = 0, l = colcount; cc < l; cc++ )
                    {
                        // Get the column name
                        Local<String> k = nkNewV8String( rsmeta->getColumnLabel( cc + 1 ).c_str() );
                        //std::cout << "Column Label: " << k->ToString() << std::endl;

                        // Set the column data in the record
                        std::string ctype = rsmeta->getColumnTypeName( cc + 1 ).c_str();
                        if( ctype == std::string( "INT" ) )
                        {
                            Handle<Value> svh = nkNewV8String( rset->getString( rsmeta->getColumnLabel( cc + 1 ).c_str() ).c_str() );
                            record->Set( k, Integer::New( nkPreIsolated  svh->IntegerValue() ) );
                        }
                        else if( ctype == std::string( "VARCHAR" ) || ctype == std::string( "TEXT" ) || ctype == std::string( "DATETIME" ) )
                        {
                            Handle<Value> svh = nkNewV8String( rset->getString( rsmeta->getColumnLabel( cc + 1 ).c_str() ).c_str() );
                            nkV8StringValueType sav( svh );
                            record->Set( k, nkNewV8String( *sav ) );
                        }
                        else
                        {
                            std::string eret = "Error: Unknown column type [" + ctype + "].\n";
                            record->Set( k, nkNewV8String( eret.c_str() ) );
                        }
                    }

                    records->Set( rec, record );
                    rec++;
                }

                // Don't forget to release memory not under the management of Node
                delete con;
                delete pstmt;

                // Return the resulting array
                nkReturn( Handle<Array>::Cast( records ) )
            }else
            {
                nkReturn( nkNewV8String( "We somehow fail horribly..." ) )
            }
        }
    }catch( sql::SQLException &e )
    {
        // Don't forget to release memory not under the management of node
        delete con;
        delete pstmt;

        std::cout << "MySQLCPPCONN Failed" << std::endl << "# ERR: SQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << "\n" << std::endl;
        std::cout << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState " << e.getSQLState() << " )" << std::endl;

        nkReturn( nkNewV8String( "Catch Failed" ) )
    }
}


/**
 * Executes the unprepared query
 *
 * @since 0.2.0
 */
nkMethodReturnType Driver::ExecuteQuery( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<Integer> type = Local<Integer>::New( nkPreIsolated dvr->type_ );
    Local<String> _host = Local<String>::New( nkPreIsolated dvr->host_ );
    Local<String> _db = Local<String>::New( nkPreIsolated dvr->db_ );
    Local<String> _user = Local<String>::New( nkPreIsolated dvr->user_ );
    Local<String> _password = Local<String>::New( nkPreIsolated dvr->password_ );
    Local<String> _query = Local<String>::New( nkPreIsolated dvr->query_ );

    if( type->IntegerValue() == KWAERI_EMPTY || type->IntegerValue() != KWAERI_NOPREP )
    {
        std::cout << "Error: Escaped .ExecuteQuery" << std::endl;
        Driver* rdvre = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
        rdvre->Wrap( args.This() );
        nkReturn( args.This() )
    }

    // It's important to keep these here, so we can <b>delete</b> if an exception is caught.
    sql::Driver* driver;
    sql::Connection* con;
    sql::ResultSet* rset;
    sql::ResultSetMetaData* rsmeta;
    sql::Statement* stmt;
    try
    {
        nkV8StringValueType hav( _host ), dbav( _db ), userav( _user ), passav( _password ), queryav( _query );
        std::string host = *hav, db = *dbav, user = *userav, pass = *passav, query = *queryav, tresult;
        bool result = true;

        // Create a connection
        driver = get_driver_instance();
        con = driver->connect( host, user, pass );
        con->setSchema( db );
        stmt = con->createStatement();

        bool res = false;
        res = stmt->execute( query );

        // Fetch our results
        if( res )
        {
            rset = stmt->getResultSet();
            rsmeta = rset->getMetaData();
            Local<Array> records = Array::New( nkIsolate );

            // Now let's figure out how many columns are in the resultset
            int colcount = rsmeta->getColumnCount();

            int rec = 0;
            //std::cout << "We gots results from the noprep query!  It's a SELECT - spewing column info for each result..." << std::endl;
            while( rset->next() )
            {
                // Each iteration brings another record, or set of columns present in the model which were specified to be selected.  What
                // we want to do is allocate an array, each index containing an object which holds all the columns of this record.
                Local<Object> record = Object::New( nkIsolate ) ;

                for( int cc = 0, l = colcount; cc < l; cc++ )
                {
                    // Get the column name
                    //Local<String> k = keys->Get( cc )->ToString();
                    Local<String> k = nkNewV8String( rsmeta->getColumnLabel( cc + 1 ).c_str() );

                    // Set the column data in the record
                    std::string ctype = rsmeta->getColumnTypeName( cc + 1 ).c_str();
                    //std::cout << "Column: " << rsmeta->getColumnLabel( cc + 1 ) << " Type: " << ctype << std::endl;
                    if( ctype == std::string( "INT" ) )
                    {
                        Handle<Value> svh = nkNewV8String( rset->getString( rsmeta->getColumnLabel( cc + 1 ).c_str() ).c_str() );
                        record->Set( k, Integer::New( nkPreIsolated  svh->IntegerValue() ) );
                    }
                    else if( ctype == std::string( "VARCHAR" ) || ctype == std::string( "TEXT" ) || ctype == std::string( "DATETIME" ) )
                    {
                        Handle<Value> svh = nkNewV8String( rset->getString( rsmeta->getColumnLabel( cc + 1 ).c_str() ).c_str() );
                        nkV8StringValueType sav( svh );
                        record->Set( k, nkNewV8String( *sav ) );
                    }
                    else
                    {
                        std::string eret = "Error: Unknown column type [" + ctype + "].\n";
                        record->Set( k, nkNewV8String( eret.c_str() ) );
                    }
                }

                records->Set( rec, record );
                rec++;
            }
            //std::cout << "End of spewing..." << std::endl << std::endl;

            // Don't forget to release memory not under the management of Node
            delete con;
            delete stmt;

            // Return the resulting array
            nkReturn( Handle<Array>::Cast( records ) )
        }
        else
        {
            Local<Integer> affected = Integer::New( nkPreIsolated  stmt->getUpdateCount() );

            //std::cout << "We affected " << stmt->getUpdateCount() << " records. It was an INSERT UPDATE or DELETE" << std::endl;
            // Don't forget to release memory not under the management of Node
            delete con;
            delete stmt;

            // And return the count
            nkReturn( affected )
        }
    }catch( sql::SQLException &e )
    {
        // Don't forget to release memory not under the management of node
        delete con;
        delete stmt;

        std::cout << "MySQLCPPCONN Failed" << std::endl << "# ERR: SQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << "\n" << std::endl;
        std::cout << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState " << e.getSQLState() << " )" << std::endl;

        nkReturn( nkNewV8String( "Catch Failed" ) )
    }
}


/**
 * Gets the host_ string
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::GetConnection( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<String> _host = Local<String>::New( nkPreIsolated dvr->host_ );
    Local<String> _port = Local<String>::New( nkPreIsolated dvr->port_ );
    Local<String> _db = Local<String>::New( nkPreIsolated dvr->db_ );
    Local<String> _user = Local<String>::New( nkPreIsolated dvr->user_ );
    Local<String> _password = Local<String>::New( nkPreIsolated dvr->password_ );
    Local<String> _query = Local<String>::New( nkPreIsolated dvr->query_ );

    // Convert the v8::Persistent<v8::String> to std::string
    nkV8StringValueType hostav( _host ), portav( _port), dbav( _db ), userav( _user ), passav( _password );
    std::string host = *hostav, port = *portav, db = *dbav, user = *userav, password = *passav;

    std::string conn = "Connection: \nHost: " + host + ",\nPort: " + port + ",\nDatabase: " + db + ",\nUser: " + user + ",\nPassword: " + password + "\n";
    std::cout << conn << std::endl;

    // Convert the new std::string back to a v8::String of sorts
    Handle<Value> connh = nkNewV8String( conn.c_str() );

    // Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
    nkV8StringValueType connav( connh );

    // Return a string containing all the connection info for this Driver instance
    nkReturn( nkNewV8String( *connav ) )
}


/**
 * Gets the query_ string
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::GetQuery( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );
    Local<String> query = Local<String>::New( nkPreIsolated dvr->query_ );

    // Return the full query string to JavaScript
    nkReturn( query->ToString() )
}


/**
 * Resets the query_ string, phmap_ registry, and all flags
 *
 * @param const ARGUMENTS&      args        ARGUMENTS passed during invocation
 *
 * @return  String      The current query string
 *
 * @since 0.0.1
 */
nkMethodReturnType Driver::Reset( const nkMethodArgumentType& args )
{
    nkIsolation( args )
    nkScope

    Driver* dvr = ObjectWrap::Unwrap<Driver>( nkUnwrapThis );

    // Reset the query string
    nkResetV8PStringUtf8( dvr->query_, "" );

    // Reset the model
    if( !args[0]->IsUndefined() && args[0]->IsObject() )
    {   // If an object is passed, we replace the model
        Local<Object> model = args[0]->ToObject();
        nkResetV8PObject( dvr->model_, model );
        nkResetV8PBoolean( dvr->modeled_, true );
    }
    else if( !args[0]->IsUndefined() )
    {   // If anything other than an object is passed, we clear the model
        nkResetV8PObject( dvr->model_, Object::New( nkIsolate ) );
        nkResetV8PBoolean( dvr->modeled_, false );
    }// Otherwise we leave the model situation as-is

    // Reset the place holder map
    nkResetV8PArray( dvr->phmap_, Array::New( nkIsolate ) );

    // Reset the type flag
    nkResetV8PInteger( dvr->type_, KWAERI_EMPTY );

    // Reset the prepared flag
    nkResetV8PInteger( dvr->prepared_, KWAERI_NOT_PREP );

    // Return the entire object to allow chaining
    Driver* rdvr = new Driver( dvr->host_, dvr->port_, dvr->db_, dvr->user_, dvr->password_, dvr->model_, dvr->modeled_, dvr->type_, dvr->prepared_, dvr->phmap_, dvr->mapped_, dvr->query_ );
    rdvr->Wrap( args.This() );
    nkReturn( args.This() )
}
