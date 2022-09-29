#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <queue>


using namespace std;

struct Data
{
    int pid;

    int arrival;
    int finish;

    int priority;

    int burst;
    int left;

    int waiting;
    int turnaround; 
};

struct Data_pprrq
{ 
    int priority ;
    queue<Data> samepriority ; 
};


vector<Data> datalist;
vector<Data> fcfslist;
vector<Data> rrlist;
vector<Data> srtflist;
vector<Data> pprrlist;
vector<Data> hrrnlist;
vector<Data> ganttlist;


int spilt( string &line );
void readandAcc(string line);
void arrivalandpid(vector<Data> &list );
void arrivalandleft(vector<Data> &list );
void arrivalandpriority(vector<Data> &list );
void priority(vector<Data_pprrq> &list );
void pid(vector<Data> &list );
bool notend(vector<Data> list);
int find(vector<Data> list, int pid );
void check_delete(vector<Data_pprrq> &pprrqueue);
void write_ganttlist(string filename,int waynumber);
void write_waitingtime(string filename,int waynumber);
void write_turnaroundtime(string filename,int waynumber);

void FCFS();
void RR(int timeslice);
void SRTF();
void PPRR(int timeslice);
void HRRN();


int main(){
  string filename = "\0";
  fstream infile,outfile;
  int waynumber = 0, timeslice = 0 ;
  int linenumber = 0, time = 0  ;
  

  cout << "Input file name(input_...):\n" ;
  //filename = "input3";
  cin >> filename;

  while ( filename != "0") {

    //開檔案
    infile.open(filename + ".txt", ios::in);

    //輸入wayofnumber和timeslice
    string line = "/0";
    getline(infile, line);
    waynumber = spilt(line);
    timeslice =  spilt(line);
    //timeslice =  3;
    //cout << waynumber  << endl ;
    //cout << timeslice  << endl ;

    //丟掉說明欄
    getline(infile, line );
    //cout << line << endl ;

    datalist.clear();
    fcfslist.clear();
    rrlist.clear();
    srtflist.clear();
    pprrlist.clear();
    hrrnlist.clear();
    //cout<< datalist.size();

    //輸入資料
    while (getline(infile, line) ) {
        linenumber++;
        if( line.size() > 0 )
          readandAcc(line);
        //cout << line << endl ;   
        //cin.get();
    } // end while
    //cout << "8888888888888888";
    //cout<< datalist.size();

    //開啟輸出檔
    outfile.open( "out_"+filename + ".txt", ios::out);
    
    
    switch (waynumber) {
        case 1:{ //FCFS
            outfile << "FCFS\n" ;
            outfile.close();

            FCFS();
            write_ganttlist(filename,1);
            break;
        }
        case 2:{ //RR
            outfile << "RR\n" ;
            outfile.close();

            RR(timeslice);
            write_ganttlist(filename,2);
            break;
        }
        case 3:{ //SRTF
            outfile << "SRTF\n" ;
            outfile.close();

            SRTF();
            write_ganttlist(filename,3);
            break;
        }
        case 4: { // PPRR
            outfile << "Priority RR\n" ;
            outfile.close();

            PPRR(timeslice);
            write_ganttlist(filename,4);
            break;
        }
        case 5: { // HRRN
            outfile << "HRRN\n" ;
            outfile.close();

            HRRN();
            write_ganttlist(filename,5);
            break;
        }
        case 6: { // ALL
            outfile << "All\n" ;
            outfile.close();
            
            FCFS();
            write_ganttlist(filename,1);
            
            RR(timeslice);
            write_ganttlist(filename,2);

            SRTF();
            write_ganttlist(filename,3);

            PPRR(timeslice);
            write_ganttlist(filename,4);

            HRRN();
            write_ganttlist(filename,5);

            break;
        }
        default:
            break;
    }  // end switch

    write_waitingtime(filename,waynumber);

    write_turnaroundtime(filename,waynumber);

   cout << "\n\nSuccessful!Check the output:"<< "out_"+filename + ".txt" << "\n";
    
   cout << "\n\nInput file name(input_...):\n";
   cin >> filename ;

   //datalist = {};
   infile.close();
   outfile.close();

  }

  cin.get();

}



//*******************************************************************************//
//*                        STORE  INPUT DATA                                    *//
//*******************************************************************************//

int spilt( string &line ){
    int number = 0 ;
    int whitespace = 0, i = 0 ;
    stringstream ss;
    ss << line;
    ss >> number;

    while( i < line.size() ){
      if( line[i] == ' '){
         whitespace++; 
      } // if
      i++;
    }  //while
    line.erase(0,whitespace); //刪除wayofnumber及空格

    return number ;
}  // split()


void readandAcc(string line){
    int i = 0 ; 
    vector<string> result ;
    string temp = "\0"; 
    //datalist.clear(); 
	             	
    stringstream input(line);
    while(input>>temp){
      result.push_back(temp); 
    } // while
	            
    // cout << atoi(result[i].c_str())  << endl ;
    Data current ;
    current.pid = atoi(result[i].c_str());
    //cout << current.pid  << '\t' ;
    i++;
    current.burst = atoi(result[i].c_str());
    //cout << current.burst  << '\t' ;
    current.left = current.burst;
    i++;
    current.arrival = atoi(result[i].c_str());
    //cout << current.arrival  << '\t' ;
    i++;
    current.priority = atoi(result[i].c_str());
    //cout << current.priority  << endl ;
    
    datalist.push_back(current);
    //cout<< datalist.size();

} // end readandAcc()


//*******************************************************************************//
//*                             PreSort                                         *//
//*******************************************************************************//

void arrivalandpid(vector<Data> &list ){

    for ( int i = list.size() - 1; i > 0; i-- ) {
      bool swapped = false;
      for ( int j = 0; j < i; j++ ) {
        if ( ( list[j].arrival > list[j + 1].arrival ) || ( ( list[j].arrival == list[j + 1].arrival ) && list[j].pid > list[j + 1].pid ) ) {
          swap( list[j], list[j + 1] );
          swapped = true;
        }
      }
      if ( !swapped ) break;

    }

}  //arrivalandpid(vector<Data> list )


void arrivalandleft(vector<Data> &list ){

    for ( int i = list.size() - 1; i > 0; i-- ) {
      bool swapped = false;
      for ( int j = 0; j < i; j++ ) {
        if ( ( list[j].arrival > list[j + 1].arrival ) || ( ( list[j].arrival == list[j + 1].arrival ) && list[j].left > list[j + 1].left ) ) {
          swap( list[j], list[j + 1] );
          swapped = true;
        }
      }
      if ( !swapped ) break;

    }

}  //arrivalandleft(vector<Data> list )

void arrivalandpriority(vector<Data> &list ){

    for ( int i = list.size() - 1; i > 0; i-- ) {
      bool swapped = false;
      for ( int j = 0; j < i; j++ ) {
        if ( ( list[j].arrival > list[j + 1].arrival ) || ( ( list[j].arrival == list[j + 1].arrival ) && list[j].priority > list[j + 1].priority ) ) {
          swap( list[j], list[j + 1] );
          swapped = true;
        }
      }
      if ( !swapped ) break;

    }

}  //arrivalandpriority(vector<Data> list )

void priority(vector<Data_pprrq> &list ){

    for ( int i = list.size() - 1 ; i > 0; i-- ) {
      bool swapped = false;
      for ( int j = 0; j < i; j++ ) {
        if ( ( list[j].priority > list[j + 1].priority )  ) {
          swap( list[j], list[j + 1] );
          swapped = true;
        }
      }
      if ( !swapped ) break;

    }

}  //priority(vector<Data> list )

void pid(vector<Data> &list ){

    for ( int i = list.size() - 1; i > 0; i-- ) {
      bool swapped = false;
      for ( int j = 0; j < i; j++ ) {
        if (  list[j].pid > list[j + 1].pid  ) {
          swap( list[j], list[j + 1] );
          swapped = true;
        }
      }
      if ( !swapped ) break;

    }

}  //arrivalandpid(vector<Data> list )



//*******************************************************************************//
//*                         Scheduling                                          *//
//*******************************************************************************//

void FCFS(){
    Data current ;
    int next = -1;
    bool findnext = false ;
    ganttlist.clear();
    int time = 0 ;

    fcfslist.assign(datalist.begin(), datalist.end());

    arrivalandpid( fcfslist );

    for( time = 0 ; notend(fcfslist) ; time++ ){

      if( fcfslist[next].left == 0 ) {
        fcfslist[next].finish = time ;
        next = -1 ;
      }
        //fcfslist[next].finish = time ;

      findnext = false ;
      for(int i = 0 ; fcfslist[i].arrival <= time && findnext==false  ; i++  ){
        if( fcfslist[i].arrival <= time && fcfslist[i].left > 0 ){
          next = i;
          findnext = true ;
        } // if
      } // end for

      if( next == -1  ){
        current.pid = 99;
        current.arrival = 999;
      }
      else{
        current.pid = fcfslist[next].pid;
      }

      ganttlist.push_back(current);
      fcfslist[next].left = fcfslist[next].left - 1 ;

    }
    
    if( fcfslist[next].left == 0 ) 
        fcfslist[next].finish = time ;
    /*
    for( int i = 0 ; i <  fcfslist.size() ; i ++ ){
      //cout << fcfslist[i].pid << '\t';
      for(int j = i ;  fcfslist[j].left > 0 ; fcfslist[j].left-- ){
        current.pid = fcfslist[j].pid;
        //cout <<  current.pid ;
        ganttlist.push_back(current);
      }
    }
    */
   /*
    for(int i = 0 ; i < ganttlist.size() ; i++ ) 
      cout << ganttlist[i].pid << '\t';
    cout << '\n';
   */
}  // FCFS()

void RR(int timeslice){
   int next = -1,timesliceused = 0 ;
   Data current;
   int time = 0 ;
   queue<Data> rrqueue;
   ganttlist.clear();

   rrlist.assign(datalist.begin(), datalist.end());

   arrivalandpid( rrlist );
   for( time = 0 ; notend(rrlist) ; time++ ){

     if( rrlist[next].left == 0 ) {
       rrlist[next].finish = time ;
       //next = -1 ;
     }
        

     //當下進來的資料
     for(int i = 0 ; rrlist[i].arrival <= time ; i++  ){
       if (rrlist[i].arrival == time ){
         //cout << rrlist[i].pid <<  ":"  <<rrlist[i].arrival << '\n';
         rrqueue.push(rrlist[i]);
       }
     }

     //若上一個執行的 還未執行完畢
     if( timesliceused >= timeslice && next != -1 && rrlist[next].left > 0 ){
      rrqueue.push(rrlist[next]);
     }
    
    //判斷需不需要換下一位
    if (  (rrlist[next].left == 0 && rrqueue.size()==0) || (timesliceused == 0 && rrqueue.size()==0) ){ //沒有上一位
      timesliceused = 0 ;
      //next = -1 ;
    }  
    else if (  timesliceused == 0  || timesliceused >= timeslice || rrlist[next].left == 0  ){
      timesliceused = 1 ;
      if(rrqueue.size()!= 0){
        next = find( rrlist , rrqueue.front().pid);
        rrqueue.pop();
      }
    }
    else
      timesliceused ++;
    
    if( next == -1 ||( rrqueue.size()==0 && rrlist[next].left == 0  ) || timesliceused == 0  )
        current.pid = 99;
    else
        current.pid = rrlist[next].pid;
      ganttlist.push_back(current);
      //cout << current.pid << '\t';
      rrlist[next].left = rrlist[next].left - 1 ;
    

   }

   if( rrlist[next].left == 0 ) 
        rrlist[next].finish = time ;
   /*
   for(int i = 0 ; i < ganttlist.size() ; i++ ) 
      cout << ganttlist[i].pid << '\t';
    cout << '\n';
   */
} // RR()

void SRTF(){
   int next = -1,shortest = 999 ;
   int time = 0 ;
   Data current;
   ganttlist.clear();

   srtflist.assign(datalist.begin(), datalist.end());

   arrivalandpid( srtflist );

   for( time = 0 ; notend(srtflist) ; time++ ){
      
      if( srtflist[next].left == 0 ) 
        srtflist[next].finish = time ;
     
      shortest = 999;
      next = -1 ;
      //找出已經到達且left最小者
      for(int i = 0 ; srtflist[i].arrival <= time ; i++  ){
        if( srtflist[i].left > 0 ){
          if( srtflist[i].left < shortest || ( srtflist[i].left == shortest && srtflist[i].arrival < srtflist[next].arrival ) 
            || ( srtflist[i].left == shortest && srtflist[i].arrival == srtflist[next].arrival  && srtflist[i].pid <  srtflist[next].pid )  ){
            shortest = srtflist[i].left ;
            next = i;
          } // if
          
        } // if
      } // end for

      if( next == -1 )
        current.pid = 99;
      else
        current.pid = srtflist[next].pid;
      ganttlist.push_back(current);
      srtflist[next].left = srtflist[next].left - 1 ;
   }

   if( srtflist[next].left == 0 ) 
        srtflist[next].finish = time ;

  /*
   for(int i = 0 ; i < ganttlist.size() ; i++ ) 
      cout << ganttlist[i].pid << '\t';
    cout << '\n';
  */
} // SRTF()
   
void PPRR(int timeslice){
   int next = -1,timesliceused = 0 ;
   int hpp = 999;
   int time = 0 ;
   bool inserted = false;
   Data current;
   vector<Data_pprrq> pprrqueue;
   Data_pprrq temp ;
   ganttlist.clear();

   pprrlist.assign(datalist.begin(), datalist.end());

   arrivalandpid( pprrlist );
   /*
   cout << "pprrsort";
   for(int i = 0 ; i < pprrlist.size() ; i++ ) 
      cout << pprrlist[i].pid << '\t';
    cout << '\n';
   */

   for( time = 0 ; notend(pprrlist) ; time++ ){

     if( pprrlist[next].left == 0 ) {
       pprrlist[next].finish = time ;
       //next = -1;
       //timesliceused = 0 ;
     }
       // pprrlist[next].finish = time ;
     if(current.pid != 99)
       hpp = pprrlist[next].priority ;

     //當下進來的資料
     for(int i = 0 ; pprrlist[i].arrival <= time ; i++  ){
       if (pprrlist[i].arrival == time ){
        //cout << pprrlist[i].pid <<  ":"  <<pprrlist[i].priority <<" " << pprrqueue.size() << '\n';
         for(int j = 0 ; j < pprrqueue.size() ; j++  ){ //有跟當下進入者一樣priority的資料
           if ( pprrlist[i].priority == pprrqueue[j].priority ){
             pprrqueue[j].samepriority.push(pprrlist[i]);
             inserted = true ;
           }
         }

         if( inserted == false ){ //沒有跟當下進入者一樣priority的資料
           temp.priority = pprrlist[i].priority;
           while (! temp.samepriority.empty())  
             temp.samepriority.pop();
           temp.samepriority.push(pprrlist[i]);
           pprrqueue.push_back(temp);
           priority(pprrqueue);
         }

         inserted = false;
       }
     }

     //若上一個執行的 還未執行完畢
     if(  timesliceused >= timeslice && next != -1 && pprrlist[next].left > 0 ){
      for(int j = 0 ; j < pprrqueue.size() && inserted == false ; j++  ){
           if ( inserted == false && pprrlist[next].priority == pprrqueue[j].priority ){
             pprrqueue[j].samepriority.push(pprrlist[next]);
             inserted = true ;
           }
      }
      if( inserted == false ){ //沒有一樣priority的資料
        temp.priority = pprrlist[next].priority;
        while (! temp.samepriority.empty())  
          temp.samepriority.pop();
        temp.samepriority.push(pprrlist[next]);
        pprrqueue.push_back(temp);
        priority(pprrqueue);
      }


      inserted = false;
     }

     //把空的節點刪掉
     check_delete(pprrqueue);
     priority(pprrqueue);
     /*
     for(int i = 0 ; i < pprrqueue.size(); i ++){
       cout << "(" << pprrqueue[i].priority << ")" << ": ";
       cout << "size " << pprrqueue[i].samepriority.size() << "  : " ;
       cout << pprrqueue[i].samepriority.front().pid << "  " <<pprrqueue[i].samepriority.back().pid << '\n';
       
     }
     */

    
    //判斷下一位
    if( ( pprrlist[next].left == 0 && pprrqueue.size()==0) || (timesliceused == 0 && pprrqueue.size()==0)   ){
      /*
      for(int i = 0 ; i < pprrlist.size(); i++){
        if( pprrlist[i].left > 0  )
         cout <<  pprrlist[i].pid << " " <<  pprrlist[i].left << endl ;
      }
      cout << endl;
      */
      timesliceused = 0 ;
    }
    else if(  hpp > pprrqueue[0].priority ){ //有更高階來襲
    
       if(  timesliceused < timeslice && pprrlist[next].left > 0  ){
         for(int j = 0 ; j < pprrqueue.size() && inserted == false ; j++  ){
           if ( inserted == false && pprrlist[next].priority == pprrqueue[j].priority ){
             pprrqueue[j].samepriority.push(pprrlist[next]);
             inserted = true ;
           }
         }
         if( inserted == false ){ //沒有一樣priority的資料
            temp.priority = pprrlist[next].priority;
            while (! temp.samepriority.empty())  
              temp.samepriority.pop();
            temp.samepriority.push(pprrlist[next]);
            pprrqueue.push_back(temp);
            priority(pprrqueue);
         }

         inserted = false;
       } 

       hpp = pprrqueue[0].priority ;
       next = find( pprrlist , pprrqueue[0].samepriority.front().pid);
       //cout << pprrqueue[0].samepriority.front().pid << endl;
       pprrqueue[0].samepriority.pop();
       //cout << pprrqueue[0].samepriority.front().pid << endl; 
       timesliceused = 1 ;
    }
    else if(   timesliceused == 0  || timesliceused >= timeslice || pprrlist[next].left == 0){
        next = find( pprrlist , pprrqueue[0].samepriority.front().pid);
        //cout << pprrqueue[0].samepriority.front().pid << endl;
        pprrqueue[0].samepriority.pop();
        //cout << pprrqueue[0].samepriority.front().pid << endl;
        timesliceused = 1 ;
      }
    else {
        //pprrqueue[0].samepriority.pop();
        timesliceused++;
    }

    
    

    if( next == -1 ||( pprrqueue.size()==0 && pprrlist[next].left == 0  ) || timesliceused == 0 ){
        current.pid = 99;
        timesliceused = 0;
    }
    else
        current.pid = pprrlist[next].pid;
    ganttlist.push_back(current);
    pprrlist[next].left = pprrlist[next].left - 1 ;
    //cout << "next TIME:" <<  pprrlist[next].pid << ":" << pprrlist[next].left << endl;
    //cout << "[" <<time << "]" << "end" << '\n';
    //check_delete(pprrqueue);

   }

   if( pprrlist[next].left == 0 ) 
        pprrlist[next].finish = time ;

   /*
   for(int i = 0 ; i < ganttlist.size() ; i++ ) 
      cout << ganttlist[i].pid << '\t';
    cout << "\n";
   */

} // PPRR()

void HRRN(){
   Data current;
   int next = -1 ;
   float hrr = -1, rr = -1 ;
   int time = 0 ;
   ganttlist.clear();


   hrrnlist.assign(datalist.begin(), datalist.end());

   arrivalandpid( hrrnlist );

   for( time = 0 ; notend(hrrnlist) ; time++ ){
      hrr = -1;

      if( hrrnlist[next].left == 0 ) {
        hrrnlist[next].finish = time ;
        next = -1 ;
      }

      if( hrrnlist[next].left <= 0 ){
        //找出hrr者
        for(int i = 0 ; hrrnlist[i].arrival <= time ; i++  ){
          if( hrrnlist[i].left > 0 ){
            rr = (float)( time -  hrrnlist[i].arrival + hrrnlist[i].burst ) / hrrnlist[i].burst ; 
            if( rr > hrr || (rr == hrr &&  hrrnlist[i].arrival < hrrnlist[next].arrival ) 
              || ( rr == hrr && hrrnlist[i].arrival == hrrnlist[next].arrival  && hrrnlist[i].pid < hrrnlist[next].pid ) ){
              hrr = rr ;
              next = i;
            } // if
          } // if
        } // end for
      }

      if( next == -1  ){
        current.pid = 99;
        hrr =0 ;
        //current.arrival = 999;
      }
      else
        current.pid = hrrnlist[next].pid;
      ganttlist.push_back(current);
      //cout << current.pid ;
      hrrnlist[next].left = hrrnlist[next].left - 1 ;
   }
   
   if( hrrnlist[next].left == 0 ) 
        hrrnlist[next].finish = time ;

   /*
   for(int i = 0 ; i < ganttlist.size() ; i++ ) 
      cout << ganttlist[i].pid << '\t';
   cout << '\n';
  */
} // HRRN()

//*******************************************************************************//
//*                    help Scheduling                                          *//
//*******************************************************************************//
bool notend(vector<Data> list){
  for(int i = 0 ; i < list.size(); i++){
    if( list[i].left > 0  )
      return true;
  }

   return false;
}

int find(vector<Data> list, int pid ){
  for(int i = 0 ; i < list.size(); i ++ ){
    if( list[i].pid == pid )
      return i;
  }
 
  return -1 ;
}


void check_delete(vector<Data_pprrq> &pprrqueue){
  for(int i = 0 ; i < pprrqueue.size();i++){
    //cout << i << " " ;
      if( pprrqueue[i].samepriority.size()!= 0 &&  pprrqueue[i].samepriority.front().left <= 0 ){
        pprrqueue[i].samepriority.pop();
      }
      if(pprrqueue[i].samepriority.size()<= 0 ){
           pprrqueue.erase(pprrqueue.begin()+i);
           i = 0 ;
      }
  }
}

//*******************************************************************************//
//*                    write output                                             *//
//*******************************************************************************//

void write_ganttlist(string filename,int waynumber){
  fstream outfile;
  int convert ;
  char c;

  outfile.open( "out_"+filename + ".txt", ios::out | ios::app );

  if( waynumber == 1 ){
    outfile << "==        FCFS==\n" ;
  }
  else if( waynumber == 2 ){
    outfile <<  "==          RR==\n" ; 
  }
  else if( waynumber == 3 ){
    outfile << "==        SRTF==\n" ;
  }
  else if( waynumber == 4 ){
    outfile << "==        PPRR==\n" ;
  }
  else if( waynumber == 5 ){
    outfile << "==        HRRN==\n" ;
  }


  for(int i = 0 ; i < ganttlist.size() ; i++ ) {
    if( ganttlist[i].pid >= 99  ){
      outfile << '-';
      //cout << '-';
    }
    else if( ganttlist[i].pid > 9  ){
      convert = ganttlist[i].pid - 10 ;
      c = 'A' + convert ;
      outfile << c ;
      //cout << c ;
    }
    else{
      outfile << ganttlist[i].pid ;
      //cout << ganttlist[i].pid ;
    }
      
  }
  outfile <<  '\n';

  outfile.close();

} // write_ganttlist()


void write_waitingtime(string filename,int waynumber){
  fstream outfile;
  pid(datalist);

  outfile.open( "out_"+filename + ".txt", ios::out | ios::app );
  outfile << "===========================================================\n\n";
  outfile << "Waiting Time\n";
  if( waynumber == 1 ){
    outfile << "ID	FCFS\n" ;
  }
  else if( waynumber == 2 ){
    outfile <<  "ID	RR\n" ; 
  }
  else if( waynumber == 3 ){
    outfile << "ID	SRTF\n" ;
  }
  else if( waynumber == 4 ){
    outfile << "ID	PPRR\n" ;
  }
  else if( waynumber == 5 ){
    outfile << "ID	HRRN\n" ;
  }
  else if( waynumber == 6 ){
    outfile << "ID	FCFS	RR	SRTF	PPRR	HRRN\n" ;
  }
  outfile << "===========================================================\n";

  for(int i = 0 ; i < datalist.size(); i ++){
    outfile << datalist[i].pid << "\t"  ;
    if( waynumber == 1 ){
       outfile << fcfslist[find(fcfslist,datalist[i].pid)].finish-fcfslist[find(fcfslist,datalist[i].pid)].arrival-fcfslist[find(fcfslist,datalist[i].pid)].burst << "\n" ;
    }
    else if( waynumber == 2 ){
      outfile << rrlist[find(rrlist,datalist[i].pid)].finish-rrlist[find(rrlist,datalist[i].pid)].arrival-rrlist[find(rrlist,datalist[i].pid)].burst << "\n" ;
    }
    else if( waynumber == 3 ){
      outfile << srtflist[find(srtflist,datalist[i].pid)].finish-srtflist[find(srtflist,datalist[i].pid)].arrival-srtflist[find(srtflist,datalist[i].pid)].burst << "\n" ;
    }
    else if( waynumber == 4 ){
      outfile << pprrlist[find(pprrlist,datalist[i].pid)].finish-pprrlist[find(pprrlist,datalist[i].pid)].arrival-pprrlist[find(pprrlist,datalist[i].pid)].burst << "\n" ;
    }
    else if( waynumber == 5 ){
      outfile << hrrnlist[find(hrrnlist,datalist[i].pid)].finish-hrrnlist[find(hrrnlist,datalist[i].pid)].arrival-hrrnlist[find(hrrnlist,datalist[i].pid)].burst << "\n" ;
    }
    else if( waynumber == 6 ){
      outfile << fcfslist[find(fcfslist,datalist[i].pid)].finish-fcfslist[find(fcfslist,datalist[i].pid)].arrival-fcfslist[find(fcfslist,datalist[i].pid)].burst << "\t" ;
      outfile << rrlist[find(rrlist,datalist[i].pid)].finish-rrlist[find(rrlist,datalist[i].pid)].arrival-rrlist[find(rrlist,datalist[i].pid)].burst << "\t" ;
      outfile << srtflist[find(srtflist,datalist[i].pid)].finish-srtflist[find(srtflist,datalist[i].pid)].arrival-srtflist[find(srtflist,datalist[i].pid)].burst << "\t" ;
      outfile << pprrlist[find(pprrlist,datalist[i].pid)].finish-pprrlist[find(pprrlist,datalist[i].pid)].arrival-pprrlist[find(pprrlist,datalist[i].pid)].burst << "\t" ;
      outfile << hrrnlist[find(hrrnlist,datalist[i].pid)].finish-hrrnlist[find(hrrnlist,datalist[i].pid)].arrival-hrrnlist[find(hrrnlist,datalist[i].pid)].burst << "\n" ;
    }
     
  }  
  outfile.close();

}

void write_turnaroundtime(string filename,int waynumber){
  fstream outfile;
  pid(datalist);

  outfile.open( "out_"+filename + ".txt", ios::out | ios::app );
  outfile << "===========================================================\n\n";
  outfile << "Turnaround Time\n";
  if( waynumber == 1 ){
    outfile << "ID	FCFS\n" ;
  }
  else if( waynumber == 2 ){
    outfile <<  "ID	RR\n" ; 
  }
  else if( waynumber == 3 ){
    outfile << "ID	SRTF\n" ;
  }
  else if( waynumber == 4 ){
    outfile << "ID	PPRR\n" ;
  }
  else if( waynumber == 5 ){
    outfile << "ID	HRRN\n" ;
  }
  else if( waynumber == 6 ){
    outfile << "ID	FCFS	RR	SRTF	PPRR	HRRN\n" ;
  }
  outfile << "===========================================================\n";

  for(int i = 0 ; i < datalist.size(); i ++){
    outfile << datalist[i].pid << "\t"  ;
    if( waynumber == 1 ){
      outfile << fcfslist[find(fcfslist,datalist[i].pid)].finish-fcfslist[find(fcfslist,datalist[i].pid)].arrival << "\n" ;
    }
    else if( waynumber == 2 ){
      outfile << rrlist[find(rrlist,datalist[i].pid)].finish-rrlist[find(rrlist,datalist[i].pid)].arrival << "\n" ;
    }
    else if( waynumber == 3 ){
      outfile << srtflist[find(srtflist,datalist[i].pid)].finish-srtflist[find(srtflist,datalist[i].pid)].arrival << "\n" ;
    }
    else if( waynumber == 4 ){
      outfile << pprrlist[find(pprrlist,datalist[i].pid)].finish-pprrlist[find(pprrlist,datalist[i].pid)].arrival << "\n" ;
    }
    else if( waynumber == 5 ){
      outfile << hrrnlist[find(hrrnlist,datalist[i].pid)].finish-hrrnlist[find(hrrnlist,datalist[i].pid)].arrival << "\n" ;
    }
    else if( waynumber == 6 ){
      outfile << fcfslist[find(fcfslist,datalist[i].pid)].finish-fcfslist[find(fcfslist,datalist[i].pid)].arrival << "\t" ;
      outfile << rrlist[find(rrlist,datalist[i].pid)].finish-rrlist[find(rrlist,datalist[i].pid)].arrival << "\t" ;
      outfile << srtflist[find(srtflist,datalist[i].pid)].finish-srtflist[find(srtflist,datalist[i].pid)].arrival << "\t" ;
      outfile << pprrlist[find(pprrlist,datalist[i].pid)].finish-pprrlist[find(pprrlist,datalist[i].pid)].arrival << "\t" ;
      outfile << hrrnlist[find(hrrnlist,datalist[i].pid)].finish-hrrnlist[find(hrrnlist,datalist[i].pid)].arrival << "\n" ;
    }
     
  }
  outfile << "===========================================================\n\n";
    
  outfile.close();

}
