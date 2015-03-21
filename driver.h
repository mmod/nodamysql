/**
 * package: nodamysql
 * sub-package: driver/header
 * author:  Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik At MMOGP</a>
 * copyright: 2011-2015 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


#ifndef DRIVER_H
#define DRIVER_H


// INCLUDES
#include <utility>      // For move semantics
#include <iostream>
#include <string>
#include <sstream>
#include <v8.h>
#include <node.h>
#include "resource.h"


// Here we define macros soley for supporting different versions of node-js and v8.  Eventually we may move this to a custom namespace.

// Node v0.11.8+
#if( NODE_MODULE_VERSION >= 13 )

// First let's include <node_object_wrap.h>
#include <node_object_wrap.h>

// We will use a different implementation of persisitent members in v0.12+ of Node.js
#define nkPersistentMemberType v8::UniquePersistent

// Introduced is the v8::Isolate, used in many functions and with many types
//
// OS = Out of scope - we'll want to avoid doing this in the future as v8::Isolate::GetCurrent() will be phased out.
#define nkIsolation( info ) v8::Isolate* isolate = info.GetIsolate();
#define nkIsolationOS v8::Isolate* isolate = v8::Isolate::GetCurrent();
#define nkIsolateOS v8::Isolate::GetCurrent()
#define nkIsolate isolate
#define nkPreIsolatedOS  v8::Isolate::GetCurrent(),
#define nkPreIsolated  isolate,
#define nkPostIsolatedOS  , v8::Isolate::GetCurrent()
#define nkPostIsolated  , isolate

// HandleScope is still around (we only ever call it after isolate is defined, so we're not bothering to make multiple macros)
#define nkScope v8::HandleScope scope( isolate );

// The way to create function templates changed, so we macro'd it
#define nkFunctionTemplate( var ) v8::FunctionTemplate::New( nkIsolate, var )

// This includes the manner in which we define the prototypes of our function templates' methods
#define nkSetPrototypeMethod( tpl, sym, var ) NODE_SET_PROTOTYPE_METHOD( tpl, sym, var );

// As well as how we set the constructor thanks to Persistent<T> type changes
#define nkSetConstructor( tpl ) constructor.Reset( nkIsolate, tpl->GetFunction() );

// And how we instantiate a new instance of our object using said constructor has changed
#define nkNewConstructor v8::Local<v8::Function> cons = v8::Local<v8::Function>::New( isolate, constructor );
#define nkNewFromConstructor cons->NewInstance( argc, argv )

// How we export the module/add-on in C++ has also changed
#define nkSetExports( var, constructor, tpl ) exports->Set( v8::String::NewFromUtf8( isolate, var ), tpl->GetFunction() );

// P = Persistent, OS = Out of Scope, S = symbol-type
//
// In the future, v8::Isolate::GetCurrent() will need to be replaced; the macro should be passed the local isolate

// Object
#define nkNewV8PObjectOS( var ) v8::UniquePersistent<v8::Object>( v8::Isolate::GetCurrent(), var )
#define nkSetV8PObjectOS( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkResetV8PObjectOS( ivar, var ) ivar.Reset( v8::Isolate::GetCurrent(), var )
#define nkNewV8PObject( var ) v8::UniquePersistent<v8::Object>( isolate, var )
#define nkSetV8PObject( ivar, var ) ivar( isolate, var )
#define nkResetV8PObject( ivar, var ) ivar.Reset( isolate, var )
#define nkNewV8ObjectOS( var ) var
#define nkNewV8Object( var ) var

// Array
#define nkNewV8PArrayOS( var ) v8::UniquePersistent<v8::Array>( v8::Isolate::GetCurrent(), var )
#define nkSetV8PArrayOS( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkResetV8PArrayOS( ivar, var ) ivar.Reset( v8::Isolate::GetCurrent(), var )
#define nkNewV8PArray( var ) v8::UniquePersistent<v8::Array>( isolate, var )
#define nkSetV8PArray( ivar, var ) ivar( isolate, var )
#define nkResetV8PArray( ivar, var ) ivar.Reset( isolate, var )
#define nkNewV8ArrayOS( var ) var
#define nkNewV8Array( var ) var

// String
#define nkNewV8PStringOS( var ) v8::UniquePersistent<v8::String>( v8::Isolate::GetCurrent(), var )
#define nkSetV8PStringOS( ivar, var ) ivar( v8::UniquePersistent<v8::String>( v8::Isolate::GetCurrent(), var ) )
#define nkResetV8PStringOS( ivar, var ) ivar.Reset( v8::Isolate::GetCurrent(), var )
#define nkNewV8PString( var ) v8::UniquePersistent<v8::String>( isolate, var )
#define nkSetV8PString( ivar, var ) ivar( isolate, var )
#define nkResetV8PString( ivar, var ) ivar.Reset( isolate, var )
#define nkResetV8PStringUtf8( ivar, var ) ivar.Reset( isolate, v8::String::NewFromUtf8( isolate, var ) )
#define nkNewV8StringOS( var ) v8::String::NewFromUtf8( v8::Isolate::GetCurrent(), var )
#define nkSetV8StringOS( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkNewV8String( var ) v8::String::NewFromUtf8( isolate, var )
#define nkSetV8String( ivar, var ) ivar( isolate, var )
#define nkNewV8SStringOS( var ) v8::String::NewFromUtf8( v8::Isolate::GetCurrent(), var, v8::String::kInternalizedString )
#define nkNewV8SString( var ) v8::String::NewFromUtf8( isolate, var, v8::String::kInternalizedString )
#define nkV8StringValueType v8::String::Utf8Value

// Integer
#define nkNewV8PIntegerOS( var ) v8::UniquePersistent<v8::Integer>( v8::Isolate::GetCurrent(), var )
#define nkSetV8PIntegerOS( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkResetV8PIntegerOS( ivar, var ) ivar.Reset( v8::Isolate::GetCurrent(), v8::Integer::New( v8::Isolate::GetCurrent(), var ) )
#define nkNewV8PInteger( var ) v8::UniquePersistent<v8::Integer>( isolate, var )
#define nkSetV8PInteger( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkResetV8PInteger( ivar, var ) ivar.Reset( isolate, v8::Integer::New( isolate, var ) )
#define nkNewV8IntegerOS( var ) var
#define nkNewV8Integer( var ) var

// Boolean
#define nkNewV8PBooleanOS( var ) v8::UniquePersistent<v8::Boolean>( v8::Isolate::GetCurrent(), var )
#define nkSetV8PBooleanOS( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkResetV8PBooleanOS( ivar, var ) ivar.Reset( v8::Isolate::GetCurrent(), v8::Boolean::New( v8::Isolate::GetCurrent(), var ) )
#define nkNewV8PBoolean( var ) v8::UniquePersistent<v8::Boolean>( isolate, var )
#define nkSetV8PBoolean( ivar, var ) ivar( isolate, var )
#define nkResetV8PBoolean( ivar, var ) ivar.Reset( isolate, v8::Boolean::New( isolate, var ) )
#define nkNewV8BooleanOS( var ) var
#define nkNewV8Boolean( var ) var

// How we provide default values to the constructor for our chosen types is forcefully changed, so these defines must be kept, and left blank
#define nkDriverConstructorProtoBlock const nkPersistentMemberType<v8::String>& host, \
                                 const nkPersistentMemberType<v8::String>& port, \
                                 const nkPersistentMemberType<v8::String>& db, \
                                 const nkPersistentMemberType<v8::String>& user, \
                                 const nkPersistentMemberType<v8::String>& password, \
                                 const nkPersistentMemberType<v8::Object>& model, \
                                 const nkPersistentMemberType<v8::Boolean>& modeled, \
                                 const nkPersistentMemberType<v8::Integer>& type, \
                                 const nkPersistentMemberType<v8::Integer>& prepared, \
                                 const nkPersistentMemberType<v8::Array>& phmap, \
                                 const nkPersistentMemberType<v8::Boolean>& mapped, \
                                 const nkPersistentMemberType<v8::String>& query \

#define nkDriverConstructorDefinitionBlockA   const nkPersistentMemberType<String>& host, \
                                              const nkPersistentMemberType<String>& port, \
                                              const nkPersistentMemberType<String>& db, \
                                              const nkPersistentMemberType<String>& user, \
                                              const nkPersistentMemberType<String>& password, \
                                              const nkPersistentMemberType<Object>& model, \
                                              const nkPersistentMemberType<Boolean>& modeled, \
                                              const nkPersistentMemberType<Integer>& type, \
                                              const nkPersistentMemberType<Integer>& prepared, \
                                              const nkPersistentMemberType<Array>& phmap, \
                                              const nkPersistentMemberType<Boolean>& mapped, \
                                              const nkPersistentMemberType<String>& query

#define nkDriverConstructorDefinitionBlockB   nkSetV8PStringOS( host_, host ), \
                                              nkSetV8PStringOS( port_, port ), \
                                              nkSetV8PStringOS( db_, db ), \
                                              nkSetV8PStringOS( user_, user ), \
                                              nkSetV8PStringOS( password_, password ), \
                                              nkSetV8PObjectOS( model_, model ), \
                                              nkSetV8PBooleanOS( modeled_, modeled ), \
                                              nkSetV8PIntegerOS( type_, type ), \
                                              nkSetV8PIntegerOS( prepared_, prepared ), \
                                              nkSetV8PArrayOS( phmap_, phmap ), \
                                              nkSetV8PBooleanOS( mapped_, mapped ), \
                                              nkSetV8PStringOS( query_, query )

// The Exception return type was changed
#define nkException( str ) isolate->ThrowException( v8::Exception::TypeError( v8::String::NewFromUtf8( isolate, str ) ) )

// The v8::Arguments type was changed to v8::FunctionCallbackInfo<Value>
#define nkMethodArgumentType v8::FunctionCallbackInfo<v8::Value>

#define nkV8ObjectType v8::Handle<v8::Object>
#define nkSetPO( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkV8ArrayType v8::Handle<v8::Array>
#define nkSetPA( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkV8IntegerType v8::Handle<v8::Integer>
#define nkSetPI( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )
#define nkV8BooleanType v8::Handle<v8::Boolean>
#define nkSetPB( ivar, var ) ivar( v8::Isolate::GetCurrent(), var )

#define nkUnwrapThis args.Holder()

#define nkMethodReturnType void
#define nkReturn( var ) args.GetReturnValue().Set( var ); \
                        return;
// Node 0.10.x and below
#else

// We will use persistent members in v <= 0.10.x of Node.js
#define nkPersistentMemberType v8::Persistent

// Throughout this else statement, you will see matching comments (to the if statements commentes) which
// explain what the macro corrects in newer versions of the API - so that tracing and debugging for future
// modifications can be made a little more simple for any new contributors.
//
//
// Introduced in the Node API for v0.12.x is the v8::Isolate, used in many functions and with many types
// we need the below defines defined in order to avoid error - even if we are building for Node API v0.10.x
//
//
// OS = Out of scope - we'll want to avoid doing this in the future as v8::Isolate::GetCurrent() will be phased out.
#define nkIsolation( info )
#define nkIsolationOS
#define nkIsolateOS
#define nkIsolate
#define nkPreIsolatedOS
#define nkPreIsolated
#define nkPostIsolatedOS
#define nkPostIsolated

// HandleScope is still around (we only ever call it after isolate is defined, so we're not bothering to make multiple macros)
#define nkScope v8::HandleScope scope;

// The way to create function templates changed, so we macro'd it
#define nkFunctionTemplate( var ) v8::FunctionTemplate::New( var )

// This includes the manner in which we define the prototypes of our function templates' methods
#define nkSetPrototypeMethod( tpl, sym, var ) tpl->PrototypeTemplate()->Set( v8::String::NewSymbol( sym ), v8::FunctionTemplate::New( var )->GetFunction() );

// As well as how we set the constructor thanks to Persistent<T> type changes
#define nkSetConstructor( tpl ) constructor = v8::Persistent<Function>::New( nkPreIsolated tpl->GetFunction() );

// And how we instantiate a new instance of our object using said constructor has changed
#define nkNewConstructor
#define nkNewFromConstructor constructor->NewInstance( argc, argv )

// How we export the module/add-on in C++ has also changed
#define nkSetExports( var, constructor, tpl ) exports->Set( v8::String::NewSymbol( var ), constructor );

// P = Persistent, OS = Out of Scope, S = symbol-type
//
// In the future, v8::Isolate::GetCurrent() will need to be replaced; the macro should be passed the local isolate

// Object
#define nkNewV8PObjectOS( var ) v8::Persistent<v8::Object>::New( var )
#define nkSetV8PObjectOS( ivar, var ) ivar( var )
#define nkResetV8PObjectOS( ivar, var ) ivar = v8::Persistent<v8::Object>::New( var )
#define nkNewV8PObject( var ) v8::Persistent<v8::Object>::New( var )
#define nkSetV8PObject( ivar, var ) ivar( var )
#define nkResetV8PObject( ivar, var ) ivar = v8::Persistent<v8::Object>::New( var )
#define nkNewV8ObjectOS( var ) var
#define nkNewV8Object( var ) var

// Array
#define nkNewV8PArrayOS( var ) v8::Persistent<v8::Array>::New( var )
#define nkSetV8PArrayOS( ivar, var ) ivar( var )
#define nkResetV8PArrayOS( ivar, var ) ivar = v8::Persistent<v8::Array>::New( var )
#define nkNewV8PArray( var ) v8::Persistent<v8::Array>::New( var )
#define nkSetV8PArray( ivar, var ) ivar( var )
#define nkResetV8PArray( ivar, var ) ivar = v8::Persistent<v8::Array>::New( var )
#define nkNewV8ArrayOS( var ) var
#define nkNewV8Array( var ) var

// String
#define nkNewV8PStringOS( var ) v8::Persistent<v8::String>::New( v8::String::New( var ) )
#define nkSetV8PStringOS( ivar, var ) v8::Persistent<v8::String>::New( v8::String::New( var ) )
#define nkResetV8PStringOS( ivar, var ) ivar = v8::Persistent<v8::String>::New( v8::String::New( var ) )
#define nkNewV8PString( var ) v8::Persistent<v8::String>::New( var )
#define nkSetV8PString( ivar, var ) v8::Persistent<v8::String>::New( var )
#define nkResetV8PString( ivar, var ) ivar = v8::Persistent<v8::String>::New( var )
#define nkResetV8PStringUtf8( ivar, var ) ivar = v8::Persistent<v8::String>::New( v8::String::New( var ) )
#define nkNewV8StringOS( var ) v8::String::New( var )
#define nkSetV8StringOS( ivar, var ) ivar = v8::String::New( var )
#define nkNewV8String( var ) v8::String::New( var )
#define nkSetV8String( ivar, var ) ivar = v8::String::New( var )
#define nkNewV8SStringOS( var ) v8::String::NewSymbol( var )
#define nkNewV8SString( var ) v8::String::NewSymbol( var )
#define nkV8StringValueType v8::String::AsciiValue

// Integer
#define nkNewV8PIntegerOS( var ) v8::Persistent<v8::Integer>::New( var )
#define nkSetV8PIntegerOS( ivar, var ) ivar = v8::Persistent<v8::Integer>::New( var )
#define nkResetV8PIntegerOS( ivar, var ) ivar = v8::Persistent<v8::Integer>::New( var )
#define nkNewV8PInteger( var ) v8::Persistent<v8::Integer>::New( var )
#define nkSetV8PInteger( ivar, var ) ivar = v8::Persistent<v8::Integer>::New( var )
#define nkResetV8PInteger( ivar, var ) ivar = v8::Persistent<v8::Integer>::New( v8::Integer::New( var ) )
#define nkNewV8IntegerOS( var ) var
#define nkNewV8Integer( var ) var

// Boolean
#define nkNewV8PBooleanOS( var ) v8::Persistent<v8::Boolean>::New( var )
#define nkSetV8PBooleanOS( ivar, var ) ivar = v8::Persistent<v8::Boolean>::New( var )
#define nkResetV8PBooleanOS( ivar, var ) ivar = v8::Persistent<v8::Boolean>::New( var )
#define nkNewV8PBoolean( var ) v8::Persistent<v8::Boolean>::New( var )
#define nkSetV8PBoolean( ivar, var ) ivar = v8::Persistent<v8::Boolean>::New( var )
#define nkResetV8PBoolean( ivar, var ) if( var == true ){ ivar = v8::Persistent<v8::Boolean>::New( v8::True() ); }else{ ivar = v8::Persistent<v8::Boolean>::New( v8::False() ); }
#define nkNewV8BooleanOS( var ) var
#define nkNewV8Boolean( var ) var

// How we provide default values to the constructor for our chosen types is forcefully changed
#define nkDriverConstructorProtoBlock   nkPersistentMemberType<v8::String> host = nkNewV8PStringOS( "localhost" ), \
                                        nkPersistentMemberType<v8::String> port = nkNewV8PStringOS( "3306" ), \
                                        nkPersistentMemberType<v8::String> db = nkNewV8PStringOS( "test" ), \
                                        nkPersistentMemberType<v8::String> user = nkNewV8PStringOS( "test" ), \
                                        nkPersistentMemberType<v8::String> password = nkNewV8PStringOS( "password" ), \
                                        nkPersistentMemberType<v8::Object> model = nkNewV8PObjectOS( v8::Object::New() ), \
                                        nkPersistentMemberType<v8::Boolean> modeled = nkNewV8PBooleanOS( v8::False() ), \
                                        nkPersistentMemberType<v8::Integer> type = nkNewV8PIntegerOS( v8::Integer::New( 0 ) ), \
                                        nkPersistentMemberType<v8::Integer> prepared = nkNewV8PIntegerOS( v8::Integer::New( 0 ) ), \
                                        nkPersistentMemberType<v8::Array> phmap = nkNewV8PArrayOS( v8::Array::New() ), \
                                        nkPersistentMemberType<v8::Boolean> mapped = nkNewV8PBooleanOS( v8::False() ), \
                                        nkPersistentMemberType<v8::String> query =  nkNewV8PStringOS( "" )

#define nkDriverConstructorDefinitionBlockA   nkPersistentMemberType<String> host, \
                                              nkPersistentMemberType<String> port, \
                                              nkPersistentMemberType<String> db, \
                                              nkPersistentMemberType<String> user, \
                                              nkPersistentMemberType<String> password, \
                                              nkPersistentMemberType<Object> model, \
                                              nkPersistentMemberType<Boolean> modeled, \
                                              nkPersistentMemberType<Integer> type, \
                                              nkPersistentMemberType<Integer> prepared, \
                                              nkPersistentMemberType<Array> phmap, \
                                              nkPersistentMemberType<Boolean> mapped, \
                                              nkPersistentMemberType<String> query

#define nkDriverConstructorDefinitionBlockB   host_( host ), \
                                              port_( port ), \
                                              db_( db ), \
                                              user_( user ), \
                                              password_( password ), \
                                              model_( model ), \
                                              modeled_( modeled ), \
                                              type_( type ), \
                                              prepared_( prepared ), \
                                              phmap_( phmap ), \
                                              mapped_( mapped ), \
                                              query_( query )

#define nkException( str ) return v8::ThrowException( v8::Exception::TypeError( v8::String::New( str ) ) )

#define nkMethodArgumentType v8::Arguments

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

#define nkUnwrapThis args.This()

#define nkMethodReturnType v8::Handle<v8::Value>
#define nkReturn( var ) return scope.Close( var );
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
        explicit Driver( nkDriverConstructorProtoBlock );
        ~Driver();

        static v8::Persistent<v8::Function> constructor;
        static nkMethodReturnType New( const nkMethodArgumentType& args );
        static nkMethodReturnType Query( const nkMethodArgumentType& args );
        static nkMethodReturnType Select( const nkMethodArgumentType& args );
        static nkMethodReturnType Insert( const nkMethodArgumentType& args );
        static nkMethodReturnType Values( const nkMethodArgumentType& args );
        static nkMethodReturnType Update( const nkMethodArgumentType& args );
        static nkMethodReturnType Delete( const nkMethodArgumentType& args );
        static nkMethodReturnType Where( const nkMethodArgumentType& args );
        static nkMethodReturnType Join( const nkMethodArgumentType& args );
        static nkMethodReturnType On( const nkMethodArgumentType& args );
        static nkMethodReturnType Limit( const nkMethodArgumentType& args );
        static nkMethodReturnType Order( const nkMethodArgumentType& args );
        static nkMethodReturnType Execute( const nkMethodArgumentType& args );
        static nkMethodReturnType ExecuteQuery( const nkMethodArgumentType& args );
        static nkMethodReturnType GetConnection( const nkMethodArgumentType& args );
        static nkMethodReturnType GetQuery( const nkMethodArgumentType& args );
        static nkMethodReturnType Reset( const nkMethodArgumentType& args );

    public:

        nkPersistentMemberType<v8::String> host_;
        nkPersistentMemberType<v8::String> port_;
        nkPersistentMemberType<v8::String> db_;
        nkPersistentMemberType<v8::String> user_;
        nkPersistentMemberType<v8::String> password_;
        nkPersistentMemberType<v8::Object> model_;
        nkPersistentMemberType<v8::Boolean> modeled_;
        nkPersistentMemberType<v8::Integer> type_;
        nkPersistentMemberType<v8::Integer> prepared_;
        nkPersistentMemberType<v8::Array> phmap_;
        nkPersistentMemberType<v8::Boolean> mapped_;
        nkPersistentMemberType<v8::String> query_;
};

#endif
