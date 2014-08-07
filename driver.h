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
		static v8::Handle<v8::Value> New( const v8::Arguments& args );
		static v8::Handle<v8::Value> Query( const v8::Arguments& args );
		static v8::Handle<v8::Value> Select( const v8::Arguments& args );
		static v8::Handle<v8::Value> Insert( const v8::Arguments& args );
		static v8::Handle<v8::Value> Values( const v8::Arguments& args );
		static v8::Handle<v8::Value> Update( const v8::Arguments& args );
		static v8::Handle<v8::Value> Delete( const v8::Arguments& args );
		static v8::Handle<v8::Value> Where( const v8::Arguments& args );
		static v8::Handle<v8::Value> Join( const v8::Arguments& args );
		static v8::Handle<v8::Value> On( const v8::Arguments& args );
		static v8::Handle<v8::Value> Limit( const v8::Arguments& args );
		static v8::Handle<v8::Value> Order( const v8::Arguments& args );
		static v8::Handle<v8::Value> Execute( const v8::Arguments& args );
		static v8::Handle<v8::Value> ExecuteQuery( const v8::Arguments& args );
		static v8::Handle<v8::Value> GetConnection( const v8::Arguments& args );
		static v8::Handle<v8::Value> GetQuery( const v8::Arguments& args );
		static v8::Handle<v8::Value> Reset( const v8::Arguments& args );

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
