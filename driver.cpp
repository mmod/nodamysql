/**
 * package: nodamysql
 * sub-package: driver/code
 * author:  Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik At MMOGP</a>
 * copyright: 2011-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// INCLUDES
#include <v8.h>
#include <node.h>
#include <iostream>
#include <string>
#include "resource.h"
#include "driver.h"

// MySQL Includes ( Also targets Boost C++ Headers )
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace v8;


// PROTOTYPES
Persistent<Function> Driver::constructor;	// Default constructor prototype


/**
 * Overloaded Constructor
 *
 * @return void
 *
 * @since 0.0.1
 */
Driver::Driver( Persistent<String> host, Persistent<String> port, Persistent<String> db, Persistent<String> user, Persistent<String> password, Persistent<Integer> type, Persistent<Object> model, Persistent<Object> phmap, Handle<Boolean> mapped, Persistent<String> query, Handle<Boolean> prepared, Handle<Boolean> modeled )
			: host_( host ), port_( port ), db_( db ), user_( user ), password_( password ), type_( type ), model_( Persistent<Object>::New( model->ToObject() ) ), phmap_( Persistent<Object>::New( phmap->ToObject() ) ), mapped_( mapped ), query_( query ), prepared_( prepared ), modeled_( modeled )
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
 * @param Handle<Object>	exports		Defines a reference to an exports object (Syntactically taken)
 *
 * @return void
 *
 * @since 0.0.1
 */
void Driver::Init( Handle<Object> exports )
{
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New( New );
    tpl->SetClassName( String::NewSymbol( "Driver" ) );
    tpl->InstanceTemplate()->SetInternalFieldCount( 12 );

    // Prototype(s) of our methods for v8
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Query" ), FunctionTemplate::New( Query )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Select" ), FunctionTemplate::New( Select )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Insert" ), FunctionTemplate::New( Insert )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Values" ), FunctionTemplate::New( Values )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Update" ), FunctionTemplate::New( Update )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Delete" ), FunctionTemplate::New( Delete )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Where" ), FunctionTemplate::New( Where )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Join" ), FunctionTemplate::New( Join )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "On" ), FunctionTemplate::New( On )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Limit" ), FunctionTemplate::New( Limit )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Order" ), FunctionTemplate::New( Order )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Execute" ), FunctionTemplate::New( Execute )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "GetConnection" ), FunctionTemplate::New( GetConnection )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "GetQuery" ), FunctionTemplate::New( GetQuery )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "Reset" ), FunctionTemplate::New( Reset )->GetFunction() );

    constructor = Persistent<Function>::New( tpl->GetFunction() );
    exports->Set( String::NewSymbol( "Driver" ), constructor );
}


/**
 * Returns a new instance of the Driver object
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return		Driver		New instance of a Driver object
 *
 * @since 0.0.1
 */
Handle<Value> Driver::New( const Arguments& args )
{
    HandleScope scope;

    if( args.IsConstructCall() )
    {
	// Invoked as constructor: 'new nodamysql(...)'
	Local<Object> config;

	if( args[0]->IsUndefined() )
	{
	    config = Object::New();
	    config->Set( String::NewSymbol( "host" ), String::New( "localhost" ) );
	    config->Set( String::NewSymbol( "port" ), String::New( "3306" ) );
	    config->Set( String::NewSymbol( "db" ), String::New( "test" ) );
	    config->Set( String::NewSymbol( "user" ), String::New( "test" ) );
	    config->Set( String::NewSymbol( "password" ), String::New( "testpass" ) );
	}
	else
	{
	    config = args[0]->ToObject();
	}

	Driver* dvr

	// A model doesn't have to be passed or used
	if( config->Get( String::New( "model" ) )->IsUndefined() )
	{
	    dvr = new Driver(
		Persistent<String>::New( config->Get( String::New( "host" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "port" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "db" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "user" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "password" ) )->ToString() ),
		Persistent<Integer>::New( Integer::New( 0 ) ),
		Persistent<Object>::New( Object::New() ),
		Persistent<Object>::New( Object::New() ),
		Handle<Boolean>( v8::False() ),
		Persistent<String>::New( String::New( "" ) ),
		Handle<Boolean>( v8::False() ),
		Handle<Boolean>( v8::False )
	    );
	}
	else
	{
	    dvr = new Driver(
		Persistent<String>::New( config->Get( String::New( "host" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "port" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "db" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "user" ) )->ToString() ),
		Persistent<String>::New( config->Get( String::New( "password" ) )->ToString() ),
		Persistent<Integer>::New( Integer::New( 0 ) ),
		Persistent<Object>::New( config->Get( String::New( "model" ) )->ToObject() ),
		Persistent<Object>::New( Object::New() ),
		Handle<Boolean>( v8::False() ),
		Persistent<String>::New( String::New( "" ) ),
		Handle<Boolean>( v8::False() ),
		Handle<Boolean>( v8::True )
	    );
	}

	dvr->Wrap( args.This() );

	return scope.Close( args.This() );
    }
    else
    {
	// Invoked as plain function 'nodamysql(...)', turn it into a construct call
	const int argc = 11;

	Local<Object> config;

	if( args[0]->IsUndefined() )
	{
	    config = Object::New();
	    config->Set( String::NewSymbol( "host" ), String::New( "localhost" ) );
	    config->Set( String::NewSymbol( "port" ), String::New( "3306" ) );
	    config->Set( String::NewSymbol( "db" ), String::New( "test" ) );
	    config->Set( String::NewSymbol( "user" ), String::New( "test" ) );
	    config->Set( String::NewSymbol( "password" ), String::New( "testpass" ) );
	}
	else
	{
		config = args[0]->ToObject();
	}

	Local<Value> argv[argc] = {
	    config->Get( String::New( "host" ) ),
	    config->Get( String::New( "port" ) ),
	    config->Get( String::New( "db" ) ),
	    config->Get( String::New( "user" ) ),
	    config->Get( String::New( "pass" ) ),
	    Integer::New( 0 ),
	    config->Get( String::New( "model" ) ),
	    Object::New(),
	    Local<Boolean>::New( v8::False() ),
	    String::New( "" ),
	    Local<Boolean>::New( v8::False() )
	};

	return scope.Close( constructor->NewInstance( argc, argv ) );
    }
}


/**
 * Sets a supplied query statement (Not prepared!)
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Query( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() || ( dvr->type_->IntegerValue() != KWAERI_EMPTY ) )
    {
	std::cout << "Error: Escaped .Query" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), args[0]->ToString() ) );
    dvr->type_ = Persistent<Integer>::New( Integer::New( KWAERI_NOPREP ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Builds a SELECT clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Select( const Arguments& args )
{
    // Always declare the HandleScope
    HandleScope scope;

    // Unwrap the object
    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    // Fetch our arguments and define default behavior for incorrect arguments or no arguments
    if( args[0]->IsUndefined() || dvr->type_->IntegerValue() != KWAERI_EMPTY )
    {
	std::cout << "Error: Escaped .Select" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    // Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
    // but because of this we will use alternate means in order to avoid any issues.

    // This is how we convert a JavaScript string to a std::string (c++)
    String::AsciiValue av( args[0] );
    std::string select = *av;

    // Build our query string part
    std::string qp = std::string( "SELECT " );

    // If we're using models, we build our select in the proper manner
    if( dvr->modeled_->BooleanValue() )
    {
	// The supplied model let's us know which columns to select - let's write it out
	Local<Array> keys = dvr->model_->GetOwnPropertyNames();		// Gets us all the keys in an array
	for( int i = 0, l = keys->Length(); i < l; i++ )
	{
		// Get the keys to build the select clause
		Local<String> k = keys->Get( i )->ToString();

		String::AsciiValue kav( k );
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
    {	// Otherwise we tack on the user provided string to the query part to build the select clause
	qp += select;
    }

    //std::cout << "Prints Part: " << qp << std::endl;

    // This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
    // all of the suggested methods for converting a std::string to v8::Local<v8::String> were
    // ineffective for me.  This is the only way I've gotten it to work properly without error(s).
    //Handle<Value> qph = String::New( qp.c_str() );

    //String::AsciiValue qpav( qph );

    //Local<String> qps = String::New( *qpav );

    //dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );
    dvr->query_ = Persistent<String>::New( String::New( qp.c_str() ) );
    dvr->type_ = Persistent<Integer>::New( Integer::New( KWAERI_SELECT ) );

    //std::cout << "Prints: " << dvr->query_->ToString() << std::endl;

    // Return the entire object to allow chaining, results can be checked there
    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Builds an INSERT clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Insert( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() || ( dvr->type_->IntegerValue() != KWAERI_EMPTY ) )
    {
	std::cout << "Error: Escaped .Insert" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    String::AsciiValue av( args[0] );
    std::string insert = *av;
    std::string qp;

    // If we're using models, the supplied model let's us know which columns to provide values for
    if( dvr->modeled_->BooleanValue() )
    {
	// Build our query string part
	qp = std::string( "INSERT INTO " );
	qp += insert;


	qp += " ( ";

	Local<Array> keys = dvr->model_->GetOwnPropertyNames();		// Gets us all the keys in an array
	for( int i = 0, l = keys->Length(); i < l; i++ )
	{
	    // Get the keys to build the select clause
	    Local<String> k = keys->Get( i )->ToString();

	    String::AsciiValue kav( k );
	    std::string kv = *kav;

	    if( i != 0 )
	    {
		    qp += ", ";
	    }

	    qp += kv;
	}

	qp += " ) ";

	//std::cout << "Prints Part: " << qp << std::endl;
    }
    else
    {
	qp = "INSERT " + insert;
    }


    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );
    dvr->type_ = Persistent<Integer>::New( Integer::New( KWAERI_INSERT ) );

    // Return the entire object to allow chaining, results can be checked there
    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Builds upon an INSERT or UPDATE clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Values( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() || ( dvr->type_->IntegerValue() != ( KWAERI_INSERT || KWAERI_UPDATE ) ) )
    {
	std::cout << "Error: Escaped .Values" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    std::string qp;
    if( dvr->type_->IntegerValue() == KWAERI_INSERT )
    {
	// Add on to the insert clause, providing values
	qp = std::string( "VALUES ( " );

	Handle<Object> vals = args[0]->ToObject();
	Local<Array> keys = vals->GetOwnPropertyNames();		// Gets us all the keys in an array
	for( int i = 0, l = keys->Length(); i < l; i++ )
	{
	    // Get the keys to build the VALUES clause, and replace values with question mark
	    Local<String> k = keys->Get( i )->ToString();

	    // Store the values and conditions for the placeholders in the order they are added to the clause
	    dvr->phmap_->Set( k, vals->Get( k )->ToString() );

	    if( i != 0 )
	    {
		    qp += ", ";
	    }

	    qp += "?";
	}

	qp += " );";
    }
    else if( dvr->type_->IntegerValue() == KWAERI_UPDATE )
    {
	// Add on to the update clause, providing values
	qp = std::string( "SET " );

	Handle<Object> vals = args[0]->ToObject();
	Local<Array> keys = vals->GetOwnPropertyNames();		// Gets us all the keys in an array
	for( int i = 0, l = keys->Length(); i < l; i++ )
	{
	    // Get the keys to build the where clause, and replace values with question mark
	    Local<String> k = keys->Get( i )->ToString();

	    // Store the values and conditions for the placeholders in the order they are added to the clause
	    dvr->phmap_->Set( k, vals->Get( k )->ToString() );

	    String::AsciiValue kav( k );
	    std::string kv = *kav;

	    if( i != 0 )
	    {
		    qp += ", ";
	    }

	    qp += kv + "=?";
	}

	qp += " ) ";
    }

    //std::cout << "Prints Part: " << qp << std::endl;

    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Builds a UPDATE clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Update( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() || ( dvr->type_->IntegerValue() != KWAERI_EMPTY ) )
    {
	std::cout << "Error: Escaped .Update" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    String::AsciiValue av( args[0] );
    std::string update = *av;

    // Build our query string part
    std::string qp = std::string( "UPDATE " );
    qp += update + " ";

    //std::cout << "Prints Part: " << qp << std::endl;

    // Update the query string
    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );
    dvr->type_ = Persistent<Integer>::New( Integer::New( KWAERI_UPDATE ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Builds a DELETE clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Delete( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() || ( dvr->type_->IntegerValue() != KWAERI_EMPTY ) )
    {
	std::cout << "Error: Escaped .Delete" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    String::AsciiValue av( args[0] );
    std::string remove = *av;

    // Build our query string part
    std::string qp = std::string( "DELETE FROM " );
    qp += remove;

    //std::cout << "Prints Part: " << qp << std::endl;

    // Update the query string
    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );
    dvr->type_ = Persistent<Integer>::New( Integer::New( KWAERI_DELETE ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Builds a WHERE clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Where( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    // The supplied argument should be an object with arrays defining operators, types, and values for each where clause argument
    if( args[0]->IsUndefined() || !args[0]->IsObject() || dvr->type_->IntegerValue() == KWAERI_EMPTY || dvr->type_->IntegerValue() == KWAERI_INSERT || dvr->type_->IntegerValue() == KWAERI_NOPREP )
    {
	std::cout << "Error: Escaped .Where" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    // Begin building our where clause
    std::string qp = " WHERE ( ";

    // We loop through the where object instead of setting it as the value of phmap_ because Update/Set/Where also uses the placeholder map in Set and Where at times.
    Handle<Object> where = args[0]->ToObject();
    Local<Array> keys = where->GetOwnPropertyNames();		// Gets us all the keys for an object inside of an array
    for( int i = 0, l = keys->Length(); i < l; i++ )
    {
	// Get the key for this where clause argument, as well as the data array which defines the operator to use, the type to set, and the value to set.
	Local<String> k = keys->Get( i )->ToString();
	Local<Array> data = Local<Array>::Cast( where->Get( k ) );

	Local<Array> prep = Array::New();

	// Convert the key and the operator to std:string
	String::AsciiValue kav( k ), oav( data->Get( KWAERI_OPERATOR ) );
	std::string kv = *kav, ov = *oav;

	// Build upon the query string
	if( i != 0 )
	{
	    qp += " AND ";
	}

	// Add the key and operator value to the query string
	qp += kv + ov;

	// The 'where' array could have more than 3 elements if - for instance - they are using a BETWEEN operator.
	if( int acount = data->Length() > 3 )
	{
	    for( int ii = 2; ii < acount; ii++ )
	    {
		// In which case we need to set multiple placeholders
		prep->Set( 0, data->Get( KWAERI_COLUMN_TYPE ) );

		if( ii > 2 )
		{
		    prep->Set( 1, data->Get( ii ) );
		    qp += " AND ";
		}
		else
		{
		    prep->Set( 1, data->Get( KWAERI_COLUMN_VALUE ) );
		}

		dvr->phmap_->Set( k, prep );
		qp += "?";
	    }
	}
	else
	{
	    // Otherwise we set a single placeholder
	    prep->Set( 0, data->Get( KWAERI_COLUMN_TYPE ) );
	    prep->Set( 1, data->Get( KWAERI_COLUMN_VALUE ) );
	    dvr->phmap_->Set( k, prep );
	    qp += "?";
	}
    }

    // add on the new part.
    qp += " )";

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), String::New( qp.c_str() ) ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Sets tables to join to the query
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Join( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() )
    {
	std::cout << "Error: Escaped .Join" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    // Get the database name for which we are joining
    String::AsciiValue av( args[0] );
    std::string db = *av;

    // Build our query string part
    std::string qp = std::string( " JOIN " );
    qp += db;

    //std::cout << "Prints Part: " << qp << std::endl;

    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Sets the 'ON' clause for joining tables
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::On( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() )
    {
	std::cout << "Error: Escaped .On" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    String::AsciiValue av( args[0] );
    std::string on = *av;

    // Build our query string part
    std::string qp = std::string( " ON ( " );
    qp += on;
    qp += std::string( " )" );

    //std::cout << "Prints Part: " << qp << std::endl;

    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );

    return scope.Close( args.This() );
}


/**
 * Sets the LIMIT clause
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Limit( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() )
    {
	std::cout << "Error: Escaped .Limit" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    // We allow the invoker to specify 1 or two arguments for ease of use
    std::string from, to;
    if( args[1]->IsUndefined() )
    {
	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue avto( args[0] );
	from = "0";
	to = *avto;
    }
    else
    {
	String::AsciiValue avfrom( args[0] ), avto( args[1] );
	from = *avfrom;
	to = *avto;
    }

    // Build our query string part
    std::string qp = std::string( " LIMIT " );
    qp += from;
    qp += std::string( ", " );
    qp += to;

    //std::cout << "Prints Part: " << qp << std::endl;

    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Sets the ORDERBY clause
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Order( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( args[0]->IsUndefined() )
    {
	std::cout << "Error: Escaped .Order" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    std::string order;
    String::AsciiValue avorder( args[0] );
    order = *avorder;

    // Build our query string part
    std::string qp = std::string( " ORDER BY " );
    qp += order;

    //std::cout << "Prints Part: " << qp << std::endl;

    // Update the driver's query string
    Handle<Value> qph = String::New( qp.c_str() );
    String::AsciiValue qpav( qph );
    Local<String> qps = String::New( *qpav );

    dvr->query_ = Persistent<String>::New( String::Concat( dvr->query_->ToString(), qps->ToString() ) );

    Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
    rdvr->Wrap( args.This() );
    return scope.Close( args.This() );
}


/**
 * Executes the prepared query
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Execute( const Arguments& args )
{
    HandleScope scope;

    Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

    if( dvr->type_->IntegerValue() == KWAERI_EMPTY )
    {
	std::cout << "Error: Escaped .Execute" << std::endl;
	Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->model_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), Persistent<String>::New( dvr->query_->ToString() ), Handle<Boolean>( dvr->prepared_ ), Handle<Boolean>( dvr->modeled_ ) );
	rdvre->Wrap( args.This() );
	return scope.Close( args.This() );
    }

    if( dvr->prepared_IntegerValue() == KWAERI_NOPREP )
    {
	try
	{
	    sql::Driver* driver;
	    sql::Connection* con;
	    sql::ResultSet* rset;
	    sql::ResultSetMetaData* rsmeta;
	    sql::Statement* stmt;

	    String::AsciiValue hav( dvr->host_ ), dbav( dvr->db_ ), userav( dvr->user_ ), passav( dvr->password_ ), queryav( dvr->query_ );
	    std::string db = *dbav, host = *hav, user = *userav, pass = *passav, query = *queryav, tresult;
	    bool result = true;

	    // Create a connection
	    driver = get_driver_instance();
	    con = driver->connect( host, user, pass );
	    con->setSchema( db );
	    stmt = con->createStatement();

	    std::cout << "Executing query...";
	    bool res = stmt->execute( query );

	    // Fetch our results
	    if( res )
	    {
		rset = stmt->getResultSet();
		rsmeta = rset->getMetaData();
		Local<Array> records = Array::New();

		// Now let's figure out how many columns are in the resultset
		int colcount = rsmeta->getColumnCount();
		std::cout << "Column count: " << colcount << endl;

		int rec = 0;
		while( rset->next() )
		{
		    std::cout << "We gots results from the noprep query!" << std::endl;
		    // Each iteration brings another record, or set of columns present in the model which were specified to be selected.  What
		    // we want to do is allocate an array, each index containing an object which holds all the columns of this record.
		    Local<Object> record = Object::New();

		    for( int cc = 0, l = colcount; cc < l; cc++ )
		    {
			// Get the column name
			//Local<String> k = keys->Get( cc )->ToString();
			Local<String> k = String::New( rsmeta->getColumnLabel( cc + 1 ).c_str() );
			std::cout << "Column Label: " << k << std::endl;

			// Set the column data in the record
			std::string ctype = rsmeta->getColumnTypeName( cc + 1 ).c_str() );
			std::cout << "Column Type Name: " << ctype << std::endl;
			if( ctype == std::string( "int" ) )
			{
			    record->Set( k, Integer::New( rset->getInt( k->ToString()/* rsmeta->getColumnLabel( cc + 1 ).c_str() */ ) ) )
			}
			else if( ctype == std::string( "varchar" ) || ctype == std::string( "text" ) )
			{
			    Handle<Value> svh = String::New( rset->getString( k->ToString()/* rsmeta->getColumnLabel( cc + 1 ).c_str() */ ).c_str() );
			    String::AsciiValue sav( svh );
			    record->Set( k, String::New( *sav ) );
			}
			else
			{
			    record->Set( k, String::New( "Error: Unknown column type." ) );
			}
		    }

		    records->Set( rec, record );
		    rec++;
		}

		std::cout << "We should have got a message about getting results.." << rec << " of them" << std::endl;

		// Don't forget to release memory not under the management of Node
		delete con;
		delete stmt;

		// Return the resulting array
		return scope.Close( Handle<Array>::Cast( records ) );
	    }
	    else
	    {
		Local<Integer> affected = Integer::New( stmt->getUpdateCount() );

		std::cout << "We didn't gets no results from our noprep query..." << std::endl;
		// Don't forget to release memory not under the management of Node
		delete con;
		delete stmt;

		// And return the count
		return scope.Close( affected );
	    }
	}catch( sql::SQLException &e )
	{
	    // Don't forget to release memory not under the management of node
	    delete con;
	    delete stmt;

	    std::cout << "Failed" << std::endl << "# ERR: SQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << "\n" << std::endl;
	    std::cout << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState " << e.getSQLState() << " )" << std::endl;
	}
    }
    else
    {
	try
	{
	    sql::Driver* driver;
	    sql::Connection* con;
	    sql::ResultSet* rset;
	    sql::ResultSetMetaData* rsmeta;
	    sql::PreparedStatement* pstmt;

	    String::AsciiValue hav( dvr->host_ ), dbav( dvr->db_ ), userav( dvr->user_ ), passav( dvr->password_ ), queryav( dvr->query_ );
	    std::string db = *dbav, host = *hav, user = *userav, pass = *passav, query = *queryav, tresult;
	    bool result = true;

	    // Create a connection
	    driver = get_driver_instance();
	    con = driver->connect( host, user, pass );
	    con->setSchema( db );
	    pstmt = con->prepareStatement( query );

	    // We don't actually need to dynamically set placeholder values using a model, since we can get column type information based on keys by
	    // using the resultset metadata object from pstmt, even before executing the query :) :
	    Local<Array> keys = dvr->phmap_->GetOwnPropertyNames();		// Gets us all the keys in the placeholder array

	    // If any values are set we need to prepare them.
	    if( keys->Length() > 0 )
	    {
		for( int i = 0, l = keys->Length(); i < l; i++ )
		{
		    int ti = ( i + 1 );

		    // Get the keys for which to set the placeholders values
		    Local<String> k = keys->Get( i )->ToString();

		    // Set the placeholder's values
		    String::AsciiValue phav( dvr->phmap_->Get( k ) );

		    // Convert the AsciiValue to a String
		    std::string ph = *phav;

		    // Get the column definition
		    Local<Array> defarr = Local<Array>::Cast( dvr->model_->Get( k ) );
		    String::AsciiValue cdefav( defarr->Get( KWAERI_COLUMN_TYPE ) );
		    std::string cdef = *cdefav;

		    if( cdef == std::string( "int" ) )
		    {
			int ival = atoi( ph.c_str() );
			pstmt->setInt( ti, ival );
		    }
		    else if( cdef == std::string( "text" ) || cdef == std::string( "varchar" ) )
		    {
			pstmt->setString( ti, ph.c_str() );
		    }
		    else
		    {
			// Error, unrecognized type
			std::cout << "Error: Unrecognized type, driver.cpp | 1073" << std::endl;
		    }
		}
	    }

	    // We actually do not need to use a model to properly fetch result data, since we can use getMetaData to determine types, etc.  We can also do this,  using prepared statements, before
	    // actually executing the query :)
	    rsmeta = pstmt->getMetaData();

	    // Now let's figure out how many columns are in the resultset
	    int colcount = rsmeta->getColumnCount();
	    std::cout << "Column count: " << colcount << endl;

	    // Let's go ahead and execute our prepared statement
	    bool res = pstmt->execute();

	    // Fetch our results
	    if( res )
	    {
		rset = pstmt->getResultSet();
		Local<Array> records = Array::New();
		int rec = 0;

		while( rset->next() )
		{
		    std::cout << "We gots results!" << std::endl;
		    // Each iteration brings another record, or set of columns present in the model which were specified to be selected.  What
		    // we want to do is allocate an array, each index containing an object which holds all the columns of this record.
		    Local<Object> record = Object::New();

		    for( int cc = 0, l = colcount; cc < l; cc++ )
		    {
			// Get the column name
			//Local<String> k = keys->Get( cc )->ToString();
			Local<String> k = String::New( rsmeta->getColumnLabel( cc + 1 ).c_str() );
			std::cout << "Column Label: " << k << std::endl;

			// Set the column data in the record
			std::string ctype = rsmeta->getColumnTypeName( cc + 1 ).c_str() );
			std::cout << "Column Type Name: " << ctype << std::endl;
			if( ctype == std::string( "int" ) )
			{
			    record->Set( k, Integer::New( rset->getInt( k->ToString()/* rsmeta->getColumnLabel( cc + 1 ).c_str() */ ) ) )
			}
			else if( ctype == std::string( "varchar" ) || ctype == std::string( "text" ) )
			{
			    Handle<Value> svh = String::New( rset->getString( k->ToString()/* rsmeta->getColumnLabel( cc + 1 ).c_str() */ ).c_str() );
			    String::AsciiValue sav( svh );
			    record->Set( k, String::New( *sav ) );
			}
			else
			{
			    record->Set( k, String::New( "Error: Unknown column type." ) );
			}
		    }

		    records->Set( rec, record );
		    rec++;
		}

		std::cout << "We should have got a message about getting results.." << rec << " of them" << std::endl;

		// Don't forget to release memory not under the management of Node
		delete con;
		delete pstmt;

		// Return the resulting array
		return scope.Close( Handle<Array>::Cast( records ) );
	    }
	    else
	    {
		Local<Integer> affected = Integer::New( pstmt->getUpdateCount() );

		std::cout << "We didn't gets no results" << std::endl;
		// Don't forget to release memory not under the management of Node
		delete con;
		delete pstmt;

		// And return the count
		return scope.Close( affected );
	    }
	}catch( sql::SQLException &e )
	{
	    // Don't forget to release memory not under the management of node
	    delete con;
	    delete pstmt;

	    std::cout << "Failed" << std::endl << "# ERR: SQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << "\n" << std::endl;
	    std::cout << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState " << e.getSQLState() << " )" << std::endl;
	}
    }
}


/**
 * Gets the host_ string
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::GetConnection( const Arguments& args )
{
	HandleScope scope;

	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Convert the v8::Persistent<v8::String> to std::string
	String::AsciiValue hostav( dvr->host_->ToString() ), portav( dvr->port_->ToString() ), dbav( dvr->db_->ToString() ), userav( dvr->user_->ToString() ), passav( dvr->password_->ToString() );
	std::string host = *hostav, port = *portav, db = *dbav, user = *userav, password = *passav;

	std::string conn = "Connection: \nHost: " + host + ",\nPort: " + port + ",\nDatabase: " + db + ",\nUser: " + user + ",\nPassword: " + password + "\n";
	std::cout << conn << std::endl;

	// Convert the new std::string back to a v8::String of sorts
	Handle<Value> connh = String::New( conn.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue connav( connh );

	// Return a string containing all the connection info for this driver instance
	return scope.Close( String::New( *connav ) );
}


/**
 * Gets the query_ string
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::GetQuery( const Arguments& args )
{
	HandleScope scope;

	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Return the full query string to JavaScript
	return scope.Close( dvr->query_->ToString() );
}


/**
 * Resets the query_ string, phmap_ registry, and all flags
 *
 * @param const Arguments&		args		Arguments passed during invocation
 *
 * @return	String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Reset( const Arguments& args )
{
	HandleScope scope;

	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Reset the query string
	dvr->query_ = Persistent<String>::New( String::New("") );

	// Reset the model
	dvr->model_ = Persistent<Object>::New( Object::New() );

	// Reset the place holder map
	dvr->phmap_ = Persistent<Object>::New( Object::New() );

	// Reset the type flag
	dvr->type_ = Persistent<Integer>::New( Integer::New( KWAERI_EMPTY ) );

	// Return the query string to JavaScript
	return scope.Close( dvr->query_->ToString() );
}
