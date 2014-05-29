{
  "targets": [
    {
      "target_name": "nodamysql",
      "sources": [ "nodamysql.cpp", "driver.cpp" ],
      "cflags": [ "-std=c++11" ],
      "include_dirs": [ "/opt/mysql/server-5.6/include/" ],
      "libraries": [ "/usr/local/lib/libmysqlcppconn.so", "-L/opt/mysql/server-5.6/lib", "-L/usr/local/lib" ]
    }
  ]
}