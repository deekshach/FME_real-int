#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

const int MAX_ROWS = 100;
const int MAX_COLS = 20;

struct System 
{
    double A[MAX_ROWS][MAX_COLS];    //coefficients of vars
    double b[MAX_ROWS];        //right hand side values
    int rows;                  //number of eqns
    int cols;                   //number of vars
};

struct IntSystem 
{
    int A[MAX_ROWS][MAX_COLS];
    int b[MAX_ROWS];
    int rows;
    int cols;
    bool isExact[MAX_COLS];    //for int if soln is exacft
};

#endif
