#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;
  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
  
  
  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  try{  
    drop_table(C,"PLAYER");
    drop_table(C,"TEAM");
    drop_table(C,"STATE");
    drop_table(C,"COLOR");
  }
  catch( const std::exception & e){
    //do nothing. This is because we are trying to drop tables that does not exist
  }
  create_table(C,"PLAYER");
  create_table(C,"TEAM");
  create_table(C,"STATE");
  create_table(C,"COLOR");

  loadPlayer(C);
  loadTeam(C);
  loadState(C);
  loadColor(C);
  
  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


