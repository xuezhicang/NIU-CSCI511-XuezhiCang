#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include <string>
using std::string;
using std::getline;

#include <sstream>
using std::ostringstream;

#include <postgresql/libpq-fe.h>
string safe_string(PGconn *, const string & s);


int main(){
  

  string place_name_str;
  ostringstream query;
  PGresult * search_result;

  int nrows;
  int ncols;
  int donothing;


  //link the DB
  PGconn * mydb;

  mydb = PQconnectdb("host=courses password=your_password dbname='z1747635'");

  if(PQstatus(mydb) != CONNECTION_OK)
  {
      cerr << "Sorry, We can't connect to database" << endl;
      cerr << PQerrorMessage(mydb) << endl;
      return -1;
  }    
  cerr << "Great! we have connectted to database" << endl;

    
  cout << "Please input a place's name? ";
  getline(std::cin, place_name_str);


  query.str("");
  query << "select id, name,type,";
  query <<"max_lon_deg,max_lon_min,max_lon_sec,";
  query <<"min_lon_deg,min_lon_min,min_lon_sec,";
  query <<"min_lat_deg,min_lat_min,min_lat_sec,";
  query <<"max_lat_deg,max_lat_min,max_lat_sec from z1747635 where name='";
  query << safe_string(mydb, place_name_str) << "';";

  
  //cout<<place_name_str<<endl;
  //cout<<"query:  "<<query.str()<<endl;

  search_result = PQexec(mydb, query.str().c_str());

  if( PQresultStatus(search_result) != PGRES_TUPLES_OK)
    {
      cerr << "Error in query Please check your data and program!" << query.str() << endl;
      cerr << PQerrorMessage(mydb) << endl;
      PQfinish(mydb);
      return -1;
    }


  nrows = PQntuples(search_result);
  ncols = PQnfields(search_result);

  for(int row = 0; row < nrows; row++)
    {
      for(int col = 0; col < ncols; col++)
	    { 
	       if(col==0)
	       {
	           cout<<"ID is "<<PQgetvalue(search_result, row, col);           
	       }	      

	       if(col==1)
	       {
	           cout<<" Name is "<<PQgetvalue(search_result, row, col);           
	       }
	       if(col==2)
	       {
	           cout<<" Type is";
	           if(string(PQgetvalue(search_result, row, col))=="P")
	               cout<<" Place";
	           if(string(PQgetvalue(search_result, row, col))=="S")
	               cout<<" State";
	           if(string(PQgetvalue(search_result, row, col))=="M")
	               cout<<" Subcounty";
	           if(string(PQgetvalue(search_result, row, col))=="C")
	               cout<<" County";	               	               	               
	       }
	       
	       if(col==3)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	cout<<endl<<"no bounds info in the Database now";
	           else
	           {
	           	cout<<endl;
	           	cout<<"lon rangre is from "<<PQgetvalue(search_result, row, col)<<"degree ";
	           }
	           	
	       }
	       if(col==4)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"min ";
	       }	       
	       if(col==5)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"sec to ";
	       }	       

	       if(col==6)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"degree ";
	       }
	       if(col==7)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"min ";
	       }	       
	       if(col==8)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"sec; the range of lat is from ";
	       }	       


	       if(col==9)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"degree ";
	       }
	       if(col==10)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"min ";
	       }	       
	       if(col==11)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"sec to ";
	       }	       

	       if(col==12)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"degree ";
	       }
	       if(col==13)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing = 1;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"min ";
	       }	       
	       if(col==14)
	       {
	           if(string(PQgetvalue(search_result, row, col))=="")
	           	donothing++;
	           else
	           	cout<<PQgetvalue(search_result, row, col)<<"sec";
	       }

	      //cout << PQgetvalue(search_result, row, col) << " ";
	    }
      cout << endl;
    }

  PQfinish(mydb);
  return 0;

}


string safe_string(PGconn * mydb, const string & s)
{
  char * buffer;
  int len;

  len = s.length();
  buffer = new char[2*len+1];

  PQescapeStringConn(mydb, buffer, s.c_str(), len, nullptr);

  string result(buffer);
  delete [] buffer;
  return result;
}
