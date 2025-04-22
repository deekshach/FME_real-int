#include <iostream>
#include "real_fme.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool hasContradiction(System sys)       //impossible cases-->quick exit
{
    for (int i=0; i<sys.rows; i++) 
    {
        bool allZeros = true;
        for (int j=0; j<sys.cols; j++) 
        {
            if (sys.A[i][j] != 0.0) 
            {
                allZeros = false;
                break;
            }
        }
        if (allZeros && sys.b[i] < 0.0) 
        {
            return true;
        }
    }
    return false;
}


System eliminateVariable(System sys, int col) //eliminate var at col
{
    int positive[MAX_ROWS];
    int negative[MAX_ROWS];
    int zero[MAX_ROWS];
    int posCount = 0;
    int negCount = 0;
    int zeroCount = 0;
    
    
    for (int i=0; i<sys.rows; i++)    //separating out coeffs based on sign in diff array 
    {
        if (sys.A[i][col]>0) 
        {
            positive[posCount++]=i;
        } 
        else if (sys.A[i][col]<0) 
        {
            negative[negCount++]=i;
        } 
        else 
        {
            zero[zeroCount++]= i;
        }
    }
    
    
    System newSys;
    newSys.cols = sys.cols;
    newSys.rows = posCount*negCount+zeroCount;    //possible number of permutation of eqns
    int newRow=0;
    
    
    for (int i=0;i<zeroCount; i++)       //copying 0 coeff rows
    {
        int row=zero[i];
        for (int j=0; j<sys.cols; j++) 
        {
            newSys.A[newRow][j]= sys.A[row][j];
        }
        newSys.b[newRow]=sys.b[row];
        newRow++;
    }
    
    
    for (int i=0; i<posCount; i++) //for non-zero coeff rows
    {
        for (int j=0; j<negCount; j++) 
        {
            int posRow= positive[i];
            int negRow= negative[j];
            
            double posCoeff= sys.A[posRow][col];
            double negCoeff= -sys.A[negRow][col];
            
    
            for (int k=0;k<sys.cols; k++)    //creating new ineq by div
            {
                newSys.A[newRow][k] = (sys.A[posRow][k] / posCoeff)+(sys.A[negRow][k] / negCoeff);
            }
            newSys.b[newRow] = (sys.b[posRow] / posCoeff)+(sys.b[negRow] / negCoeff);
            newRow++;
        }}
    
    return newSys;
}


bool solveFME(System sys) 
{
    cout << "Starting real-valued FME process..." << endl << endl;
    
    
    for (int col=0; col<sys.cols; col++)     //elim var loop
    {
        cout<<"Eliminating variable x"<<col<<endl;
        sys=eliminateVariable(sys, col);
        
        
        if (hasContradiction(sys))     //contradicting soln
        {
            cout<<"Contradiction found after eliminating x"<<col<<endl;
            return false;
        }}
    
return true;
}
