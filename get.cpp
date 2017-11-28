#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>
#include <fastcgi++/http.hpp>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "connector.hpp"

void error_log(const char* msg)
{
  using namespace std;
  using namespace boost;
  static ofstream error;
  if(!error.is_open())
  {
      error.open("/tmp/errlog", ios_base::out | ios_base::app);
      error.imbue(locale(error.getloc(), new posix_time::time_facet()));
  }
  error << '[' << posix_time::second_clock::local_time() << "] " << msg << endl;
}

class PutComic : public Connector
{
  inline void sendError(const std::string& errorMsg)
  {
      out << "{ \"success\" : 0, \"message\" : \"" + errorMsg + "\" }" << std::endl;
  }
  bool response()
  {
      out << "Content-Type: application/json; charset=ISO-8859-1\r\n\r\n";
      std::map<std::string, std::string> parameters;
      for (Fastcgipp::Http::Environment<char>::Gets::const_iterator it = environment().gets.begin(); it != environment().gets.end(); ++it)
      {
          parameters[it->first] = it->second;
      }
      if (parameters.find("id") == parameters.end())
      {
          sendError("Missing id");
      }
      else
      {
          sql::Statement* stmt = con->createStatement();
          try
          {
              sql::ResultSet* res = stmt->executeQuery("SELECT name, publisher, UNIX_TIMESTAMP(date) as date, edition FROM comic WHERE id = " + parameters["id"]);
              if (!res->next())
              {
                  sendError("Could not found comic with id = " + parameters["id"]);
              }
              else
              {
                  std::string result = "{ \"success\" : 1, ";
                  result += "\"name\": \"" + res->getString("name") + "\",";
                  result += "\"publisher\": \"" + res->getString("publisher") + "\",";
                  result += "\"date\": " + res->getString("date") + ",";
                  result += "\"edition\": " + res->getString("edition");
                  result += "}";
                  delete res;
                  out << result << std::endl;
              }
          } catch (sql::SQLException& e)
          {
              sendError(e.what());
          }
          delete stmt;
      }
      return true;
  }
};

int main()
{
  try
  {
      Fastcgipp::Manager<PutComic> fcgi;
      fcgi.handler();
  }
  catch (std::exception& e)
  {
          error_log(e.what());
  }
  return 0;
}
