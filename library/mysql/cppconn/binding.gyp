{
	'variables':
	{	# Any variable names we wish to create and define
	},
	
	'includes':
	[	# Any GYP include - or .gypi - files
		'../../../includables/common-mysql.gypi'	
	],
	
	'target_defaults':
	{	# Any default configuration settings
	
		'default_configuration': 'Release',
		'cflags':
		[
			'-std=c++11'
		],
		'configurations':
		{
			'Debug': 
			{
				'defines': [ 'DEBUG', '_DEBUG' ],
				'msvs_settings':
				{
					'VCCLCompilerTool':
					{
						'RuntimeLibrary': 3,		# shared debug
						'ExceptionHandling': 1,		# /EHsc
						'AdditionalOptions':
						[
							'/GR',
							'/MDd',
							'/EHsc', # Enable unwind semantics for Exception Handling.  This one actually does the trick - and no warning either.
						]
					}
				}
			},
			'Release':
			{
				'defines': [ 'NDEBUG' ],
				'msvs_settings':
				{
					'VCCLCompilerTool':
					{
						'RuntimeLibrary': 2,		# shared release
						'ExceptionHandling': 1,		# /EHsc
						'AdditionalOptions':
						[
							'/GR',
							'/MD',	# RuntimeLibrary setting above seems to not do anything as the warning makes clear to you during build.
							'/EHsc', # Enable unwind semantics for Exception Handling.  This one actually does the trick - and no warning either.
							'/Dmysqlcppconn_EXPORTS'	# This needs to be set.
						]
					}
				}
			}
		},
		'conditions':
		[
			[
				'OS == "win"',
				{
					'defines':
					[
						'WIN32'
					]
				}
			]
		]
	},
  	'targets': 
  	[	# Any 'for sure' targets
  	],
    'conditions': 
    [	# Conditional targets
		[ 
			'OS=="win"', 
			{
				'variables':
				{
					'boost_dir%': '<!(IF DEFINED BOOST_ROOT (echo %BOOST_ROOT%) ELSE (echo C:/boost/boost_<(boost_version)))',
					'mysql_dir%': '<!(IF DEFINED MYSQL_DIR (echo %MYSQL_DIR%) ELSE (echo C:/mysql/server/<(mysql_version)))',
					'cppconn_inc_dir': 'win',
					'cppconn_inc_dir_for_driver': 'win/cppconn',
					'cppconn_inc_dir_for_nativeapi': 'win/cppconn/nativeapi'
				},
				'targets':
				[
					{
						'target_name': 'mysqlcppconn',
						'type': 'shared_library',
						'sources': 
						[ 
							'win/driver/mysql_art_resultset.cpp',
							'win/driver/mysql_art_rset_metadata.cpp',
							'win/driver/mysql_connection.cpp', 
							'win/driver/mysql_debug.cpp',
							'win/driver/mysql_driver.cpp',
							'win/driver/mysql_metadata.cpp',
							'win/driver/mysql_parameter_metadata.cpp',
							'win/driver/mysql_prepared_statement.cpp',
							'win/driver/mysql_ps_resultset.cpp',
							'win/driver/mysql_ps_resultset_metadata.cpp',
							'win/driver/mysql_resultbind.cpp',
							'win/driver/mysql_resultset.cpp',
							'win/driver/mysql_resultset_metadata.cpp',
							'win/driver/mysql_statement.cpp',
							'win/driver/mysql_util.cpp',
							'win/driver/mysql_warning.cpp',
							'win/driver/mysql_uri.cpp',
							'win/driver/nativeapi/mysql_client_api.cpp',
							'win/driver/nativeapi/library_loader.cpp',
							'win/driver/nativeapi/mysql_native_driver_wrapper.cpp',
							'win/driver/nativeapi/mysql_native_connection_wrapper.cpp',
							'win/driver/nativeapi/mysql_native_statement_wrapper.cpp',
							'win/driver/nativeapi/mysql_native_resultset_wrapper.cpp'
						],
						'cflags': [ '-std=c++11' ],
						'include_dirs': [ '<(cppconn_inc_dir)/', '<(cppconn_inc_dir_for_driver)/', '<(cppconn_inc_dir_for_nativeapi)/', '<(mysql_dir)/include/', '<(boost_dir)/' ],
						'libraries': [ '<(mysql_dir)/lib/libmysql', 'kernel32', 'user32', 'gdi32', 'winspool', 'shell32', 'ole32', 'oleaut32', 'uuid', 'comdlg32', 'advapi32', 'ws2_32' ]
					}
				]
      		}
		],
		[
			'OS=="linux"', 
			{
				'variables':
				{
					'boost_dir%': '<!(if [ -z $BOOST_ROOT ]; then echo \"/opt/boost_<(boost_version)/\"; else echo $BOOST_ROOT; fi)',
					'mysql_dir%': '<!(if [ -z $MYSQL_DIR ]; then echo \"/opt/mysql/server/<(mysql_version)/\"; else echo $MYSQL_DIR; fi)'
					'cppconn_inc_dir': 'lin',
					'cppconn_inc_dir_for_driver': 'lin/cppconn',
					'cppconn_inc_dir_for_nativeapi': 'lin/cppconn/nativeapi'
				},
				'targets':
				[
					{
						'target_name': 'libmysqlcppconn',
						'type': 'shared_library',
						'sources': 
						[ 
							'lin/driver/mysql_art_resultset.cpp',
							'lin/driver/mysql_art_rset_metadata.cpp',
							'lin/driver/mysql_connection.cpp', 
							'lin/driver/mysql_debug.cpp',
							'lin/driver/mysql_driver.cpp',
							'lin/driver/mysql_metadata.cpp',
							'lin/driver/mysql_parameter_metadata.cpp',
							'lin/driver/mysql_prepared_statement.cpp',
							'lin/driver/mysql_ps_resultset.cpp',
							'lin/driver/mysql_ps_resultset_metadata.cpp',
							'lin/driver/mysql_resultbind.cpp',
							'lin/driver/mysql_resultset.cpp',
							'lin/driver/mysql_resultset_metadata.cpp',
							'lin/driver/mysql_statement.cpp',
							'lin/driver/mysql_util.cpp',
							'lin/driver/mysql_warning.cpp',
							'lin/driver/mysql_uri.cpp',
							'lin/driver/nativeapi/mysql_client_api.cpp',
							'lin/driver/nativeapi/library_loader.cpp',
							'lin/driver/nativeapi/mysql_native_driver_wrapper.cpp',
							'lin/driver/nativeapi/mysql_native_connection_wrapper.cpp',
							'lin/driver/nativeapi/mysql_native_statement_wrapper.cpp',
							'lin/driver/nativeapi/mysql_native_resultset_wrapper.cpp'
						],
						'cflags': 
						[ 
							'-std=c++11',
							'/Dmysqlcppconn_EXPORTS'	# This needs to be set. 
						],
						'include_dirs': [ '<(cppconn_inc_dir)/', '<(cppconn_inc_dir_for_driver)/', '<(cppconn_inc_dir_for_nativeapi)/', '<(mysql_dir)/include/', '<(boost_dir)/' ],
						'libraries': [ '<(mysql_dir)/lib/libmysql' ]
		      	 	}
				]
			}
		]
	]
}