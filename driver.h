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
#define nkIsolation v8::Isolate* isolate = v8::Isolate::GetCurrent();
#define nkScope v8::HandleScope scope( isolate )
#define nkIsolateOS v8::Isolate::GetCurrent()
#define nkIsolate isolate
#define nkPreIsolatedOS  v8::Isolate::GetCurrent(),
#define nkPreIsolated  isolate,
#define nkPostIsolatedOS  , v8::Isolate::GetCurrent()
#define nkPostIsolated  , isolate
#define nkNewV8PObject( var ) v8::Handle<v8::Object>::Cast( var )
#define nkNewV8Object( var ) var
#define nkNewV8PArray( var ) v8::Handle<v8::String>::Cast( var )
#define nkNewV8Array( var ) var
#define nkNewV8PStringOS( var ) obj.Reset( v8::Isolate::GetCurrent(), v8::Handle<v8::String>::Cast( v8::String::NewFromUtf8( v8::Isolate::GetCurrent(), var ) ) )
#define nkNewV8StringOS( var ) v8::String::NewFromUtf8( v8::Isolate::GetCurrent(), var )
#define nkNewV8PString( var ) v8::Handle<v8::String>::Cast( v8::String::NewFromUtf8( nkIsolate, var ) )
#define nkNewV8String( var ) v8::String::NewFromUtf8( nkIsolate, var )
#define nkNewV8PInteger( var ) v8::Handle<v8::Integer>::Cast( var )
#define nkNewV8Integer( var ) var
#define nkNewV8PBoolean( var ) v8::Handle<v8::Boolean>::Cast( var )
#define nkNewV8Boolean( var ) var
#define nkException( str ) v8::ThrowException( v8::Exception::TypeError( v8::String::New( str ) ) )
#define nkArguments v8::FunctionCallbackInfo<v8::Value>
#define nkReturnType void
#define nkReturn( var ) return scope.Close( var )

// Node 0.11.13 etc
#elif( NODE_MODULE_VERSION > 13 )
#define nkIsolation v8::Isolate* isolate = v8::Isolate::GetCurrent();
#define nkScope v8::HandleScope scope( isolate )
#define nkIsolateOS v8::Isolate::GetCurrent()
#define nkIsolate isolate
#define nkPreIsolatedOS  v8::Isolate::GetCurrent(),
#define nkPreIsolated  isolate,
#define nkPostIsolatedOS  , v8::Isolate::GetCurrent()
#define nkPostIsolated  , isolate
#define nkNewV8PObject( var ) v8::Handle<v8::Object>::Cast( var )
#define nkNewV8Object( var ) var
#define nkNewV8PArray( var ) v8::Handle<v8::String>::Cast( var )
#define nkNewV8Array( var ) var
#define nkNewV8PStringOS( var ) v8::Handle<v8::String>::Cast( v8::String::NewFromUtf8( v8::Isolate::GetCurrent(), var ) )
#define nkNewV8StringOS( var ) v8::String::NewFromUtf8( v8::Isolate::GetCurrent(), var )
#define nkNewV8PString( var ) v8::Handle<v8::String>::Cast( v8::String::NewFromUtf8( nkIsolate, var ) )
#define nkNewV8String( var ) v8::String::NewFromUtf8( nkIsolate, var )
#define nkNewV8PInteger( var ) v8::Handle<v8::Integer>::Cast( var )
#define nkNewV8Integer( var ) var
#define nkNewV8PBoolean( var ) v8::Boolean::New( var )
#define nkNewV8Boolean( var ) var
#define nkException( str ) nkIsolate->ThrowException( v8::Exception::TypeError( v8::String::NewFromUtf8( nkIsolate, str ) ) )
#define nkArguments v8::FunctionCallbackInfo<v8::Value>
#define nkV8StringType v8::Handle<v8::String>
#define nkSetPS( ivar, var ) ivar.Reset( var )
#define nkV8ObjectType v8::Handle<v8::Object>
#define nkSetPO( ivar, var ) ivar.Reset( var )
#define nkV8ArrayType v8::Handle<v8::Array>
#define nkSetPA( ivar, var ) ivar.Reset( var )
#define nkV8IntegerType v8::Handle<v8::Integer>
#define nkSetPI( ivar, var ) ivar.Reset( var )
#define nkV8BooleanType v8::Handle<v8::Boolean>
#define nkSetPB( ivar, var ) ivar.Reset( var )
#define nkReturnType void
#define nkReturn( var ) return scope.Close( var )

// Node 0.10.x and below
#else
#define nkIsolation
#define nkScope v8::HandleScope scope
#define nkIsolate
#define nkIsolateOS
#define nkPreIsolatedOS
#define nkPreIsolated
#define nkPostIsolatedOS
#define nkPostIsolated
#define nkNewV8PObject( var ) v8::Persistent<v8::Object>::New( var )
#define nkNewV8Object( var ) var
#define nkNewV8PArray( var ) v8::Persistent<v8::Array>::New( var )
#define nkNewV8Array( var ) v8::Persistent<v8::Array>::New( var )
#define nkNewV8PStringOS( var ) v8::Persistent<v8::String>::New( v8::String::New( var ) )
#define nkNewV8StringOS( var ) v8::String::New( var )
#define nkNewV8PString( var ) v8::Persistent<v8::String>::New( var )
#define nkNewV8String( var ) nkNewV8StringOS( var )
#define nkNewV8PInteger( var ) v8::Persistent<v8::Integer>::New( var )
#define nkNewV8Integer( var ) var
#define nkNewV8PBoolean( var ) v8::Persistent<v8::Boolean>::New( var )
#define nkNewV8Boolean( var ) var
#define nkException( str ) return v8::ThrowException( v8::Exception::TypeError( v8::String::New( str ) ) )
#define nkArguments v8::Arguments
#define nkV8StringType v8::Persistent<v8::String>
#define nkSetPS( ivar, var ) ivar( var )
#define nkV8ObjectType v8::Persistent<v8::Object>
#define nkSetPO( ivar, var ) ivar( var )
#define nkV8ArrayType v8::Persistent<v8::Array>
#define nkSetPA( ivar, var ) ivar( var )
#define nkV8IntegerType v8::Persistent<v8::Integer>
#define nkSetPI( ivar, var ) ivar( var )
#define nkV8BooleanType v8::Persistent<v8::Boolean>
#define nkSetPB( ivar, var ) ivar( var )
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
class Driver : public node::ObjectWrap {
	public:
		static void Init( v8::Handle<v8::Object> exports );

	private:
		explicit Driver(
							nkV8StringType host = nkNewV8PStringOS( "localhost" ),
							nkV8StringType port = nkNewV8PStringOS( "3306" ),
							nkV8StringType db = nkNewV8PStringOS( "test" ),
							nkV8StringType user = nkNewV8PStringOS( "test" ),
							nkV8StringType password = nkNewV8PStringOS( "password" ),
							nkV8ObjectType model = nkNewV8PObject( v8::Object::New( nkIsolateOS ) ),
							nkV8BooleanType modeled = nkNewV8PBoolean( v8::False( nkIsolateOS ) ),
							nkV8IntegerType type = nkNewV8PInteger( v8::Integer::New( nkPreIsolatedOS 0 ) ),
							nkV8IntegerType prepared = nkNewV8PInteger( v8::Integer::New( nkPreIsolatedOS 0 ) ),
							nkV8ArrayType phmap = nkNewV8PArray( v8::Array::New( nkIsolateOS ) ),
							nkV8BooleanType mapped = nkNewV8PBoolean( v8::False( nkIsolateOS ) ),
							nkV8StringType query = nkNewV8PStringOS( "" )
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
