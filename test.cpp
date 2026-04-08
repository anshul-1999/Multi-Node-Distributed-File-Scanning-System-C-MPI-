#include <conio.h>
#include <fstream.h>
#include <iomanip.h>
#include <iostream.h>

void main()
{
    clrscr();
    int i,j,k;
    for(i=1;i<=5;i++)
    {
        for(j=1;j<=i;j++)
        {
            cout<<j;
        }
        for(k=1;k<=5-i;k++)
        {
            cout<<" ";
        }
        for(k=1;k<=5-i;k++)
        {
            cout<<" ";
        }
        for(j=i;j>=1;j--)
        {
            cout<<j;
        }
        cout<<endl;
    }
    for(i=4;i>=1;i--)
    {
        for(j=1;j<=i;j++)
        {
            cout<<j;
        }
        for(k=1;k<=5-i;k++)
        {
            cout<<" ";
        }
        for(k=1;k<=5-i;k++)
        {
            cout<<" ";
        }
        for(j=i;j>=1;j--)
        {
            cout<<j;
        }
        cout<<endl;
    }
    getch();
}