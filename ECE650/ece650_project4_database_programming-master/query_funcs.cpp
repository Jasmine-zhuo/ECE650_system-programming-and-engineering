#include "query_funcs.h"
void drop_table(connection * C, string tableName){
  work W(*C);
  string sql = "DROP TABLE " + tableName;
  W.exec(sql);
  W.commit();

}
void create_table(connection *C, string tableName){
  work W(*C);
  string sql;
  if(tableName == "PLAYER"){
    sql = "CREATE TABLE " + tableName +
      " (PLAYER_ID SERIAL PRIMARY KEY, "  \
      " TEAM_ID INT,"                           \
      " UNIFORM_NUM INT,"                       \
      " FIRST_NAME VARCHAR(40),"                \
      " LAST_NAME VARCHAR(40),"                 \
      " MPG INT,"                               \
      " PPG INT,"                               \
      " RPG INT,"                               \
      " APG INT,"                               \
      " SPG DECIMAL(5,1),"                      \
      " BPG DECIMAL(5,1)"                       \
      ");";
  }
  else if (tableName == "TEAM"){
    sql = "CREATE TABLE " + tableName +
      " (TEAM_ID SERIAL PRIMARY KEY, "\
      " NAME VARCHAR(40),"\
      " STATE_ID INT,"\
      " COLOR_ID INT,"\
      " WINS INT,"\
      " LOSSES INT"\
      ");";
  }
  else if (tableName == "STATE"){
    sql = "CREATE TABLE " + tableName +
      " (STATE_ID SERIAL PRIMARY KEY, "\
      " NAME VARCHAR(40));";
  }
  else if(tableName == "COLOR"){
    sql = "CREATE TABLE "+ tableName +
      " (COLOR_ID SERIAL PRIMARY KEY, "\
      " NAME VARCHAR(40));";
  }
  else{
    cout << "Invalid table!" << endl;
  }
  //  std::cout << sql << std::endl;
  W.exec(sql);
  W.commit();
}

void loadPlayer(connection * C){
  fstream f("player.txt",fstream::in);
  string line;
  while(std::getline(f,line)){
    stringstream ss(line);

    string item;
    int player_id,team_id,jersey_num;
    string first_name,last_name;         
    int mpg,ppg,rpg,apg; 
    double spg,bpg;                              
    for(int i=0; ss >> item; i++){
      switch(i){

      case 0:
        player_id = atoi(item.c_str());break;
      case 1:
        team_id = atoi(item.c_str());break;
      case 2:
        jersey_num = atoi(item.c_str());break;
      case 3:
        first_name = escape(item);break;
      case 4:
        last_name = escape(item);break;
      case 5:
        mpg = atoi(item.c_str());break;
      case 6:
        ppg = atoi(item.c_str());break;
      case 7:
        rpg = atoi(item.c_str());break;
      case 8:
        apg = atoi(item.c_str());break;
      case 9:
        spg = atof(item.c_str());break;
      case 10:
        bpg = atof(item.c_str());break;
          
      default:break;
      }
    }
    add_player(C,team_id,jersey_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg);
    
  }
  f.close();
} 
void loadTeam(connection * C){
    fstream f("team.txt",fstream::in);
  string line;
  while(std::getline(f,line)){
    stringstream ss(line);

    string item;
    int team_id,state_id,color_id,wins,losses;                         

    string team_name;
    for(int i=0; ss >> item; i++){
      switch(i){
        
      case 0:
        team_id = atoi(item.c_str());break;
      case 1:
        team_name = item;break;
      case 2:
        state_id = atoi(item.c_str());break;
      case 3:
        color_id = atoi(item.c_str());break;
      case 4:
        wins = atoi(item.c_str());break;
      case 5:
        losses = atoi(item.c_str());break;
      default:break;
      }
    }
    add_team(C, team_name, state_id, color_id, wins, losses);
  }
  f.close();
}   
void loadState(connection * C){
  fstream f("state.txt",fstream::in);
  string line;
  while(std::getline(f,line)){
    stringstream ss(line);

    string item;
    string state_name;
    int state_id;
    for(int i=0; ss >> item; i++){
      switch(i){

      case 0:
        state_id = atoi(item.c_str());break;
      case 1:
        state_name = item;break;
      default:break;
      }
    }
    add_state(C, state_name);
  }
  f.close();

} 
void loadColor(connection * C){
  fstream f("color.txt",fstream::in);
  string line;
  while(std::getline(f,line)){
    stringstream ss(line);

    string item;
    string color_id,color_name;                         
    for(int i=0; ss >> item; i++){
      switch(i){

      case 0:
        color_id = atoi(item.c_str());break;
      case 1:
        color_name = item;break;
      default:break;
      }
    }
    add_color(C, color_name);
  }
  f.close();
}
string escape(string &s){
  char ch = '\'';
  size_t pos = s.find(ch);
  if(pos != string::npos){
    s.insert(pos,1,'\'');
  }
  return s;
 }

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
  string sql;
  sql = "INSERT INTO PLAYER (TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,"\
    "MPG,PPG,RPG,APG,SPG,BPG)"\
    "VALUES(" + to_string(team_id) + ","                                \
    + to_string(jersey_num) + ",'" + first_name + "','" + last_name + "'," + to_string(mpg) \
    + "," + to_string(ppg) + "," + to_string(rpg) + "," + to_string(apg) + "," + to_string(spg) + "," + to_string(bpg) + ");";

  // cout << sql << endl;
  work W(*C);
  W.exec(sql);
  W.commit();
}


void add_team(connection *C, string team_name, int state_id, int color_id, int wins, int losses)
{
  string sql;
  sql = "INSERT INTO TEAM (NAME,STATE_ID,COLOR_ID,WINS,LOSSES)"\
    "VALUES('"  + team_name +"'," + to_string(state_id) + "," + to_string(color_id) \
    + "," + to_string(wins) + "," + to_string(losses) + ");";
  // cout << sql << endl;
  work W(*C);
  W.exec(sql);
  W.commit();
}


void add_state(connection *C, string state_name)
{
  string sql;
  sql="INSERT INTO STATE (NAME)"\
    "VALUES('"  + state_name + "');";
  //cout << sql << endl;
  work W(*C);
  W.exec(sql);
  W.commit();
}

void add_color(connection *C,string color_name)
{
  string sql;
  sql="INSERT INTO COLOR (NAME)"\
    "VALUES('" + color_name + "');";
  // cout << sql << endl;
  work W(*C);
  W.exec(sql);
  W.commit();
}
 


void addFilter(string & sql, int & numEnabled, const string & column, const string & min, const string & max)
{                                                          
  if(numEnabled++ > 0){                                                      
    sql += " AND ";                                                          
  }                                                                        
  sql += column + " BETWEEN " + min +" AND " + max;
}


/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  
  //e.g. SELECT * FROM PLAYER WHERE MPG BETWEEN min_mpg AND max_mpg;
  string sql;
  nontransaction  N(*C);

  sql = "SELECT * FROM PLAYER ";
  int numEnabled = 0;
  if(use_mpg + use_ppg + use_rpg + use_apg + use_spg + use_bpg > 0 ){
    sql += "WHERE ";
  }
  if(use_mpg == 1){
    addFilter(sql,numEnabled,"MPG",to_string(min_mpg),to_string(max_mpg));
  }
  if(use_ppg == 1){
    addFilter(sql,numEnabled,"PPG",to_string(min_ppg),to_string(max_ppg));
  }
  if(use_rpg == 1){                                                        
    addFilter(sql,numEnabled,"RPG",to_string(min_rpg),to_string(max_rpg));
  }
  if(use_apg == 1){                                                        
    addFilter(sql,numEnabled,"APG",to_string(min_apg),to_string(max_apg)); 
  }
  if(use_spg == 1){                                                        
    addFilter(sql,numEnabled,"SPG",to_string(min_spg),to_string(max_spg)); 
  }
  if(use_bpg == 1){                                                        
    addFilter(sql,numEnabled,"BPG",to_string(min_bpg),to_string(max_bpg)); 
  }                                                                        

  //  cout << "sql is : \n" << sql << endl;
  result R(N.exec(sql));
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
  
  for(auto c = R.begin(); c != R.end(); ++c){
    cout << c[0].as<string>() << ' ';
    cout << c[1].as<string>() << ' ';
    cout << c[2].as<string>() << ' ';
    cout << c[3].as<string>() << ' ';
    cout << c[4].as<string>() << ' ';
    cout << c[5].as<int>() << ' ';
    cout << c[6].as<int>() << ' ';
    cout << c[7].as<int>() << ' ';
    cout << c[8].as<int>() << ' ';
    cout << c[9].as<string>() << ' ';
    cout << c[10].as<string>() << endl;
  }
 
}

void query2(connection *C, string team_color)
{
  string sql;
  nontransaction N(*C);
  
  sql = "SELECT T.NAME FROM TEAM AS T,COLOR AS C WHERE T.COLOR_ID = C.COLOR_ID AND C.NAME =  '" + \
    team_color + "';";
  result R(N.exec(sql));
  cout << "NAME" << endl;
  for(auto c= R.begin(); c!= R.end(); ++c){
    cout << c[0].as<string>() << endl;
  }
  
}


void query3(connection *C, string team_name)

{
  string sql;
  nontransaction N(*C);
  sql = "SELECT P.FIRST_NAME,P.LAST_NAME FROM PLAYER AS P, TEAM AS T WHERE P.TEAM_ID = T.TEAM_ID AND T.NAME = '"+\
    team_name + "' ORDER BY P.PPG DESC;";
  result R(N.exec(sql));
  cout << "FIRST_NAME LAST_NAME" << endl;
  for(result::const_iterator c = R.begin(); c != R.end();++c){
    cout << c[0].as<string>() << ' ';
    cout << c[1].as<string>() << endl;
  }
}


void query4(connection *C, string team_state, string team_color)
{
  string sql;
  nontransaction N(*C);
  sql = "SELECT P.UNIFORM_NUM,P.FIRST_NAME,P.LAST_NAME FROM PLAYER AS P,TEAM AS T,STATE AS S,COLOR AS C WHERE P.TEAM_ID=T.TEAM_ID AND T.STATE_ID=S.STATE_ID AND T.COLOR_ID = C.COLOR_ID AND S.NAME = '"+\
    team_state + "' AND C.NAME='"+\
    team_color + "';";
  result R(N.exec(sql));
  cout << "UNIFORM_NUM FIRST_NAME LAST_NAME" << endl;
  for(result::const_iterator c = R.begin(); c!= R.end(); ++c){
    cout << c[0].as<int>() << ' ';
    cout << c[1].as<string>() << ' ';
    cout << c[2].as<string>() << endl;
  }
}


void query5(connection *C, int num_wins)
{
  string sql;
  nontransaction N(*C);
  sql = "SELECT P.FIRST_NAME,P.LAST_NAME,T.NAME,T.WINS FROM PLAYER AS P,TEAM AS T WHERE P.TEAM_ID=T.TEAM_ID AND T.WINS > "+ to_string(num_wins) + ";";
    result R(N.exec(sql));
  cout << "FIRST_NAME LAST_NAME NAME WINS" <<endl;
  for(result::const_iterator c=R.begin();c!=R.end();++c){
    cout << c[0].as<string>() << ' ';
    cout << c[1].as<string>() << ' ';
    cout << c[2].as<string>() << ' ';
    cout << c[3].as<int>() << endl;
  }
}
