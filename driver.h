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


// Here we define macros soley for supporting different versions of node-js and v8.  Eventually we may move this to a custom namespace.

// Node 0.11.8 etc
#if( NODE_MODULE_VERSION == 13 )
#define nkScope v8::Isolate* isolate = v8::Isolate::GetCurrent(); \
              v8::HandleScope scope( isolate )
#define nkException( str ) v8::ThrowException( v8::Exception::TypeError( v8::String::New( str ) ) )
#define nkArguments v8::FunctionCallbackInfo<v8::Value>
#define nkReturnType void
#define nkReturn

// Node 0.11.13 etc
#elif( NODE_MODULE_VERSION > 13 )
#define nkScope v8::Isolate* isolate = v8::Isolate::GetCurrent(); \
              v8::HandleScope scope( isolate )
#define nkException( str ) isolate->ThrowException( v8::Exception::TypeError( v8::String::NewFromUtf8( isolate, str ) ) )
#define nkArguments v8::FunctionCallbackInfo<v8::Value>
#define nkReturnType void
#define nkReturn

// Node 0.10.x and below
#else
#define nkScope v8::HandleScope scope
#define nkException( str ) return v8::ThrowException( v8::Exception::TypeError( v8::String::New( str ) ) )
#define nkArguments v8::Arguments
#define nkReturnType v8::Handle<v8::Value>
#define nkReturn( var ) return scope.Close( var )
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
		static nkReturnType New( const nkArguments& args );
		static nkReturnType Query( const nkArguments& args );
		static nkReturnType Select( const nkArguments& args );
		static nkReturnType Insert( const nkArguments& args );
		static nkReturnType Values( const nkArguments& args );
		static nkReturnType Update( const nkArguments& args );
		static nkReturnType Delete( const nkArguments& args );
		static nkReturnType Where( const nkArguments& args );
		static nkReturnType Join( const nkArguments& args );
		static nkReturnType On( const nkArguments& args );
		static nkReturnType Limit( const nkArguments& args );
		static nkReturnType Order( const nkArguments& args );
		static nkReturnType Execute( const nkArguments& args );
		static nkReturnType ExecuteQuery( const nkArguments& args );
		static nkReturnType GetConnection( const nkArguments& args );
		static nkReturnType GetQuery( const nkArguments& args );
		static nkReturnType Reset( const nkArguments& args );

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
