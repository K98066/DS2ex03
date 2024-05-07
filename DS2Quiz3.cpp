// 11020134 §f©v³Í 
// 11120117 ªL¸aµ¾ 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <math.h>

using namespace std;

struct data{
	
	char sid[10] ;
	char sname[10] ;
	unsigned char score[6] ;
	float mean ; // average score 

};

class database{
	
	private:
		
		// both these integer can use by mission 1 and 2 to output result 
		double averageSuccess ;
		double averageFail ;
		
		// for recording quadraticHash or doubleHash ( need to clear and initialize -1 when using it )
		vector<int> hashValue ; 
		
		vector<data> student ;
		vector<data> quadraticHash ;
		vector<data> doubleHash ;
	
	public:

	  int size() {
	
		return student.size();  	
	  	
	  }
	  
	  bool isEmpty( string fileType ) {
	  	
	  	if ( strcmp( fileType.c_str(), "quadratic" ) == 0 && quadraticHash.size() == 0 )
		  return true ;
		   
	  	if ( strcmp( fileType.c_str(), "double" ) == 0  && doubleHash.size() == 0 )
		  return true ;

	  	return false ;
	  	
	  }
	  
	  void clear() {
	  	
	  	student.clear();
	  	quadraticHash.clear();
	  	doubleHash.clear();
	  	
	  }

	  void TxtToBin( string fileName ) {
	
		data temp ;
	
		string input ;
	  	string binary ;

		ifstream in ;
	  	ofstream out ;

		memset( &temp, 0x00, sizeof(temp) );

		in.open( fileName.c_str() );

		// bin file already exist
		if ( in.good() )
		  return ;
		
		in.close();
		
		out.open( fileName.c_str(), ios::out | ios::binary );

		// write from head of file
		out.seekp( 0, out.beg );
	  	for ( int i = 0 ; i < student.size() ; i++ ) {
	  		
	  	  temp = student.at(i);
		  out.write( ( char* ) &temp, sizeof( temp ) );

		} 
	
	  	out.close();
	  	
	  }
	  
	  bool InfileBin( string fileName ) {
		
		int size = 0;
			
	    data temp ;

		string input ;

	  	ifstream in ;

		memset( &temp, 0x00, sizeof(temp) );

	  	in.open( fileName.c_str(), ios::in | ios::binary );
	
	  	if ( in.fail() )
	   	  return false;

		in.seekg( 0, in.end );
		size = in.tellg() / sizeof(temp) ;

		in.seekg( 0, in.beg );
	  	for ( int i = 0 ; i < size ; i++ ) {
	  		
	  	  in.read( ( char* ) &temp, sizeof( temp ) );

		  student.push_back( temp );

	  	} // while
	
	  	in.close();
	
	  }

	  bool InfileTxt( string fileName ) {
			
	    data temp ;

		string input ;

	  	ifstream in ;

	  	in.open( fileName.c_str() );
	
	  	if ( in.fail() )
	   	  return false;

	  	while ( ! in.eof() ) {

		  // check it is already empty ( read '\n' ) or not ( read id )
	      getline( in, input, '\t' ) ; 

	      if ( ! in.eof() ) {

	      	strcpy( temp.sid, input.c_str() );

	      	// read name
	  		getline( in, input, '\t' ) ;
	      	strcpy( temp.sname, input.c_str() );

			// read 6 scores
			for ( int i = 0 ; i < 6 ; i++ ) {
	
	  	  	  getline( in, input, '\t' ) ;
	  	  	  temp.score[i] = atoi( input.c_str() );
	
			} // for 
	  	  
	  	  	// read average score ( last data )
	  	  	getline( in, input, '\n' ) ;
	  	  	
			// string to float
	  	  	temp.mean = atof( input.c_str() ); 
	  	  	
	  	  	student.push_back( temp );
	  	  	
	      } // if
	  
	  	} // while
	
	  	in.close();
	
	  }
	  
	  bool CheckPrime( int input ) {
	  	
		int divisor = 2;
		
	  	while ( divisor <= input / 2 ) {
	  		
	  	  if ( input % divisor == 0 )
	  	    return false ;
	  	    
	  	  divisor++ ;

		} // while
	  	
	  	return true;
	  	
	  }
	  
	  int QuadraticStep( int hvalue, int multiplier ) {
	  	
	  	hvalue = hvalue + multiplier * multiplier ;
	  	
	  	if ( hvalue >= quadraticHash.size() )
	  	  hvalue %= quadraticHash.size();

		return hvalue;
	
	  }
	  
	  void QuadraticProbing() {
	  	
	  	int index = 0;
	  	int current = 0;
	  	int position = 0;
	  	
	  	int successSearch = 0;
	  	int successAmount = 0;

	  	int failSearch = 0;

	  	int multiplier = 0;
		int remainder = 0;

		int maxPrime = 0;	  	
	    int hvalue = 0;
	  	
	  	vector<int> searchPosition ;
	  	
		maxPrime = student.size() * 1.3 ;
		
		if ( student.size() * 1.3 > maxPrime )
		  maxPrime++ ; 

		while( ! CheckPrime( maxPrime ) )
		  maxPrime++ ;
		  
	  	// generate hash table and hvalue space
	  	hashValue.clear();
	  	for ( data temp ; quadraticHash.size() < maxPrime ; ) {

	  	  hashValue.push_back( -1 );
	  	  quadraticHash.push_back( temp );
	  	
	 	} // for

	  	while ( current < student.size() ) {

	  	  // calculate hash value
	  	  remainder = 1 ;
	  	  index = 0;
	  	  while ( student[current].sid[index] != '\0' ) {
	  	  	
	  	  	remainder *= (int) student[current].sid[index] ;
	  	  	remainder %= maxPrime ;
	  	    index++ ;
	  	  	
		  } // while
		  
		  hvalue = remainder ; 

	  	  // check data can put into hash table or not 
	  	  position = hvalue ;

		  // every key at least has one search
	  	  successSearch++ ;

	  	  multiplier = 0;
	  	  searchPosition.clear();
	  	  while ( hashValue[position] != -1 ) { // ! Exists( searchPosition, position ) && multiplier <= 15

			multiplier++;
			position = QuadraticStep( hvalue, multiplier );
	  	  	successSearch++ ;
	
		  } // while

	  	  // this position is empty
	  	  if ( hashValue[position] == -1 ) {
	  	  		
	  	  	hashValue[position] = hvalue ;
	  	  	strcpy( quadraticHash[position].sid, student[current].sid );
	  	  	strcpy( quadraticHash[position].sname, student[current].sname );
	  	  	quadraticHash[position].mean = student[current].mean ;

	  	  	successAmount++ ;

		  } // if
		  
		  current++ ;
	  		
		} // while
		
		
		// calculate the least time to comfirm the key is not exist 
		for ( index = 0 ; index < quadraticHash.size() ; index++ ) {
			
		  // every key at least has one search
			
	  	  multiplier = 0;
	  	  position = index ;
	  	  while ( hashValue[position] != -1 ) { // ! Exists( searchPosition, position ) && multiplier <= 15

			multiplier++;
			position = QuadraticStep( index, multiplier );
	  	  	failSearch++ ;
	
		  } // while
			
		} // for 

		averageSuccess = (double) successSearch / successAmount ;
		averageFail = (double) failSearch / quadraticHash.size() ;

	  }
	  
	  void DoubleHashing() {
	  	int temp = 0;
	  	
	  	int index = 0;
	  	int current = 0;
	  	int position = 0;
	  	
	  	int successSwitches = 0;
	  	int successAmount = 0;
	  	
	  	int failSwitches = 0;
	  	int failAmount = 0;

	  	int multiplier = 0;

		int maxPrime = 0;
		int max_key = 0; // smallest prime bigger than (data count / 2)
		int step = 0;
	  	long long int hvalue = 1;
		long long int copy_hvalue = 1;
	  	
	  	vector<int> searchPosition ;
	  	
		maxPrime = student.size() * 1.3 ;
		
		if ( student.size() * 1.3 > maxPrime )
		  maxPrime++ ; 

		while( ! CheckPrime( maxPrime ) )
		  maxPrime++ ;
		
		while( max_key <= (student.size() / 2) ) 
		  max_key++ ;
		
		while( ! CheckPrime( max_key ) )
		  max_key++ ;
		
	  	// generate hash table
	  	hashValue.clear();
	  	for ( data temp ; doubleHash.size() < maxPrime ; ) {

	  	  hashValue.push_back( -1 );
	  	  doubleHash.push_back( temp );
	  	
	    }

		// for double hashing
	  	while ( current < student.size() ) {
	  	
	  	  // calculate hash value
	  	  hvalue = 1;
	  	  index = 0;
	  	  while ( student[current].sid[index] != '\0' ) {
	  	  	
	  	  	hvalue *= (int) student[current].sid[index] ;
	  	    index++ ;	
	  	  	
		  } // while

		  // copy hvalue
		  copy_hvalue = hvalue;

	  	  // check data can put into hash table or not 
	  	  hvalue = hvalue % maxPrime ;
	  	  position = hvalue ;

		  // every key at least has one search
		  temp = 1;
	  	  searchPosition.clear();

		  step = max_key - (copy_hvalue % max_key);

		  // before the while function below. position is same as the h1 hash value
		  // the h2 function :
		  // check if position(h1's value) already have data.
		  // do : position add stop value
		  // don't : put in the data
		  while ( hashValue[position] != -1 ) {
		  	
			temp++ ;
			position = position + step;
			
			if (position >= doubleHash.size()) 
			  position = position - doubleHash.size();
			  

		  }

	  	  // this position is empty
	  	  if ( hashValue[position] == -1 ) {
	  	  	
	  	  	hashValue[position] = hvalue ;
	  	  	strcpy ( doubleHash[position].sid, student[current].sid );
	  	  	strcpy ( doubleHash[position].sname, student[current].sname );
	  	  	doubleHash[position].mean = student[current].mean ;

			successSwitches += temp ;
	  	  	successAmount++ ;
	  	  		
		  } // if
		  
		  current++ ;
	  		
		} // while

		averageSuccess = (double) successSwitches / successAmount ;

	  }

	  void Exists( vector<int> & searchPosition, vector<int> & searchTimes, int position ) {
	  	
	  	int index = 0;
	
	  	while ( index < searchPosition.size() ) {
	  	
	  	  if ( searchPosition[index] == position ) {
	
			searchTimes[index]++ ;
			return ;
	
		  } // if 
	  		
	  	  index++ ; 
	  		
		} // while
	  	
	  	searchPosition.push_back( position ) ;
	  	searchTimes.push_back( 1 ) ;
	  	
	  }

	  bool Search( string fileType ) {
	  	
	  	int multiplier = 0;
	  	int loopTimes = -1;
	  	vector <int> searchPosition ;
	  	vector <int> searchTimes ;
	  	
	  	int remainder = 1;
	  	int hvalue = 0;
	  	int position = 0;
	  	
	  	int step = 0;
	  	int max_key = 0;
	  	int attemps = 1;
	  	
	  	string studentID ;
	  	
	  	cout << "Input a student ID to search ([0] Quit): " ;

	  	cin >> studentID ;
	  	
	  	if ( strcmp ( studentID.c_str(), "0" ) == 0 )
	  	  return false ;
	  	
	  	cout << endl ;
	  	
	  	for ( int index = 0 ; studentID[index] != '\0' ; index++ ) {
	  	
	  	  remainder *= (int) studentID[index] ;
	  	  remainder %= quadraticHash.size() ;
	  	  	
		} // while

		hvalue = remainder ;
		position = hvalue ;
	  	
	  	if ( strcmp( fileType.c_str(), "quadratic" ) == 0 ) {

	  	  while ( hashValue[position] != -1 && strcmp( quadraticHash[position].sid, studentID.c_str() ) != 0 && searchTimes.size() != loopTimes ) {

			Exists( searchPosition, searchTimes, position );
			
		    for ( loopTimes = 0 ; searchTimes[loopTimes] == 2 ; loopTimes++ )
			  ;

			multiplier++;
			position = QuadraticStep( hvalue, multiplier );
	  	  	attemps++ ;
	
		  } // while
		
	  	  if ( strcmp( quadraticHash[position].sid, studentID.c_str() ) == 0 ) 
	  	    cout << "{ " << quadraticHash[position].sid << ", " << quadraticHash[position].sname << ", " << quadraticHash[position].mean << " } is found after " << attemps << " probes." << endl ;
	  	    
	  	  else
	  	    cout << studentID << " is not found after " << attemps << " probes." << endl ;

		} // if 


	  	if ( strcmp( fileType.c_str(), "double" ) == 0 ) {
	  	
		  while( max_key <= (student.size() / 2) ) 
		    max_key++ ;
		
		  while( ! CheckPrime( max_key ) )
		    max_key++ ;
	  	
		  step = max_key - (hvalue % max_key);
		  while ( hashValue[position] != -1 && strcmp( doubleHash[position].sid, studentID.c_str() ) != 0 )   {
		  	
			attemps++ ;
			position = position + step;
			
			if (position >= doubleHash.size()) 
			  position = position - doubleHash.size();

		  } // while
	  	
	  	  if ( strcmp( doubleHash[position].sid, studentID.c_str() ) == 0 ) 
	  	    cout << "{ " << doubleHash[position].sid << ", " << doubleHash[position].sname << ", " << doubleHash[position].mean << " } is found after " << attemps << " probes." << endl ;
	  	    
	  	  else
	  	    cout << studentID << " is not found after " << attemps << " probes." << endl ;	  
			  
		   
	  	
		} // if 
	  	
	  	return true ;
	  	
	  }
	  
	  void OutputResult( string fileType ) {
	  	
	  	if ( strcmp( fileType.c_str(), "quadratic" ) == 0 ) {
	  		
	  	  cout << "Hash table has been successfully created by Quadratic probing" << endl ;

		  // because double hashing dont need to find unsucessful search
		  cout << "unsuccessful search: " << fixed << setprecision(4) << averageFail << " comparisons on average" << endl ;

		}

	  	if ( strcmp( fileType.c_str(), "double" ) == 0 )
	  	  cout << "Hash table has been successfully created by Double hashing" << endl ;
	  	
		cout << "successful search: " << fixed << setprecision(4) << averageSuccess << " comparisons on average" << endl ;	


	  } 
	  
	  void OutFile( string fileName, string fileType ) {
	  	
	  	vector<data> outHash;
	  	
	  	ofstream out ;
	  	
	    out.open( fileName.c_str() , ios::out | ios::binary );
	  	
	  	if ( strcmp( fileType.c_str(), "quadratic" ) == 0 ) {
	  	  out << "--- Hash table created by Quadratic probing ---" << endl ;
		  outHash = quadraticHash;
		} // if

	  	if ( strcmp( fileType.c_str(), "double" ) == 0 ) {
	  	  out << "--- Hash table created by Double hashing ---" << endl ;
		  outHash = doubleHash;
		} // if
	  	  
	  	for ( int i = 0 ; i < outHash.size() ; i++ ) {
	  	
		  if ( i < 100 )	
		   out << "[ " << i << "] " ;
		  
		  else 
		   out << "[" << i << "] " ;
		  
		  if ( hashValue[i] != -1 )
	  	    out << hashValue[i] << ", " << outHash[i].sid << ", " << outHash[i].sname << ", " << outHash[i].mean ;
		  
		  out << endl ;
		  
		} // for

	  	if ( strcmp( fileType.c_str(), "quadratic" ) == 0 )
	  	  out << "-----------------------------------------------" << endl ;	  	

	  	if ( strcmp( fileType.c_str(), "double" ) == 0 )
	  	  out << "--------------------------------------------" << endl ;	

	  	  
	  	out.close();
	  	
	  }
	   
	  void Print() {
	  	
	  	// test function
	  	
	  	for ( int i = 0 ; i < student.size() ; i++ )
	  	  cout << student[i].sid << "\t" << student[i].sname << "\t" << student[i].score[0] << "\t" << student[i].score[1] << "\t" << student[i].score[2] << "\t" << student[i].score[3] << "\t" << student[i].score[4] << "\t" << student[i].score[5] << "\t" << student[i].mean << endl ; 

//	  	for ( int i = 0 ; i < quadraticHash.size() ; i++ )
//	  	  cout << "["  << i << "]" << quadraticHash[i].hvalue << "\t" << quadraticHash[i].sid << "\t" << quadraticHash[i].sname << "\t" << quadraticHash[i].mean << endl ;

	  } 

};

int main() {
	
  int command = 0;
  
  string fileNum ;
  string fileName ;
	
  database hash ;

  do {
	
	cout << endl ;
    cout << "******* Hash Table *****" << endl ;
  	cout << "* 0. Quit              *" << endl ;
  	cout << "* 1. Quadratic probing *" << endl ;
  	cout << "* 2. Double hashing    *" << endl ;
  	cout << "************************" << endl ;
  	cout << "Input a choice(0, 1, 2): " ;
  
	cin >> command ;

	if ( command == 1 ) {
		
		if ( hash.size() != 0 )
		  hash.clear();

		  cout << endl << "Input a file number ([0] Quit): " ;
		  cin >> fileNum ;
		  cout << endl ;
			
		  // scan data from bin first
		  fileName = "input" + fileNum + ".bin" ;
		  if ( strcmp( fileNum.c_str(), "0" ) != 0 && ! hash.InfileBin( fileName ) ) 
			cout << "### " << fileName << " does not exist! ###" << endl << endl ;
			
		  // when bin file does not exist, check txt file and build bin file
		  fileName = "input" + fileNum + ".txt" ;
		  if (  strcmp( fileNum.c_str(), "0" ) != 0 && hash.size() == 0 && ! hash.InfileTxt( fileName ) ) 
			cout << "### " << fileName << " does not exist! ###" << endl ;
		
		  if ( hash.size() != 0 ) {
		  	
		  	// check need to build bin file or not
		  	fileName = "input" + fileNum + ".bin" ;
		  	hash.TxtToBin( fileName );
		  	
		  	hash.QuadraticProbing();
		  	
		  	fileName = "quadratic" + fileNum + ".txt" ;
		  	hash.OutputResult( "quadratic" );
		  	hash.OutFile( fileName, "quadratic" );
		  	
		  	while ( hash.Search( "quadratic" ) )
		  	  ;
		  	
		  } // else
		  
		  cout << endl ;
		
	} // if 
	
	else if ( command == 2 ) {

		if ( hash.size() == 0 )
		  cout << "### Command 1 first. ###" << endl ;
		  
		else {

		  cout << endl ;
		
		  if ( hash.isEmpty( "double" ) )
		    hash.DoubleHashing();
		  	
		  fileName = "double" + fileNum + ".txt" ;
		  hash.OutputResult( "double" );
		  hash.OutFile( fileName, "double" );
		  
 		  while ( hash.Search( "quadratic" ) )
			;
		  	
		} // else 
			
		cout << endl ;
		
	} // if 
	
	else if ( command != 0 )
	  cout << endl << "Command does not exist!" << endl ;

  } while ( command != 0 );
	
  system("pause");

  return 0;
	
}
