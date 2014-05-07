/**
 * package: nodamysql
 * version:  0.0.1
 * author:  Richard B. Winters <a href="mailto:rik@mmogp.com">rik At MassivelyModified</a>
 * copyright: 2013-2014 Richard B. Winters
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// INCLUDES
#include <node.h>
#include <driver.h>

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
Driver::Driver( String::AsciiValue host, String::AsciiValue port, String::AsciiValue db, String::AsciiValue user, String::AsciiValue password ) : host_( host ), port_( port ), db_( db ), user_( user ), password_( password )
{
	query_ = "";
	phmap_ = Object::New();
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
	tpl->InstanceTemplate()->SetInternalFieldCount( 6 );		// We probably need to change this to 6 (db, host, port, user, pass, query)

	// Prototype(s) of our methods for v8
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Select" ) ), FunctionTemplate::New( Select )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Where" ) ), FunctionTemplate::New( Where )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Join" ) ), FunctionTemplate::New( Join )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Limit" ) ), FunctionTemplate::New( Limit )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Order" ) ), FunctionTemplate::New( Order )->GetFunction() );
	tpl->PrototypeTemplate()->Set( String::NewSymbol( "Execute" ) ), FunctionTemplate::New( Execute )->GetFunction() );

	constructor = Persistent<Function>::New( tpl->GetFunction() );
	exports->Set( String::NewSymbol( "Driver" ), constructor );
}


/**
 *  Returns a new instance of the Driver object
 *
 *  @param const Arguments&		Arguments passed during invocation
 *
 *  @return		Driver		New instance of a Driver object
 *
 *  @since 0.0.1
 */
Handle<Value> Driver::New( const Arguments& args )
{
	HandleScope scope;

	if( args.IsConstructCall() )
	{
		// Invoked as constructor: 'new nodamysql(...)'
		String::AsciiValue host = args[0]->IsUndefined() ? "localhost" : args[0]->ToString();
		String::AsciiValue port = args[1]->IsUndefined() ? "3306" : args[1]->NumberValue();
		String::AsciiValue db = args[2]->IsUndefined() ? "test" : args[2]->ToString();
		String::AsciiValue user = args[3]->IsUndefined() ? "test" : args[3]->ToString();
		String::AsciiValue password = args[4]->IsUndefined() ? "password" : args[4]->ToString();
		String::AsciiValue query = args[5]->IsUndefined() ? String::New("") : args[5]->ToString();	// Might not need this

		Driver* dvr = new Driver( host, port, db, user, password );
		dvr->query_ = query;	// Or this
		drv->Wrap( args.This() );

		return args.This();
	}
	else
	{
		// Invoked as plain function 'nodamysql(...)', turn it into a construct call
		const int argc = 5;
		Local<Value> argv[argc] = { args[0], args[1], args[2], args[3], args[4] };

		return scope.Close( constructor->NewInstance( argc, argv ) );
	}
}


/**
 *  Builds a SELECT clause
 *
 *  @param const Arguments&		Arguments passed during invocation
 *
 *  @return String		The current query string
 *
 *  @since 0.0.1
 */
Handle<Value> Driver::Select( const Arguments& args )
{
	HandleScope scope;

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Don't forget to fetch our arguments
	String::AsciiValue select = args[0]->ToString();

	// Build our query string
	dvr->query_ = String::New( "SELECT " + select );

	// Return the resulting query string to the user so they can verify
	return scope.Close( String::New( drv->query_ ) );
}


/**
 *  Builds a WHERE clause
 *
 *  @param const Arguments&		Arguments passed during invocation
 *
 *  @return String		The current query string
 *
 *  @since 0.0.1
 */
Handle<Value> Driver::Where( const Arguments& args )
{
	HandleScope scope;

		// Unwrap the object
		Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

		// Don't forget to fetch our arguments
		if( args[0]->IsUndefined() || !args[0]->IsObject() )
		{
			return scope.Close( String::New( drv->query_ ) )
		}

		drv->query_ = String::New( drv->query_ + " WHERE" );

		// he supplied argument should be an object with columns and values specified for the where clause
		Local<Object> where = args[0]->ToObject();				// Could be args[0].As<Object>();
		Local<Array> keys = where->GetOwnPropertyNames();		// Gets us all the keys in an array ofc
		for( int i = 0, l = props->length(); i < l; i++ )
		{
			// Add the key and the values to our local map so they can be provided during executing of the query.
			Local<String> k = keys->Get( i )->ToString() );
			drv->phmap_->Set( k, where->Get( k ) );

			if( i == 0 )
			{
				drv->query_ = String::New( drv->query_ + " " + k + "=?");
			}else
			{
				drv->query_ = String::New( drv->query_ + " AND " + k + "=?")
			}
		}

		// Return the resulting query string to the user so they can verify
		return scope.Close( String::New( drv->query_ ) );
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

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Don't forget to fetch our arguments
	String::AsciiValue db = args[0]->IsUndefined() ? "test" : args[0]->ToString();

	// Build our query string
	dvr->query_ = String::New( drv->query_ + " JOIN (" + db + ")" );

	// Return the resulting query string to the user so they can verify
	return scope.Close( String::New( drv->query_ ) );
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

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Don't forget to fetch our arguments
	if( args[0]->IsUndefined() )
	{
		return scope.Close( String::New( drv->query_ ) )
	}

	String::AsciiValue on = args[0]->ToString();

	// Build our query string
	dvr->query_ = String::New( drv->query_ + " ON (" + on + ")" );

	// Return the resulting query string to the user so they can verify
	return scope.Close( String::New( drv->query_ ) );
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

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Don't forget to fetch our arguments
	if( args[0]->IsUndefined() )
	{
		return scope.Close( String::New( drv->query_ ) );
	}

	// We allow the invoker to specify 1 or two arguments for ease of use
	int from, to;
	if( args[1]->IsUndefined() )
	{
		from = 0;
		to = args[0]->NumberValue();
	}
	else
	{
		from = args[0]->NumberValue();
		to = args[1]->NumberValue();
	}

	// Build our query string
	dvr->query_ = String::New( drv->query_ + " LIMIT " + from + ", " + to );

	// Return the resulting query string to the user so they can verify
	return scope.Close( String::New( drv->query_ ) );
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

	// Unwrap the object
	Driver* dvr = ObjectWrap::Unwrap<Driver>( args.This() );

	// Don't forget to fetch our arguments
	if( args[0]->IsUndefined() )
	{
		return scope.Close( String::New( drv->query_ ) );
	}

	String::AsciiValue order = args[0]->ToString();

	// Build our query string
	dvr->query_ = String::New( drv->query_ + " ORDER BY " + order );

	// Return the resulting query string to the user so they can verify
	return scope.Close( String::New( drv->query_ ) );
}
















