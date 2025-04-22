#include <iostream>
#include <cmath>
#include "integer_fme.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////



int gcd(int a, int b) 
{
    a=abs(a); b=abs(b);   //the coefficients can be -ve too;need to only take pos val
        while (b != 0) 
            {
                int temp = b;
                b = a % b;
                a = temp;
        }
    
return a;
}




int lcm(int a,int b) 
{
    if (a == 0 || b == 0) 
        return 0;
    
    return ((a/gcd(a, b))*b);
}




IntSystem eliminateVariableInt(IntSystem sys, int col)   //eliminating var at col 
{
    int positive[MAX_ROWS];
    int negative[MAX_ROWS];
    int zero[MAX_ROWS];
    int posCount = 0;
    int negCount = 0; 
    int zeroCount = 0;
    
    
for(int i=0; i<sys.rows; i++)          //separate rows for their sign
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
                zero[zeroCount++]=i;
}}
    
    
    IntSystem newSys;
    
    newSys.cols=sys.cols;
    
    //newSys.rows=posCount + negCount +zeroCount;     
    newSys.rows=posCount*negCount +zeroCount;     //to create inequality for every opposite sign coeff. pair
    
    int newRow = 0;
    bool anyInexact = false;
    
    
    for (int j=0; j<sys.cols; j++) 
    {
        newSys.isExact[j]=sys.isExact[j];
    }
    
    
    for (int i=0; i<zeroCount; i++)   //copying rows where 0 
    {
        int row = zero[i];
        for (int j=0; j<sys.cols; j++) 
        {
            newSys.A[newRow][j]=sys.A[row][j];
        }
        newSys.b[newRow]=sys.b[row];
        newRow++;
    }
    

    for (int i=0; i<posCount; i++)        //combining pos and neg riows 
    {
        for (int j=0; j<negCount; j++) 
        {
            int posRow=positive[i];
            int negRow=negative[j];
            
            int p =sys.A[posRow][col];
            int q =-sys.A[negRow][col];
            
            int multiplier = lcm(p,q);
            int m1=multiplier/p;
            int m2=multiplier/q;
            
            
            for (int k=0; k<sys.cols; k++)      //Creating new inequality
            {
                newSys.A[newRow][k] = m1*sys.A[posRow][k] + m2*sys.A[negRow][k];
            }
            
            newSys.b[newRow]=m1*sys.b[posRow] + m2*sys.b[negRow];
            
        
            if (gcd(p, q) != 1) 
            {
                anyInexact = true;
                cout << "  Variable x" << col << ": Projection is INEXACT (GCD of " << p << " and " << q << " is " << gcd(p, q) << ")" <<endl;
            }
            newRow++;
        }}
    
    
    if(anyInexact) 
    {
        newSys.isExact[col]=false;
    } 
    else if (posCount>0 && negCount>0) 
    {
        cout << "  Variable x" << col << ": Projection is EXACT" << endl;
    }
    
    return newSys;
}


void generateLoopNest(IntSystem sys)      //loop nest print
{
    cout<<endl<<"Loop nest for solution space:"<<endl<<endl;
    
    int lowerBounds[MAX_COLS];     //separate bound arrays
    int upperBounds[MAX_COLS];
    bool hasLowerBound[MAX_COLS];
    bool hasUpperBound[MAX_COLS];
    
    for (int i = 0; i < sys.cols; i++) 
    {
        lowerBounds[i]=0;     // def lower bopund
        upperBounds[i]=10;    // default upper bnd
        hasLowerBound[i]=false;
        hasUpperBound[i]=false;
    }
    
    
    for (int i=0;i<sys.rows; i++)       //counting non-zero coefficients in ineq 
    {
        
        int nonZeroCount=0;
        int nonZeroIndex=-1;
        
        for (int j=0; j<sys.cols; j++) 
        {
            if (sys.A[i][j]!=0) 
            {
                nonZeroCount++;
                nonZeroIndex = j;
            }}
        
        
        if (nonZeroCount == 1)    //if only 1 var
        {
            int coefficient=sys.A[i][nonZeroIndex];
            int rhs=sys.b[i];
            
            if (coefficient>0) 
            {
                
                int bound = rhs/ coefficient;
                if (!hasUpperBound[nonZeroIndex] || bound < upperBounds[nonZeroIndex]) 
                {
                    upperBounds[nonZeroIndex]=bound;
                    hasUpperBound[nonZeroIndex]=true;
                }
            } 
            else if (coefficient<0) 
            {
            
                int bound = -(rhs/coefficient);
                if (!hasLowerBound[nonZeroIndex] || bound > lowerBounds[nonZeroIndex]) 
                {
                    lowerBounds[nonZeroIndex] = bound;
                    hasLowerBound[nonZeroIndex] = true;
                }
            }
        }
        
        
        if (nonZeroCount==2)    //2-var ineq
        {
            int firstVar =-1, secondVar =-1;
            for (int j=0; j<sys.cols; j++) 
            {
                if (sys.A[i][j]!=0) 
                {
                    if (firstVar==-1) 
                        firstVar = j;
                    else 
                        secondVar = j;
                }}
            
            int a=sys.A[i][firstVar];
            int b=sys.A[i][secondVar];
            int rhs=sys.b[i];
            
        
            if (abs(a)==1 && abs(b)==1) 
            {
                if (a>0 && !hasUpperBound[firstVar]) 
                {
                    upperBounds[firstVar]=rhs;
                    hasUpperBound[firstVar]=true;
                }
                if (b > 0 && !hasUpperBound[secondVar]) 
                {
                    upperBounds[secondVar] = rhs;
                    hasUpperBound[secondVar] = true;
                }
}}}
    
    
    for (int var=0; var<sys.cols; var++) 
    {
        for (int indent=0; indent<var; indent++) 
        {
            cout << "    ";
        }
        
        cout << "for (int x" << var << " = ";
        
        if (hasLowerBound[var]) 
        {
            cout << lowerBounds[var];
        } 
        else 
        {
            cout << "0";  //default lower bound
        }
        
        cout << "; x" << var << " <= ";
        
        if (hasUpperBound[var]) 
        {
            cout << upperBounds[var];
        } 
        else 
        {
            cout << "10";  //default upper bnd
        }
        
        cout << "; x" << var << "++) {" << endl;
        
        if (!sys.isExact[var]) 
        {
            for (int indent=0; indent<=var; indent++) 
            {
                cout << "    ";
            }
            cout << "// PS: Projection for x" << var << " was inexact" << endl;
        }}
    
    
    for (int indent=0; indent<sys.cols; indent++) 
    {
        cout << "    ";
    }
    cout << "printf(\"(";
    for (int var=0; var<sys.cols; var++) 
    {
        cout << "%d";
        if (var<sys.cols - 1) 
            cout << ", ";
    }
    cout << ")\\n\", ";
    
    for (int var=0; var<sys.cols; var++) 
    {
        cout << "x" << var;
        if (var<sys.cols - 1) 
            cout << ", ";
    }
    cout << ");" << endl;
    
    
    for (int var=sys.cols-1; var >= 0; var--) 
    {
        for (int indent=0; indent<var; indent++) 
        {
            cout << "    ";
        }
        cout << "}" << endl;
    }}


void solveIntegerFME(IntSystem sys) 
{
    IntSystem original=sys;
    
    cout<< "Starting integer FME process..." <<endl<<endl;
    

    for (int col=0; col<sys.cols; col++)     //eliminate var by col
    {
        cout<<"Eliminating variable x"<<col<<":"<<endl;
        sys=eliminateVariableInt(sys,col);
    }
    
    cout<<endl<<"Summary of projections:"<<endl;
    for (int col=0; col<original.cols; col++) 
    {
        cout<<"Variable x"<<col<<": "<<(sys.isExact[col] ? "EXACT" : "INEXACT")<<" projection"<<endl;
    }
    
    generateLoopNest(original);
}
