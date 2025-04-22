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
    
    int lowerBounds[MAX_COLS];
    int upperBounds[MAX_COLS];
    bool hasBounds[MAX_COLS];
    
    for (int i = 0; i < sys.cols; i++) 
    {
        lowerBounds[i]=-1000;  // Def lower bound
        upperBounds[i]=1000;   // def upper bnd
        hasBounds[i]=false;
    }
    
    
    for (int i=0;i<sys.rows; i++) 
    {
        
        int nonZeroCount = 0;
        int nonZeroIndex = -1;
        
        for (int j=0; j<sys.cols; j++) 
        {
            if (sys.A[i][j]!=0) 
            {
                nonZeroCount++;
                nonZeroIndex = j;
            }
        }
        
        
        if (nonZeroCount == 1)    //if only 1 var
        {
            int coefficient=sys.A[i][nonZeroIndex];
            int rhs=sys.b[i];
            
            if (coefficient>0) 
            {
                
                int bound = rhs/ coefficient;
                if (!hasBounds[nonZeroIndex] || bound < upperBounds[nonZeroIndex]) 
                {
                    upperBounds[nonZeroIndex]=bound;
                    hasBounds[nonZeroIndex]=true;
                }
            } 
            else if (coefficient<0) 
            {
            
                int bound = rhs/coefficient;
                if (!hasBounds[nonZeroIndex] || bound > lowerBounds[nonZeroIndex]) 
                {
                    lowerBounds[nonZeroIndex] = bound;
                    hasBounds[nonZeroIndex] = true;
                }
            }}}
    
    
    for (int var=0; var<sys.cols; var++) 
    {
        for (int indent=0; indent<var; indent++) 
        {
            cout << "    ";
        }
        
        if (hasBounds[var]) {
            cout << "for (int x" << var << " = " << lowerBounds[var] 
                 << "; x" << var << " <= " << upperBounds[var] 
                 << "; x" << var << "++) {" << endl;
        } else {
            cout << "for (int x" << var << " = /* unbound lower */; x" << var 
                 << " <= /* unbound upper */; x" << var << "++) {" << endl;
        }
        
        if (!sys.isExact[var]) 
        {
            for (int indent=0; indent<=var; indent++) 
            {
                cout << "    ";
            }
            cout << "// Warning: Projection for x" << var << " was inexact" << endl;
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
    }
}


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
