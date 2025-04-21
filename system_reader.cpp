#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "system_reader.h"

using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool parseKeyValue(const string& line, string& key, string& value)    //parse key:val pair
{
    size_t colonPos = line.find(':');    //every valid line has :  
    if (colonPos == string::npos) 
    {
        return false;
    }
    
    key = line.substr(0, colonPos);
    value = line.substr(colonPos + 1);
    
    
    size_t keyStart = key.find_first_not_of(" \t");    //trim whitespace
    size_t keyEnd = key.find_last_not_of(" \t");
    if (keyStart != string::npos && keyEnd != string::npos)      //extract key
    {
        key = key.substr(keyStart, keyEnd - keyStart + 1);
    } 
    else 
    {
        key = "";
    }
    
    
    size_t valueStart = value.find_first_not_of(" \t");     //extract val
    size_t valueEnd = value.find_last_not_of(" \t");
    if (valueStart != string::npos && valueEnd != string::npos) 
    {
        value = value.substr(valueStart, valueEnd - valueStart + 1);
    } 
    else 
    {
        value = "";
    }
    
    return true;
}


bool readNextDataLine(ifstream &in, string &out)      //reads next meaningful line from input file
{
    while (getline(in, out)) 
    {

        if (out.empty()) 
            continue;
        

        int i = 0;
        while (i < out.length() && isspace(out[i])) 
            i++;
        if (i == out.length()) 
            continue;
        
        
        if (out[i] == '#') continue;
        
        return true;
    }
    return false;
}


bool readSystem(const string& filename, System& sys)     //reads the equantions 
{
    ifstream inFile(filename);
    if (!inFile.is_open()) 
    {
        cerr << "Error: Cannot open input file " << filename << endl;
        return false;
    }
    
    string line;
    bool gotType = false;
    bool gotDimensions = false;
    int rowsRead = 0;
    
    while (readNextDataLine(inFile, line)) 
    {
        string key, value;
        
        if (parseKeyValue(line, key, value)) 
        {
        
            if (key == "FME_type") 
            {
                
                if (value != "real") 
                {
                    cerr << "Error: Expected FME_type: real" << endl;
                    return false;
                }
                gotType = true;
            }
            else if (key == "dimensions") 
            {
                istringstream iss(value);
                if (!(iss >> sys.rows >> sys.cols)) 
                {
                    cerr << "Error: Bad dimensions format" << endl;
                    return false;
                }
                gotDimensions = true;
            }
            else if (key.substr(0, 4) == "row_") 
            {
                
                if (!gotDimensions) 
                {
                    cerr << "Error: Dimensions must be specified before rows" << endl;
                    return false;
                }
                
                istringstream iss(value);
                for (int j = 0; j < sys.cols; j++) 
                {
                    if (!(iss >> sys.A[rowsRead][j])) 
                    {
                        cerr << "Error: Bad coefficient format in row " << rowsRead << endl;
                        return false;
                    }
                }
                
                if (!(iss >> sys.b[rowsRead])) 
                {
                    cerr << "Error: Missing b value in row " << rowsRead << endl;
                    return false;
                }
                
                rowsRead++;
            }
        }
        else {
            
            if (!gotDimensions) 
            {
                cerr << "Error: Dimensions must be specified before row data" << endl;
                return false;
            }
            
            istringstream iss(line);
            for (int j = 0; j < sys.cols; j++) 
            {
                if (!(iss >> sys.A[rowsRead][j])) 
                {
                    cerr << "Error: Bad coefficient format in row " << rowsRead << endl;
                    return false;
                }
            }
            
            if (!(iss >> sys.b[rowsRead])) 
            {
                cerr << "Error: Missing b value in row " << rowsRead << endl;
                return false;
            }
            
            rowsRead++;
        }
    }
    
    
    if (!gotType) 
    {
        cerr << "Error: FME_type not specified" << endl;
        return false;
    }
    
    if (!gotDimensions) 
    {
        cerr << "Error: Dimensions not specified" << endl;
        return false;
    }
    
    if (rowsRead < sys.rows) 
    {
        cerr << "Error: Not enough data rows. Expected " << sys.rows 
             << ", got " << rowsRead << endl;
        return false;
    }
    
    return true;
}


bool readIntSystem(const string& filename, IntSystem& sys) 
{
    ifstream inFile(filename);
    if (!inFile.is_open()) 
    {
        cerr << "Error: Cannot open input file " << filename << endl;
        return false;
    }
    
    string line;
    bool gotType = false;
    bool gotDimensions = false;
    int rowsRead = 0;
    
    while (readNextDataLine(inFile, line)) 
    {
        string key, value;
        
        if (parseKeyValue(line, key, value)) 
        {
            
            if (key == "FME_type") 
            {
                
                if (value != "integer") 
                {
                    cerr << "Error: Expected FME_type: integer" << endl;
                    return false;
                }
                gotType = true;
            }
            else if (key == "dimensions") 
            {
                istringstream iss(value);
                if (!(iss >> sys.rows >> sys.cols)) 
                {
                    cerr << "Error: Bad dimensions format" << endl;
                    return false;
                }
                gotDimensions = true;
                
        
                for (int j = 0; j < sys.cols; j++) {
                    sys.isExact[j] = true;
                } }


            else if (key.substr(0, 4) == "row_") 
            {
                
                if (!gotDimensions) 
                {
                    cerr << "Error: Dimensions must be specified before rows" << endl;
                    return false;
                }
                
                istringstream iss(value);
                for (int j = 0; j < sys.cols; j++) 
                {
                    if (!(iss >> sys.A[rowsRead][j])) 
                    {
                        cerr << "Error: Bad coefficient format in row " << rowsRead << endl;
                        return false;
                    }
                }
                
                if (!(iss >> sys.b[rowsRead])) 
                {
                    cerr << "Error: Missing b value in row " << rowsRead << endl;
                    return false;
                }
                
                rowsRead++;
            }
        }
        else 
        {
            
            if (!gotDimensions) 
            {
                cerr << "Error: Dimensions must be specified before row data" << endl;
                return false;
            }
            
            istringstream iss(line);
            for (int j = 0; j < sys.cols; j++) 
            {
                if (!(iss >> sys.A[rowsRead][j])) 
                {
                    cerr << "Error: Bad coefficient format in row " << rowsRead << endl;
                    return false;
                }
            }
            
            if (!(iss >> sys.b[rowsRead])) 
            {
                cerr << "Error: Missing b value in row " << rowsRead << endl;
                return false;
            }
            
            rowsRead++;
        }
    }
    
    
    if (!gotType) 
    {
        cerr << "Error: FME_type not specified" << endl;
        return false;
    }
    
    if (!gotDimensions) 
    {
        cerr << "Error: Dimensions not specified" << endl;
        return false;
    }
    
    if (rowsRead < sys.rows) 
    {
        cerr << "Error: Not enough data rows. Expected " << sys.rows 
             << ", got " << rowsRead << endl;
        return false;
    }
    
    return true;
}


void printSystem(const System& sys) 
{
    cout << "System of inequalities:" << endl;
    for (int i = 0; i < sys.rows; i++) 
    {
        for (int j = 0; j < sys.cols; j++) 
        {
            cout << sys.A[i][j] << "x" << j;
            if (j < sys.cols - 1) 
            {
                if (sys.A[i][j+1] >= 0) cout << " + ";
                else cout << " ";
            }}
        
        cout << " <= " << sys.b[i] << endl;
    }
    cout << endl;
}


void printIntSystem(const IntSystem& sys) 
{
    cout << "System of inequalities:" << endl;
    for (int i = 0; i < sys.rows; i++) 
    {
        for (int j = 0; j < sys.cols; j++) 
        {
            cout << sys.A[i][j] << "x" << j;
            if (j < sys.cols - 1) 
            {
                if (sys.A[i][j+1] >= 0) cout << " + ";
                else cout << " ";
            }}
        cout << " <= " << sys.b[i] << endl;
    }
    cout << endl;
}
