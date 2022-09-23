/*1-West 
2-NorthWest
3-North
4-NorthEast
5-East
6-SouthEast
7-South
8-SouthWest*/

#include "mazeblaze2.h"
int active_left_count = 0 ; //active_count will store the number of times white patches were encountered 
int active_right_count = 0;

void app_main(void)
{
   
}

int get_node_type()
{
    if (lsa_reading[0] == x )
    {
        active_left_count ++ ;
    }
    if(lsa_reading[4] == 1000)
    {
        active_right_count ++ ;
    }
    if(lsa_reading[5] = 0 && lsa_reading[6] = 0 && lsa_reading[1] = 1 && lsa_reading[2] = 1 && lsa_reading[3] = 1)
}

void turn ()
{
    char turn ;
    if ( active_left_count != 0 && lsa_reading[6] == 1000)
    {
        turn = 'L' ;
    }
    else if (straight_possible)
    {
        turn = 'S' ;
    }
    else 
    {
        turn = 'R' ;
    }

    possible_path 
}