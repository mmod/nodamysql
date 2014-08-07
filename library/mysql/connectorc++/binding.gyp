{
	'variables':
	{	# Any variable names we wish to create and define
		'cppconn_inc_dir': './',
		'cppconn_inc_dir_for_driver': 'cppconn',
		'cppconn_inc_dir_for_nativeapi': 'driver/nativeapi'
	},
	
	'includes':
	[	# Any GYP include - or .gypi - files
		'../../../includables/common-mysql.gypi'	
	],
	
	'target_defaults':
	{	# Any default configuration settings
	
		'default_configuration': 'Release',
		'configurations':
		{
			'Debug': 
			{
				'defines': [ 'DEBUG', '_DEBUG' ]
			},
			'Release':
			{
				'defines': [ 'NDEBUG' ]
			}
		},
		'conditions':
		[
			[
				'OS=="win"',
				{ 
					'configurations':
					{
						'Debug':
						{
							'defines': [ 'WIN32' ],
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
										'-Dmysqlcppconn_EXPORTS'	# This must be set in order to build a shared variant
									]
								}
							}
						},
						'Release':
						{
							'defines': [ 'WIN32' ],
							'msvs_settings':
							{
								'VCCLCompilerTool':
								{
									'RuntimeLibrary': 2,		# shared release
									'ExceptionHandling': 1,
									'AdditionalOptions':
									[
										'/GR',
										'/MD',	# RuntimeLibrary setting above seems to not do anything as the warning makes clear to you during build.
										'/EHsc',
										'-Dmysqlcppconn_EXPORTS'	# This must be set in order to build a shared variant
									]
								}
							}
						}
					}
				}
			],
			[
				'OS!="win"',
				{
					'configurations':
					{
						'Debug':
						{
							'cflags': [ '-std=c++11', '-fexceptions', '-frtti', '-Wno-deprecated', '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-maybe-uninitialized', '-Wno-sign-compare', '-Wno-reorder', '-Wno-extra', '-Wno-switch', '-Dmysqlcppconn_EXPORTS' ],
							'cflags_cc':
							[
								'-std=c++11',
								'-fexceptions',					# Enable exception handler
								'-frtti',						# Allow dynamic casting
								'-Wno-deprecated',				# Remove deprecated warnings
								'-Wno-unused-variable',			# Remove unused-variable warnings
								'-Wno-unused-but-set-variable',	# Remove unused-but-set-variable warnings
								'-Wno-maybe-uninitialized',		# Remove may be used uninitialized warnings (Many false positives in GCC 4.7)
								'-Wno-sign-compare',			# Remove sign compare warnings
								'-Wno-reorder',					# Remove initialize after warnings
								'-Wno-extra',					# Remove explicit initialization warnings
								'-Wno-switch',					# Remove switch warnings
								'-Dmysqlcppconn_EXPORTS'		# This must be set in order to build a shared variant
							]
						},
						'Release':
						{
							'cflags': [ '-std=c++11', '-fexceptions', '-frtti', '-Wno-deprecated', '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-maybe-uninitialized', '-Wno-sign-compare', '-Wno-reorder', '-Wno-extra', '-Wno-switch', '-Dmysqlcppconn_EXPORTS' ],
							'cflags_cc': [ '-std=c++11', '-fexceptions', '-frtti', '-Wno-deprecated', '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-maybe-uninitialized', '-Wno-sign-compare', '-Wno-reorder', '-Wno-extra', '-Wno-switch', '-Dmysqlcppconn_EXPORTS' ]
						}
					}
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
					'mysql_dir%': '<!(IF DEFINED MYSQL_DIR (echo %MYSQL_DIR%) ELSE (echo C:/mysql/server/<(mysql_version)))'
				},
				'targets':
				[
					{
						'target_name': 'build_configuration',
						'type': 'none',
						'copies':
						[
							{
								'files': [ '../../../includables/buildage/win/build_config.h', '../../../includables/buildage/win/config.h' ],
								'destination': './cppconn/'
							}
						]
					},
					{
						'target_name': 'binding_configuration',
						'type': 'none',
						'copies':
						[
							{
								'files': [ '../../../includables/buildage/win/binding_config.h' ],
								'destination': './driver/nativeapi/'
							}
						]
					},
					{
						'target_name': 'dependency_configuration',
						'type': 'none',
						'copies':
						[
							{
								'files': [ '<(mysql_dir)/lib/libmysql.dll', '<(mysql_dir)/lib/libmysql.lib' ],
								'destination': '../../../../../'	# This puts a copy of libmysql.lib and libmysql.dll files into the root of the actual project (where node looks for dependencies) depending on nk-mysql.
							}
						]
					},
					{
						'target_name': 'mysqlcppconn',
						'type': 'shared_library',
						'dependencies': [ 'build_configuration', 'binding_configuration', 'dependency_configuration' ],
						'sources': 
						[ 
							'driver/mysql_art_resultset.cpp',
							'driver/mysql_art_rset_metadata.cpp',
							'driver/mysql_connection.cpp', 
							'driver/mysql_debug.cpp',
							'driver/mysql_driver.cpp',
							'driver/mysql_metadata.cpp',
							'driver/mysql_parameter_metadata.cpp',
							'driver/mysql_prepared_statement.cpp',
							'driver/mysql_ps_resultset.cpp',
							'driver/mysql_ps_resultset_metadata.cpp',
							'driver/mysql_resultbind.cpp',
							'driver/mysql_resultset.cpp',
							'driver/mysql_resultset_metadata.cpp',
							'driver/mysql_statement.cpp',
							'driver/mysql_util.cpp',
							'driver/mysql_warning.cpp',
							'driver/mysql_uri.cpp',
							'driver/nativeapi/mysql_client_api.cpp',
							'driver/nativeapi/library_loader.cpp',
							'driver/nativeapi/mysql_native_driver_wrapper.cpp',
							'driver/nativeapi/mysql_native_connection_wrapper.cpp',
							'driver/nativeapi/mysql_native_statement_wrapper.cpp',
							'driver/nativeapi/mysql_native_resultset_wrapper.cpp'
						],
						'include_dirs': [ '<(cppconn_inc_dir)/', '<(cppconn_inc_dir_for_driver)/', '<(cppconn_inc_dir_for_nativeapi)/', '<(mysql_dir)/include/', '<(boost_dir)/' ],
						'libraries': [ '<(mysql_dir)/lib/libmysql', 'kernel32', 'user32', 'gdi32', 'winspool', 'shell32', 'ole32', 'oleaut32', 'uuid', 'comdlg32', 'advapi32', 'ws2_32' ]
					}
				]
      		}
		],
		[
			'OS!="win"', 
			{
				'variables':
				{	# On Linux, make sure you have the boost headers in /usr/include or /usr/local/include
					'mysql_inc_dir%': '<!(command -v mysql_config >/dev/null && echo 2>&1 `mysql_config --variable=pkgincludedir` || echo >&2 "/opt/mysql/server-5.6/include")',
					'mysql_lib_dir%': '<!(command -v mysql_config >/dev/null && echo 2>&1 `mysql_config --variable=pkglibdir` || echo >&2 "/opt/mysql/server-5.6/lib")'
				},
				'targets':
				[
					{
						'target_name': 'build_configuration',
						'type': 'none',
						'copies':
						[
							{
								'files': [ '../../../includables/buildage/lin/build_config.h', '../../../includables/buildage/lin/config.h' ],
								'destination': 'cppconn/'
							}
						]
					},
					{
						'target_name': 'binding_configuration',
						'type': 'none',
						'copies':
						[
							{
								'files': [ '../../../includables/buildage/lin/binding_config.h' ],
								'destination': 'driver/nativeapi/'
							}
						]
					},
					{
						'target_name': 'libmysqlcppconn',
						'type': 'shared_library',
						'dependencies': [ 'build_configuration', 'binding_configuration' ],
						'sources': 
						[ 
							'driver/mysql_art_resultset.cpp',
							'driver/mysql_art_rset_metadata.cpp',
							'driver/mysql_connection.cpp', 
							'driver/mysql_debug.cpp',
							'driver/mysql_driver.cpp',
							'driver/mysql_metadata.cpp',
							'driver/mysql_parameter_metadata.cpp',
							'driver/mysql_prepared_statement.cpp',
							'driver/mysql_ps_resultset.cpp',
							'driver/mysql_ps_resultset_metadata.cpp',
							'driver/mysql_resultbind.cpp',
							'driver/mysql_resultset.cpp',
							'driver/mysql_resultset_metadata.cpp',
							'driver/mysql_statement.cpp',
							'driver/mysql_util.cpp',
							'driver/mysql_warning.cpp',
							'driver/mysql_uri.cpp',
							'driver/nativeapi/mysql_client_api.cpp',
							'driver/nativeapi/library_loader.cpp',
							'driver/nativeapi/mysql_native_driver_wrapper.cpp',
							'driver/nativeapi/mysql_native_connection_wrapper.cpp',
							'driver/nativeapi/mysql_native_statement_wrapper.cpp',
							'driver/nativeapi/mysql_native_resultset_wrapper.cpp'
						],
						'include_dirs': [ '<(cppconn_inc_dir)/', '<(cppconn_inc_dir_for_driver)/', '<(cppconn_inc_dir_for_nativeapi)/', '<(mysql_inc_dir)/' ],
						'libraries': [ '<(mysql_lib_dir)/libmysqlclient.so' ]
		      	 	}
				]
			}
		]
	]
}