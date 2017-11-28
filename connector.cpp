#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class Connector : public Fastcgipp::Request<char>
{
public:
  Connector()
  {
      driver = get_driver_instance();
      con = driver->connect("tcp://127.0.0.1:3306", "black_fighter", "qweqwe");
      con->setSchema("book_shop");
  }
  virtual ~Connector()
  {
      delete con;
  }
protected:
  sql::Driver* driver;
  sql::Connection* con;
};
