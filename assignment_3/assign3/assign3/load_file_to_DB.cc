#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include <string>
using std::string;
using std::stoi;
using std::stof;
using std::to_string;

#include <fstream>
using namespace std;

#include <postgresql/libpq-fe.h>

int main()
{

    string state_id_str;
    string county_id_str;
    string place_or_subcounty_str;
    string id_str;


    string min_lon_str;
    string min_lat_str;
    string max_lon_str;
    string max_lat_str;

    int state_id;
    int county_id;
    int place_or_subcounty_id;
    int id_num;

    int min_lon_deg;
    int min_lon_min;
    double  min_lon_sec;

    int min_lat_deg;
    int min_lat_min;
    double  min_lat_sec;
    
    int max_lon_deg;
    int max_lon_min;
    double  max_lon_sec;

    int max_lat_deg;
    int max_lat_min;
    double  max_lat_sec;


    string state_code;
    string county_code;
    string subcounty_or_place_code;
    string region_type;
    string year_of_dataset;
    string name_string;
    string name_string_clean;

    int nrows_checking_ID;
    int nrows_checking_ID_and_name;
    int nrows_checking_ID_and_boundary;

    string selection_query_checking_ID;
    string selection_query_checking_ID_and_name;
    string selection_query_checking_ID_and_boundary;

    PGresult * selection_result_check_ID;
    PGresult * selection_result_check_ID_and_name;
    PGresult * selection_result_check_ID_and_boundary;

    string insert_query;
    string update_query;

    PGresult * insert_result;
    PGresult * update_result;
    
    
    // filr location and name example:
    string state_file = "/home/turing/duffin/courses/cs511/data/state.bnd";
    string county_file = "/home/turing/duffin/courses/cs511/data/county.bnd";
    string place_file = "/home/turing/duffin/courses/cs511/data/state17s.bnd";
    string name_file = "/home/turing/duffin/courses/cs511/data/TGR13089.RTC";
    
    string input_file_location;
    string file_type;
    
    cout<<"Please input your file's location"<<endl;
    cin>>input_file_location;
    cout << "your file is: "<<endl;

    cout << "Please input the type of your data."<<endl;
    cout << "if your data is staet boundary, please type S"<<endl;
    cout << "if your data is county boundary, please type C"<<endl;
    cout << "if your data is subcounty boundary, please type M"<<endl;
    cout << "if your data is place boundary, please type P"<<endl;
    cout << "if your data is name file, please type N"<<endl;
    cin >> file_type;
    cout << "your input string is: "<<file_type<<endl;


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

    
    if(file_type == "S")
    {
	cout << "get a State boundary file"<<endl;
	//input_file_location = state_file;
    }

    else if(file_type == "C")
    {
	cout << "get a County boundary file"<<endl;
	//input_file_location = county_file;
    }     

    else if(file_type == "M")
    {
	cout << "get a Subcountry boundary file"<<endl;
	//input_file_location = place_file;	
    }
        

    else if(file_type == "P")
    {
	cout << "get a place boundary file"<<endl;
	//input_file_location = place_file;
    }
        

    else if(file_type == "N")
    {
	cout << "get a name file"<<endl;
	//input_file_location = name_file;
    }
        
    else
    {
        cout << "your input is not a recognizable file type. Bye!"<<endl;
        return -1;
    }
        
        
    //read the file line by line
    std::ifstream file(input_file_location);
    std::string str;
    while (std::getline(file, str)) {

    
        if(file_type == "N")
        {

            state_code = str.substr(5,2);
            county_code = str.substr(7,3);
            subcounty_or_place_code = str.substr(14,5);
            region_type = str.substr(24,1);
            year_of_dataset = str.substr(10,4);
            name_string = str.substr(52,60); 
            name_string_clean = name_string.erase(name_string.find_last_not_of("\t\n\v\f\r ") + 1);       



            if(state_code.find_first_not_of(' ') != std::string::npos && (region_type =="S" ||region_type =="C" || region_type =="M" ||region_type =="P") && (year_of_dataset =="2000" || year_of_dataset.find_first_not_of(' ') == std::string::npos ))
            {
                
                //if the line represnets state
                if(county_code.find_first_not_of(' ') == std::string::npos && subcounty_or_place_code.find_first_not_of(' ') == std::string::npos && region_type == "S")
                {              
                    id_str = state_code;
                    id_num = stoi(id_str);
                    //cout<<" "<<"state:"<<name_string_clean<<" "<<id_num;
                }

                //if the line represnets conty
                else if(county_code.find_first_not_of(' ') != std::string::npos && subcounty_or_place_code.find_first_not_of(' ') == std::string::npos && region_type == "C") 
                {
                    id_str = state_code + county_code;
                    id_num = stoi(id_str);
                    //cout<<" "<<"county:"<<name_string_clean<<" "<<id_num;
                }

                //if the line represnets Subcounty
                else if(subcounty_or_place_code.find_first_not_of(' ') != std::string::npos && region_type =="M") 
                {
                    id_str = state_code + subcounty_or_place_code;
                    id_num = stoi(id_str);
                    //cout<<" "<<"Subcounty:"<<name_string_clean<<" "<<id_num;
                }

                //if the line represnets place
                else if(subcounty_or_place_code.find_first_not_of(' ') != std::string::npos && region_type == "P") 
                {
                    id_str = state_code + subcounty_or_place_code;
                    id_num = stoi(id_str);
                    //cout<<" "<<"place:"<<name_string_clean<<" "<<id_num;
                }
            

            
                selection_query_checking_ID = "select * from z1747635 where ID=" + to_string(id_num)+";";
                selection_result_check_ID = PQexec(mydb,selection_query_checking_ID.c_str()); 
            
                selection_query_checking_ID_and_name = "select * from z1747635 where ID=" + to_string(id_num) +" and Name='"+ name_string_clean +"';";
                selection_result_check_ID_and_name = PQexec(mydb,selection_query_checking_ID_and_name.c_str()); 
                
                nrows_checking_ID = PQntuples(selection_result_check_ID);
                nrows_checking_ID_and_name = PQntuples(selection_result_check_ID_and_name);
                


                if( PQresultStatus(selection_result_check_ID) == PGRES_TUPLES_OK && PQresultStatus(selection_result_check_ID_and_name) == PGRES_TUPLES_OK && nrows_checking_ID==0 && nrows_checking_ID_and_name == 0)
                {
		            //insert a new tuple
	                cout<<"we will insert the name infomation of ID= " + to_string(id_num)+ " in the table"<<endl;
	                insert_query="Insert into z1747635(ID,type,name)"
		                "values (" + to_string(id_num)+ ",'"+ region_type +"','" +name_string_clean + "')";
	                insert_result = PQexec(mydb,insert_query.c_str()); 
                    
                    if( PQresultStatus(insert_result) != PGRES_COMMAND_OK)
	                {
	                    cout<<"error in the insert processing!! Please check the program or the data"<<endl;
	                }
                }
                else if(PQresultStatus(selection_result_check_ID) == PGRES_TUPLES_OK && PQresultStatus(selection_result_check_ID_and_name) == PGRES_TUPLES_OK && nrows_checking_ID>0 && nrows_checking_ID_and_name == 0)
                {
                    //update an existed tuple
                    cout<<"we will update the name infomation of ID = " + to_string(id_num)+ " in the table"<<endl;
                    update_query = "UPDATE z1747635 SET Name='" + name_string_clean+"' where ID=" + to_string(id_num);
                    update_result = PQexec(mydb,update_query.c_str()); 
                    if( PQresultStatus(update_result) != PGRES_COMMAND_OK)
	                {
	                    cout<<"error in the update processing!! Please check the program or the data"<<endl;
	                }                    
                    
                }
                else if(PQresultStatus(selection_result_check_ID) == PGRES_TUPLES_OK && PQresultStatus(selection_result_check_ID_and_name) == PGRES_TUPLES_OK && nrows_checking_ID>0 && nrows_checking_ID_and_name > 0)
                {
                    // do nothing becasue the cooresponding tuple is great now
                    cout<<"the boundary and name infomation of ID=" + to_string(id_num)+ " looks great! the program donothing here!"<<endl;
                
                }
                else
                {
                    // report error
                    if(PQresultStatus(selection_result_check_ID) != PGRES_TUPLES_OK || PQresultStatus(selection_result_check_ID_and_name) != PGRES_TUPLES_OK)
                    {
                        cout<<"error in the program or data!! Please check the program or the data"<<endl;
                    }
                    
                    
                }
            

            }
        }    
    
    
    

        else
        {
            if(file_type == "S")// if the file includes state boundary info
            {
                state_id_str = str.substr(5,2);
                id_str = state_id_str;
            }

            else if(file_type == "C") // if the file includes county boundary info
            {
                county_id_str = str.substr(2,5);
                id_str = county_id_str;
                
            }
            else if(file_type == "M" ) // if the file includes subcounty boundary info
            {
                place_or_subcounty_str = str.substr(0,7);
                id_str = place_or_subcounty_str;
            
            }

            else if(file_type == "P") // if the file includes place boundary info
            {
                place_or_subcounty_str = str.substr(0,7);
                id_str = place_or_subcounty_str;
            
            }


            string min_lon_str = str.substr(7,10);
            string min_lat_str = str.substr(17,9);
            string max_lon_str = str.substr(26,10);
            string max_lat_str = str.substr(36,9);

            //get ID num
            id_num = stoi (id_str);

                
            //get minimum lon
            min_lon_deg = stoi (min_lon_str.substr(0,4));
            min_lon_min = 60*(stof (min_lon_str.substr(4,6))/1000000);
            min_lon_sec = 60*(60*(stof (min_lon_str.substr(4,6))/1000000) - min_lon_min);
            
            //get minimum lat
            min_lat_deg = stoi (min_lat_str.substr(0,3));
            min_lat_min = 60*(stof (min_lat_str.substr(3,6))/1000000);
            min_lat_sec = 60*(60*(stof (min_lat_str.substr(3,6))/1000000) - min_lat_min);
        


            //get maxium lon
            max_lon_deg = stoi (max_lon_str.substr(0,4));
            max_lon_min = 60*(stof (max_lon_str.substr(4,6))/1000000);
            max_lon_sec = 60*(60*(stof (max_lon_str.substr(4,6))/1000000) - max_lon_min);


            //get maxium lat
            max_lat_deg = stoi (max_lat_str.substr(0,3));;
            max_lat_min = 60*(stof (max_lat_str.substr(3,6))/1000000);;
            max_lat_sec = 60*(60*(stof (max_lat_str.substr(3,6))/1000000) - max_lat_min);
            
            
            //string insert_query;
            //string selection_query;
            //PGresult * selection_result;
            //PGresult * insert_result;


            selection_query_checking_ID = "select * from z1747635 where ID=" + to_string(id_num)+";";

            selection_result_check_ID = PQexec(mydb,selection_query_checking_ID.c_str()); 

            selection_query_checking_ID_and_boundary = "select * from z1747635 where ID=" + to_string(id_num) +" and min_lat_deg=" + to_string(min_lat_deg)+";";

            selection_result_check_ID_and_boundary = PQexec(mydb,selection_query_checking_ID_and_boundary.c_str()); 



            nrows_checking_ID = PQntuples(selection_result_check_ID);
            nrows_checking_ID_and_boundary = PQntuples(selection_result_check_ID_and_boundary);

            if(PQresultStatus(selection_result_check_ID) != PGRES_TUPLES_OK || PQresultStatus(selection_result_check_ID_and_boundary) != PGRES_TUPLES_OK )
            {
                cout<<"error here"<<endl;
	     }
            if( PQresultStatus(selection_result_check_ID) == PGRES_TUPLES_OK && PQresultStatus(selection_result_check_ID_and_boundary) == PGRES_TUPLES_OK && nrows_checking_ID == 0 )
            {

                //insert the new tuple

                cout<<"we will insert the boundary infomation of ID= " + to_string(id_num)+ " to the table"<<endl;

                insert_query="Insert into z1747635(ID,type,"
                "min_lat_deg,min_lat_min,min_lat_sec,"
                "min_lon_deg,min_lon_min,min_lon_sec,"		
                "max_lat_deg,max_lat_min,max_lat_sec,"
                "max_lon_deg,max_lon_min,max_lon_sec)"
                "values (" + to_string(id_num)+ ",'"+ file_type +"'," + 
                to_string(min_lat_deg) +","+ to_string(min_lat_min)+","+to_string(min_lat_sec)+","+
                to_string(min_lon_deg) +","+ to_string(min_lon_min)+","+to_string(min_lon_sec)+","+
                to_string(max_lat_deg) +","+ to_string(max_lat_min)+","+to_string(max_lat_sec)+","+
                to_string(max_lon_deg) +","+ to_string(max_lon_min)+","+to_string(max_lon_sec)+");";
                insert_result = PQexec(mydb,insert_query.c_str()); 
                if( PQresultStatus(insert_result) != PGRES_COMMAND_OK)
                {
                    cout<<"error in the insert processing!! Please check the program or the data"<<endl;
                }
                


           }
           else if(PQresultStatus(selection_result_check_ID) == PGRES_TUPLES_OK && PQresultStatus(selection_result_check_ID_and_boundary) == PGRES_TUPLES_OK && nrows_checking_ID > 0 && nrows_checking_ID_and_boundary == 0)
           {
               //update the exieted tuple
               cout<<"we will update the boundary infomation of ID= " + to_string(id_num)+ " to the table"<<endl;
               update_query = "UPDATE z1747635 SET ("
                                "min_lat_deg,min_lat_min,min_lat_sec,"
                                "min_lon_deg,min_lon_min,min_lon_sec,"		
                                "max_lat_deg,max_lat_min,max_lat_sec,"
                                "max_lon_deg,max_lon_min,max_lon_sec)"
                                "=(" + 
                                to_string(min_lat_deg) +","+ to_string(min_lat_min)+","+to_string(min_lat_sec)+","+
                                to_string(min_lon_deg) +","+ to_string(min_lon_min)+","+to_string(min_lon_sec)+","+
                                to_string(max_lat_deg) +","+ to_string(max_lat_min)+","+to_string(max_lat_sec)+","+
                                to_string(max_lon_deg) +","+ to_string(max_lon_min)+","+to_string(max_lon_sec)+
                                ") where ID="+ to_string(id_num)+";"; 
                update_result = PQexec(mydb,update_query.c_str());   
                if( PQresultStatus(update_result) != PGRES_COMMAND_OK)
                {
                    cout<<"error in the update processing!! Please check the program or the data"<<endl;
                }            
           }
           else if(PQresultStatus(selection_result_check_ID) == PGRES_TUPLES_OK && PQresultStatus(selection_result_check_ID_and_boundary) == PGRES_TUPLES_OK && nrows_checking_ID > 0 && nrows_checking_ID_and_boundary > 0)
           {

                // do nothing becasue the cooresponding tuple is great now
                cout<<"the boundary and name infomation of ID=" + to_string(id_num)+ " looks great! the program donothing here!"<<endl;

           }
           else
           {
		// report error
               if(PQresultStatus(selection_result_check_ID) != PGRES_TUPLES_OK || PQresultStatus(selection_result_check_ID_and_boundary) != PGRES_TUPLES_OK)
               {
                 cout<<"error in the program or data!! Please check the program or the data"<<endl;
               }
           }

    }
    }
    
    
    cout<<"done"<<endl;
    PQfinish(mydb);
    return 0;

  
}

