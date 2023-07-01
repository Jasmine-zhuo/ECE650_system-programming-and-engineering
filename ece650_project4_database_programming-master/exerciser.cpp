#include "exerciser.h"

void exercise(connection *C)
{
  //query1(C,1,35,40,0,0,1,0,0,1,0,0,1,0,0.0,10.0,0,0.0,10.0);
  //query2(C,"Red");
  //query2(C,"DarkBlue");
  //query3(C,"WakeForest");
  //query3(C,"Duke");
  //query4(C,"VA","Orange");
  /*query1(C,
  //int use_mpg, int min_mpg, int max_mpg,
  1, 15, 20,
    //int use_ppg, int min_ppg, int max_ppg,
    0, 0, 0,
   //int use_rpg, int min_rpg, int max_rpg,
   0, 0, 0,
  //int use_apg, int min_apg, int max_apg,
  0, 0, 0, 
    //int use_spg, double min_spg, double max_spg, 
   0, 0, 0,
   //int use_bpg, double min_bpg, double max_bpg 
   0, 0, 0
   );*/
  //query5(C,10);
  //query5(C,5);
  query1(C,
  //int use_mpg, int min_mpg, int max_mpg,
  0, 0, 0,
    //int use_ppg, int min_ppg, int max_ppg,
    1, 15, 25,
   //int use_rpg, int min_rpg, int max_rpg,
   0, 0, 0,
  //int use_apg, int min_apg, int max_apg,
  0, 0, 0, 
    //int use_spg, double min_spg, double max_spg, 
   0, 0, 0,
   //int use_bpg, double min_bpg, double max_bpg 
   0, 0, 0
   );
}
