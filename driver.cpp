/**
 * package: nodamysql
 * version:  0.0.1
 * author:  Richard B. Winters <a href="mailto:rik@mmogp.com">rik At MassivelyModified</a>
 * copyright: 2013-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// INCLUDES
#include <v8.h>
#include <node.h>
#include <iostream>
#include <string>
#include "resource.h"
#include "driver.h"

// MySQL Includes ( Also targets Boost C++ Libraries )
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
Driver::Driver(
					Persistent<String> host,
					Persistent<String> port,
					Persistent<String> db,
					Persistent<String> user,
					Persistent<String> password,
					Persistent<Integer> type,
					Persistent<Object> phmap,
					Handle<Boolean> mapped,
					Local<String> query,
					Handle<Boolean> prepared
				) :
					host_( host ),
					port_( port ),
					db_( db ),
					user_( user ),
					password_( password ),
					type_( type ),
					phmap_( Persistent<Object>::New( phmap->ToObject() ) ),
					mapped_( mapped ),
					query_( query->ToString() ),
					prepared_( prepared )
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
	tpl->InstanceTemplate()->SetInternalFieldCount( 10 );

	// Prepare the accessors for our dynamic variables which are not static
	//tpl->InstanceTemplate().SetAccessor( String::New( "host" ), GetHost, SetHost );

	// Prototype(s) of our methods for v8
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Query" ), FunctionTemplate::New( Query )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Select" ), FunctionTemplate::New( Select )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Insert" ), FunctionTemplate::New( Insert )->GetFunction() );
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
		Local<String> driver = args[0]->IsUndefined() ? String::New( "mysql" ) : args[0]->ToString();
		Local<Object> config;

		if( args[1]->IsUndefined() )
		{
			config = Object::New();

			if( driver == String::New( "mysql" ) )
			{
				config->Set( String::NewSymbol( "host" ), String::New( "localhost" ) );
				config->Set( String::NewSymbol( "port" ), String::New( "3306" ) );
				config->Set( String::NewSymbol( "db" ), String::New( "test" ) );
				config->Set( String::NewSymbol( "user" ), String::New( "test" ) );
				config->Set( String::NewSymbol( "password" ), String::New( "testpass" ) );
			}
		}
		else
		{
			config = args[1]->ToObject();
		}

		Driver* dvr = new Driver(
									Persistent<String>::New( config->Get( String::New( "host" ) )->ToString() ),
									Persistent<String>::New( config->Get( String::New( "port" ) )->ToString() ),
									Persistent<String>::New( config->Get( String::New( "db" ) )->ToString() ),
									Persistent<String>::New( config->Get( String::New( "user" ) )->ToString() ),
									Persistent<String>::New( config->Get( String::New( "password" ) )->ToString() ),
									Persistent<Integer>::New( Integer::New( 0 ) ),
									Persistent<Object>::New( Object::New() ),
									Handle<Boolean>( v8::False() ),
									String::New( "" ),
									Handle<Boolean>( v8::False() )
								);

		dvr->Wrap( args.This() );

		return args.This();
	}
	else
	{
		// Invoked as plain function 'nodamysql(...)', turn it into a construct call
		const int argc = 10;

		Local<Object> config;

		if( args[1]->IsUndefined() )
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
			config = args[1]->ToObject();
		}

		Local<Value> argv[argc] = {
										config->Get( String::New( "host" ) ),
										config->Get( String::New( "port" ) ),
										config->Get( String::New( "db" ) ),
										config->Get( String::New( "user" ) ),
										config->Get( String::New( "pass" ) ),
										Integer::New( 0 ),
										Object::New(),
										Local<Boolean>::New( v8::False() ),
										String::New( "" ),
										Local<Boolean>::New( v8::False() )
								  };

		return scope.Close( constructor->NewInstance( argc, argv ) );
	}
}


/**
 * Executes a supplied query statement (Not prepared!)
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Query( const Arguments& args )
{
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string select = *av;

	// Build our query string part
	std::string qp = std::string( "SELECT " );
	qp += select;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the entire object to allow chaining, results can be checked there
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
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
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string select = *av;

	// Build our query string part
	std::string qp = std::string( "SELECT " );
	qp += select;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the entire object to allow chaining, results can be checked there
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
	rdvr->Wrap( args.This() );
	return scope.Close( args.This() );
}


/**
 * Builds a INSERT clause
 *
 * @param const Arguments&		Arguments passed during invocation
 *
 * @return String		The current query string
 *
 * @since 0.0.1
 */
Handle<Value> Driver::Insert( const Arguments& args )
{
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string insert = *av;

	// Build our query string part
	std::string qp = std::string( "INSERT INTO " );
	qp += insert;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the entire object to allow chaining, results can be checked there
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string select = *av;

	// Build our query string part
	std::string qp = std::string( "SELECT " );
	qp += select;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the entire object to allow chaining, results can be checked there
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string select = *av;

	// Build our query string part
	std::string qp = std::string( "SELECT " );
	qp += select;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the entire object to allow chaining, results can be checked there
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() || !args[0]->IsObject() )
	{
		std::cout << "Error: Escaped" << std::endl;
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( dvr->query_ );
	std::string eqs = *av;

	// Build our query string part
	std::string qp = " WHERE ( ";

	// The supplied argument should be an object with columns and values specified for the where clause
	Handle<Object> where = args[0]->ToObject();				// Could be args[0].As<Object>();
	Local<Array> keys = where->GetOwnPropertyNames();		// Gets us all the keys in an array
	for( int i = 0, l = keys->Length(); i < l; i++ )
	{
		// Get the keys to build the where clause, and replace values with question mark
		Local<String> k = keys->Get( i )->ToString();

		// Store the values and conditions for the placeholders in the order they are added to the clause
		dvr->phmap_->Set( k, where->Get( k )->ToString() );

		String::AsciiValue kav( k );
		std::string kv = *kav;

		if( i != 0 )
		{
			qp += " AND ";
		}

		qp += kv + "=?";
	}

	qp += " )";

	std::cout << "Prints Part: " << qp << std::endl;

	// Attach the new query part to the existing query string
	eqs += qp;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( eqs.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	dvr->query_ = String::New( *qpav );

	// Return the entire object to allow chaining, results can be checked there
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string db = *av;

	// Build our query string part
	std::string qp = std::string( " JOIN " );
	qp += db;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the resulting query string to the user so they can verify
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
	// Driver* rdvr = new Driver( dvr->host_->ToString(), dvr->port_->ToString(), dvr->db_->ToString(), dvr->user_->ToString(), dvr->password_->ToString(), dvr->phmap_->ToObject(), dvr->query_->ToString() );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// This is how we convert a JavaScript string to a std::string (c++)
	String::AsciiValue av( args[0] );
	std::string on = *av;

	// Build our query string part
	std::string qp = std::string( " ON ( " );
	qp += on;
	qp += std::string( " )" );

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the resulting query string to the user so they can verify
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
	// Driver* rdvr = new Driver( dvr->host_->ToString(), dvr->port_->ToString(), dvr->db_->ToString(), dvr->user_->ToString(), dvr->password_->ToString(), dvr->phmap_->ToObject(), dvr->query_->ToString() );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
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

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// Build our query string part
	std::string qp = std::string( " LIMIT " );
	qp += from;
	qp += std::string( ", " );
	qp += to;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the resulting query string to the user so they can verify
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
	// Driver* rdvr = new Driver( dvr->host_->ToString(), dvr->port_->ToString(), dvr->db_->ToString(), dvr->user_->ToString(), dvr->password_->ToString(), dvr->phmap_->ToObject(), dvr->query_->ToString() );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Fetch our arguments and define default behavior for incorrect arguments or no arguments
	if( args[0]->IsUndefined() )
	{
		Driver* rdvre = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
		rdvre->Wrap( args.This() );
		return scope.Close( args.This() );
	}

	std::string order;
	String::AsciiValue avorder( args[0] );
	order = *avorder;

	// Local<String> XXX = String::Concat() only let's me concat twice, I do not understand why
	// but because of this we will use standard c++ practices to avoid degradation and/or other issues

	// Build our query string part
	std::string qp = std::string( " ORDER BY " );
	qp += order;

	std::cout << "Prints Part: " << qp << std::endl;

	// This is how we convert an std::string back to a v8::Handle<v8::Value>. Just like with Concat,
	// all of the suggested methods for converting a std::string to v8::Local<v8::String> were
	// ineffective for me.  This is the only way I've gotten it to work properly without degradation
	// or other error(s).
	Handle<Value> qph = String::New( qp.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue qpav( qph );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	Local<String> qps = String::New( *qpav );

	dvr->query_ = String::Concat( dvr->query_->ToString(), qps->ToString() );

	// Return the resulting query string to the user so they can verify
	Driver* rdvr = new Driver( Persistent<String>::New( dvr->host_->ToString() ), Persistent<String>::New( dvr->port_->ToString() ), Persistent<String>::New( dvr->db_->ToString() ), Persistent<String>::New( dvr->user_->ToString() ), Persistent<String>::New( dvr->password_->ToString() ), dvr->type_, dvr->phmap_, Handle<Boolean>( dvr->mapped_ ), dvr->query_->ToString(), Handle<Boolean>( dvr->prepared_ ) );
	// Driver* rdvr = new Driver( dvr->host_->ToString(), dvr->port_->ToString(), dvr->db_->ToString(), dvr->user_->ToString(), dvr->password_->ToString(), dvr->phmap_->ToObject(), dvr->query_->ToString() );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	std::cout << "Made it inside the execute statement." << std::endl;

	//try
	//{
		sql::Driver* driver;
		sql::Connection* con;
		//sql::Statement* stmt;
		sql::ResultSet* res;
		sql::PreparedStatement* pstmt;

		String::AsciiValue hav( dvr->host_ ), dbav( dvr->db_ ), userav( dvr->user_ ), passav( dvr->password_ ), queryav( dvr->query_ );
		std::string company = "mmod", db = *dbav, host = *hav, user = *userav, pass = *passav, query = *queryav, tresult, userFirst, userLast, userEmail;
		bool result = false;

		// Create a connection
		std::cout << "Creating database driver...";
		driver = get_driver_instance();
		std::cout << "Success!" << std::endl;

		std::cout << "Connecting to database server...";
		//con = driver->connect( "127.0.0.1", "root", "^Rkitek720$" );
		con = driver->connect( host, user, pass );
		std::cout << "Success!" << std::endl;

		std::cout << "Connecting to database \"mmod\"...";
		// Connect to the database
		//con->setSchema( "mmod" );
		con->setSchema( db );
		std::cout << "Success!" << std::endl;

		//delete stmt;

		std::cout << "Preparing query...";
		//pstmt = con->prepareStatement( "SELECT `id`, `first` FROM `users` WHERE company=? AND username=? AND password=?" );
		pstmt = con->prepareStatement( query );

		int userId;

		//company = "mmod";
		//user = "rik";
		//pass = "testpass";
		Local<Array> keys = dvr->phmap_->GetOwnPropertyNames();		// Gets us all the keys in an array
		for( int i = 0, l = keys->Length(); i < l; i++ )
		{
			int ti = ( i + 1 );

			// Get the keys for which to set the placeholders values
			Local<String> k = keys->Get( i )->ToString();

			// Set the placeholders values
			String::AsciiValue phav( dvr->phmap_->Get( k ) );
			std::string ph = *phav;

			// Very boring example
			if( i == 0 )
			{
				pstmt->setString( ti, ph.c_str() );
			}else if( i == 1 )
			{
				int ival = atoi( ph.c_str() );
				pstmt->setInt( ti, ival );
			}
		}

		//pstmt->setString( 1, company.c_str() );
		//pstmt->setString( 2, user.c_str() );
		//pstmt->setString( 3, pass.c_str() );

		std::cout << "Success!" << std::endl;

		std::cout << "Executing prepared statement...";
		res = pstmt->executeQuery();

		// Fetch our results
		while( res->next() )
		{
			userId = res->getInt( "id" );
			userFirst = res->getString( "first" );
			userLast = res->getString( "last" );
			userEmail = res->getString( "email" );

			tresult = userId + ", " + userFirst + ", " + userLast + ", " + userEmail;
			result = true;
		}

		if( result )
		{
			std::cout << "Success: " << tresult << std::endl;
		}else
		{
			std::cout << "Failed" << std::endl;
			return scope.Close( String::New( "Failed" ) );
		}


	//}catch( sql::SQLException &e )
	//{
	//	std::cout << "Failed" << std::endl << "# ERR: SQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << "\n" << std::endl;
	//	std::cout << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState " << e.getSQLState() << " )" << std::endl;
	//}

	// Convert the new std::string back to a v8::String of sorts
	Handle<Value> rh = String::New( tresult.c_str() );

	// Convert the v8::Handle<v8::Value> to a v8::String::AsciiValue
	String::AsciiValue rav( rh );

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	//Local<String> conns = String::New( *connav );

	// Return the resulting query string to the user so they can verify
	return scope.Close( String::New( *rav ) );
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
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

	// Now we can use the v8::String::AsciiValue to properly instantiate a new v8::Local<v8::String>
	//Local<String> conns = String::New( *connav );

	// Return the resulting query string to the user so they can verify
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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// For testing convert the v8::Handle<v8::String> to a v8::String::AsciiValue
	//String::AsciiValue av( dvr->query_->ToString() );

	// And then to a std::string
	//std::string val( *av, av.length() );
	//std::cout << "Prints GetQuery: " << val << std::endl;		// Console dump

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
	// Always declare the HandleScope
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// For testing convert the v8::Handle<v8::String> to a v8::String::AsciiValue
	//String::AsciiValue av( dvr->query_->ToString() );

	// And then to a std::string
	//std::string val( *av, av.length() );
	//std::cout << "Prints GetQuery: " << val << std::endl;		// Console dump

	// Return the full query string to JavaScript
	return scope.Close( dvr->query_->ToString() );
}
