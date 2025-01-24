//Semester End Project for Programmmibg Fundamentals Fall-23 Session
//Assigned by Dr. Muhammad imarn
//Due Date: December 25, 2023
//Team Members: Abeera Mehtab (232087), Mahnoor (232083), Mahnoor Ikram (232115)
// The source file calls the functions to preform the task used
#include"header.h"
int main(){
    Rule rules[MAX_RULES];
    int Rule_Count;
    //initailized outside header file funtion so that they can be used in comp_rules_and_traffic fumction
    // Calling Function to load rules in rules struct array
    Load_Rules("rules.txt", rules, Rule_Count);

    Packet packets[MAX_PACKETS];
    int Packet_Count;
    //initailized outside header file funtion so that they can be used in comp_rules_and_traffic fumction
    // Calling Function to load traffic in traffic struct array
    Load_Traffic("traffic.txt", packets, Packet_Count);

    string result = comp_rules_and_traffic(rules, Rule_Count, packets, Packet_Count);
    //the result being retured from the function is now being stored in a string

    //Generating Output file:
    make_result_file("result.dat",result);

    system("PAUSE");
    return 0;
} 
