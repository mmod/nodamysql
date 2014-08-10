/**
 * package: nodamysql
 * sub-package: driver/header
 * author:  Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik At MMOGP</a>
 * copyright: 2011-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


#ifndef DRIVER_H
#define DRIVER_H


// INCLUDES
#include <v8.h>
#include <node.h>


// Here we borrow some code for now from another repository, which will help us build across multiple node[v8] versions
// Node 0.11.8 etc
#if( NODE_MODULE_VERSION == 13 )
#define SCOPE Isolate* isolate = Isolate::GetCurrent(); \
              HandleScope scope( isolate )
#define EXCEPTION( str ) ThrowException( Exception::TypeError( String::New( str ) ) )
#define ARGUMENTS v8::FunctionCallbackInfo<Value>
#define RETURNTYPE void
#define RETURN

// Node 0.11.13 etc
#elif( NODE_MODULE_VERSION > 13 )
#define SCOPE Isolate* isolate = Isolate::GetCurrent(); \
              HandleScope scope( isolate )
#define EXCEPTION( str ) isolate->ThrowException( Exception::TypeError( String::NewFromUtf8( isolate, str ) ) )
#define ARGUMENTS v8::FunctionCallbackInfo<Value>
#define RETURNTYPE void
#define RETURN

// Node 0.12 and below
#else
#define SCOPE HandleScope scope
#define EXCEPTION( str ) return ThrowException( Exception::TypeError( String::New( str ) ) )
#define ARGUMENTS v8::Arguments
#define RETURNTYPE Handle<Value>
#define RETURN return scope.Close( Undefined() )
#endif


/**
 * MySQL C++ Connector Driver
 *
 * Allows one to work with MySQL using JavaScript in Node.js.  Works using the MySQL C++ Connector current
 * version.  This driver provides intuitive facilities for building and executing simple and advanced queries.
 *
 * @since 0.0.1
 */
class Driver : public node::ObjectWrap
{
	public:
		static void Init( v8::Handle<v8::Object> exports );

	private:
		explicit Driver(
							v8::Persistent<v8::String> host = v8::Persistent<v8::String>::New( v8::String::New( "localhost" ) ),
							v8::Persistent<v8::String> port = v8::Persistent<v8::String>::New( v8::String::New( "3306" ) ),
							v8::Persistent<v8::String> db = v8::Persistent<v8::String>::New( v8::String::New( "test" ) ),
							v8::Persistent<v8::String> user = v8::Persistent<v8::String>::New( v8::String::New( "test" ) ),
							v8::Persistent<v8::String> password = v8::Persistent<v8::String>::New( v8::String::New( "password" ) ),
							v8::Persistent<v8::Object> model = v8::Persistent<v8::Object>::New( v8::Object::New() ),
							v8::Persistent<v8::Boolean> modeled = v8::Persistent<v8::Boolean>::New( v8::False() ),
							v8::Persistent<v8::Integer> type = v8::Persistent<v8::Integer>::New( v8::Integer::New( 0 ) ),
							v8::Persistent<v8::Integer> prepared = v8::Persistent<v8::Integer>( v8::Integer::New( 0 ) ),
							v8::Persistent<v8::Array> phmap = v8::Persistent<v8::Array>::New( v8::Array::New() ),
							v8::Persistent<v8::Boolean> mapped = v8::Persistent<v8::Boolean>::New( v8::False() ),
							v8::Persistent<v8::String> query = v8::Persistent<v8::String>::New( v8::String::New( "" ) )
						);
		~Driver();

		static v8::Persistent<v8::Function> constructor;
		static v8::Handle<v8::Value> New( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Query( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Select( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Insert( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Values( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Update( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Delete( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Where( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Join( const ARGUMENTS& args );
		static v8::Handle<v8::Value> On( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Limit( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Order( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Execute( const ARGUMENTS& args );
		static v8::Handle<v8::Value> ExecuteQuery( const ARGUMENTS& args );
		static v8::Handle<v8::Value> GetConnection( const ARGUMENTS& args );
		static v8::Handle<v8::Value> GetQuery( const ARGUMENTS& args );
		static v8::Handle<v8::Value> Reset( const ARGUMENTS& args );

	public:
		v8::Persistent<v8::String> host_;
		v8::Persistent<v8::String> port_;
		v8::Persistent<v8::String> db_;
		v8::Persistent<v8::String> user_;
		v8::Persistent<v8::String> password_;
		v8::Persistent<v8::Object> model_;
		v8::Persistent<v8::Boolean> modeled_;
		v8::Persistent<v8::Integer> type_;
		v8::Persistent<v8::Integer> prepared_;
		v8::Persistent<v8::Array> phmap_;
		v8::Persistent<v8::Boolean> mapped_;
		v8::Persistent<v8::String> query_;
};

#endif
