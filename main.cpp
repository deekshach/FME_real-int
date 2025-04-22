#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "system_reader.h"
#include "real_fme.h"
#include "integer_fme.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

string getFmeTypeFromConfig(const string& filename)     //getting config from config file
{
    ifstream file(filename);
    
    if (!file.is_open()) 
        {
            return "";
        }
    
    string line;
    
    while (getline(file, line)) 
    {
        
        if (line.empty() || line[0]=='#')           //skipping empty line and comments 
            {
                continue;
            }
        
        size_t colonPos = line.find(':');     //colon separator
        if (colonPos != string::npos) 
        {
            string key = line.substr(0, colonPos);
            string value = line.substr(colonPos + 1);
            
            
            key.erase(0, key.find_first_not_of(" \t"));    //whitespace trimming
            key.erase(key.find_last_not_of(" \t") + 1);
            
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (key == "FME_type") 
            {
                return value;
            }
        
}}
    
    return "";
}

void printUsage() 
{   cout << "===================================================================" << endl;
    
    cout << "Usage: ./fme [options]" << endl;
    cout << "Options:" << endl;
    cout << "  --input=filename        Input file (default: input.txt)" << endl;
    cout << "  --output=filename       Output file (default: output.txt)" << endl;
    cout << "  --help                  Display this message for help" << endl;
    cout << endl;
    cout << "input file format:" << endl;
    cout << "  FME_type: real/integer  Specify FME type" << endl;
    cout << "  dimensions: mXn         Number of inequalities (m) and variables (n)" << endl;
    cout << "  row_1: a11 a12 ... a1n b1" << endl;
    cout << "  row_2: a21 a22 ... a2n b2" << endl;
    cout << endl;
    cout << "Example: ./fme --input=integer_input.txt --output=results.txt" << endl;
    
    cout << "===================================================================" << endl;
}

int main(int argc, char* argv[]) 
{
    
    string inputFile = "input.txt";      //def input
    string outputFile = "output.txt";    // def out
    
    
    for(int i=1; i<argc; i++) 
    {
        string arg = argv[i];
        
        if (arg == "--help") 
        {
                printUsage();
                return 0;
        }

        else if (arg.substr(0,8)== "--input=") 
        {
            inputFile = arg.substr(8);
        }
        
        else if (arg.substr(0,9)== "--output=") 
        {
            outputFile = arg.substr(9);
        }
        
        else 
        {
            cout<<"Gee! I don't know this option: "<< arg <<endl;
            cout<<"Can you input an option which is present in my configuration?"<<endl;
            
            printUsage();
            return 1;
        }}
    

    string fmeType = getFmeTypeFromConfig(inputFile);    //type of fme from input file
    if (fmeType.empty()) 
    {
        cout << "Error: write something in the fme_type, no!" << endl;
        
        printUsage();
        return 1;
    }
    
    
    for (char &c : fmeType) 
    {
    c = tolower(c);    //making lower case
    }
    
    
    if (fmeType!="real" && fmeType!="integer") 
    {
        cout<<"Error: FME type must be 'real' or 'integer', got '"<<fmeType<<"'"<<endl;
        
        printUsage();
        return 1;
    }
    
    
    ofstream outFile(outputFile);    //output file
    if (!outFile.is_open()) 
    {
        cout<<"Error: Cannot open output file "<<outputFile<<endl;
        return 1;
    }
    
    
    streambuf* oldCoutBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());
    
    
    if (fmeType=="real") 
    {
        System sys;
        if (!readSystem(inputFile,sys)) 
        {
            
            cout.rdbuf(oldCoutBuf);
            cout<<"Error reading input file."<<endl;
            return 1;
        }
        
        
        cout<<"Processing real-valued system with "<<sys.rows<<" inequalities and "<<sys.cols<<" variables"<<endl<<endl;   //system configs
        
        
        printSystem(sys);  //printing initial system
        
        
        bool hasSolution = solveFME(sys);
        cout<<endl<<"Real-valued system has solution: " <<(hasSolution ? "YES" : "NO")<<endl;
    }
    
    else 
    { 
        IntSystem sys;    //integer fme
        if (!readIntSystem(inputFile, sys)) 
        {
        
            cout.rdbuf(oldCoutBuf);
            cout<<"Error reading input file."<<endl;
            return 1;
        }
        
        
        cout<<"Processing integer valued system with "<<sys.rows<<" inequalities and "<<sys.cols<<" variables"<<endl<<endl;   //system inmfo
        
    
        printIntSystem(sys);   //system
        
        
        solveIntegerFME(sys);
    }
    

    cout.rdbuf(oldCoutBuf);
    cout<<"Results written to "<<outputFile<<endl;
    
    return 0;
}
