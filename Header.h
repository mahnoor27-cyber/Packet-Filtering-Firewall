//Semester End Project for Programmmibg Fundamentals Fall-23 Session
//Assigned by Dr. Muhammad Imran
//Due Date: December 25, 2023
//Team Members: Abeera Mehtab (232087), Mahnoor (232083), Mahnoor Ikram (232115)
// The header file contains all functioms and structs and their definations

#include<fstream> 
#include<string>
#include<iostream>
#define MAX_RULES 100
#define MAX_PACKETS 100
//setting limit to number of rules and packets this program can handle as to not overload the program

using namespace std; 
struct Rule { 
    //Defining a Struct where data from rule files such as number of rules, where that rules is applied to, 
    //the IP address or protocol it's applied to, and the decision to wether allow or deny it, is stored
    int Rule_Num;
    string App;
    string IP_Pro;
    string Decision;
};
struct Packet {
    // Defing a Struct where data from Traffic file where data such as the source IP and Destination IP of Packets
    // the protocol of that packet and the data that packet contains
    string srcIP;
    string dstIP;
    string protocol;
    string data;
};

// This function takes rule file name (and address if necessary) loads the data from the file line by line and 
// stores it in the struct array created in cpp file, after every line rule count is increaed, each rule being stored in sttruct arrays defined
void Load_Rules(const string& filename, Rule rules[], int& Rule_Count) {
    ifstream Rfile(filename);
    if (!Rfile) {
        cout << "Error: Unable to open file." << endl;
        // For Error handling Purposees.
    }
    Rule_Count = 0; // Initailized to zero to avoid storage of junk variables
    while (Rfile >> rules[Rule_Count].Rule_Num >> rules[Rule_Count].App >> rules[Rule_Count].IP_Pro >> rules[Rule_Count].Decision) {
        Rule_Count++;
    }
    //cout << rules[0].IP_Pro << endl;
    // Added that statement to check wether data was being picked froom rule file or Not
}

// This function takes traffic file name(and address if necessary) as parameter,reads the whole file,
// first seperates the packet by finding where packet starts and where it ends since all packets are in one string
// then each object is seperated through substring and using elimnators to seperate
void Load_Traffic(const string& filename, Packet packets[], int& Packet_Count) {
    ifstream Tfile(filename);
    if (!Tfile) {
        cout << "Error: Unable to open file." << endl;
        return;
    }// For Error handling Purposees.
    string Trf_Str; 
    Packet_Count = 0; 
    getline(Tfile, Trf_Str);//whole file is read and stored in this string
    size_t start = 0, end = 0; //for keeping track of start and end of packet
    //While loop is used to separte packets using [ and ], and repeats till end of string or max pckets limit is reached
    // Then seperates Objects or Elements of each packet and store in Packet array
    while ((end = Trf_Str.find("[", start)) != string::npos && Packet_Count < MAX_PACKETS) {
        size_t End_Packet = Trf_Str.find("]", end);
        // string :: npos is used to check if no matches are found
        if (End_Packet == string::npos) {
            cout << "Error: Invalid file format." << endl;
            return;
        }
        string Packet_String = Trf_Str.substr(end + 1, End_Packet - end - 1); //storing packet data in substr
        size_t Start_Obj = 0, End_Obj = 0; //for keeping track of start and end of objects in array
        
        //Now parsing the data of packet in each struct data members by seperating the packet data using substr and ;
        End_Obj = Packet_String.find(";", Start_Obj);
        string srcIP_wTag = Packet_String.substr(Start_Obj, End_Obj - Start_Obj); // stores whole SRC IP with SRC tag
        packets[Packet_Count].srcIP = srcIP_wTag.substr(srcIP_wTag.find(":") + 1); // Extract IP only and ignores SRC before :
        Start_Obj = End_Obj + 1;

        End_Obj = Packet_String.find(";", Start_Obj);
        string dstIP_wTag = Packet_String.substr(Start_Obj, End_Obj - Start_Obj); // stores whole DST IP with DST tag
        packets[Packet_Count].dstIP = dstIP_wTag.substr(dstIP_wTag.find(":") + 1); // Extract IP only and ignores DST before:
        Start_Obj = End_Obj + 1;

        End_Obj = Packet_String.find(";", Start_Obj);
        string protocol_wTag = Packet_String.substr(Start_Obj, End_Obj - Start_Obj); //stores Protocl type with protocol tag
        packets[Packet_Count].protocol = protocol_wTag.substr(protocol_wTag.find(":") + 1); // Extract protocol only and Ignores protocol tag before :
        Start_Obj = End_Obj + 1;

        packets[Packet_Count].data = Packet_String.substr(Start_Obj);

        start = End_Packet + 1;
        Packet_Count++;
    }
    if (Packet_Count == MAX_PACKETS) { // Error Handling for Max_Packet limit
        cout << "Warning: Maximum packet limit reached. Some packets may not have been loaded." << endl;
    }
    // cout << packets[8].data << endl;
    // Added that statement to check wether data was being picked froom Traffic file or Not
}

string comp_rules_and_traffic(const Rule rules[], const int& ruleCount, const Packet packets[], const int& packetCount) {
    string result;
    for (int i = 0; i < packetCount; i++) {
        bool ruleApplied = false; //used to track wether rule applies to the incoming packet and initailed is to avoid junk values
        for (int j = 0; j < ruleCount; j++) {

            // for setting up ranges in IP addresses 
            string startIP = rules[j].IP_Pro, endIP = rules[j].IP_Pro; // if there is no range, start and end IP are same
            size_t dashPos = rules[j].IP_Pro.find('-');  //
            if (dashPos != string::npos) {
                startIP = rules[j].IP_Pro.substr(0, dashPos); //printing before -
                endIP = rules[j].IP_Pro.substr(dashPos + 1); //printing after -
            }
            //Checking conditions 
            if ((rules[j].App == "SRC" && packets[i].srcIP >= startIP && packets[i].srcIP <= endIP) ||
                (rules[j].App == "DST" && packets[i].dstIP >= startIP && packets[i].dstIP <= endIP) ||
                (rules[j].App == "PRO" && rules[j].IP_Pro == packets[i].protocol)) {
                result += "SRC " + packets[i].srcIP + "\t"+ " DST " + packets[i].dstIP + "\t" + " PRO " + packets[i].protocol + "\t" + rules[j].Decision + "\t" + to_string(rules[j].Rule_Num) + "\n";
                ruleApplied = true;
                break;
            }
        } 
        // For the condition if no rule applies, this should
        if (!ruleApplied) {
            result += "SRC " + packets[i].srcIP +"\t" + " DST " + packets[i].dstIP +"\t"+ " PRO " + packets[i].protocol + "\t" + rules[ruleCount - 1].Decision + "\t" + to_string(rules[ruleCount - 1].Rule_Num) + "\n";
        }
    }
    // cout << result << endl;
    // For error handlin such as checking wether result is right or not
    return result;
}

// Function to store the result string in Result File
void make_result_file(const string& filename, const string& result) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file." << endl;
    }
    file << result;
}