#pragma once

#include "QStruct.h"
#include "dbstream.h"
#include "inputFileInstance.h"

/** Query processor */
class Processor : private boost::noncopyable {
    /** Archive of data streams - initStorage */
    std::map< std::string, boost::shared_ptr< dbStream > > storage ;

    /** This function assue data access
     *  Due each field is computed in form schema/query
     *  it schould have access to this query
     *  therefore argument is reference to this query
     *  main solution (we need it make it better)
    */
    boost::rational<int> computeValue(
        field & f,
        query & q
    );

    /** Variable that contains sources of data */
    map< string, inputDF > gFileMap ;

    /** Length of data streams processed by processor */
    map< string, int > gStreamSize ;

    /** Context variables */
    map< string, vector< number > > gContextValMap ;  // schema name/record values

    /** Context variables */
    map< string, int > gContextLenMap ;

    /** Context functions */
    number getValue( string streamName, int timeOffset, int schemaOffset ) ;

    /** Context functions */
    vector<number> getRow( string streamName, int timeOffset ) ;

    /** Function will return offsets according to stack operations
     *  when A#B offsets A i B are equal , when A+B then A=0, B=0+Size(A)
     */
    int getArgumentOffset( const string & streamName, const string & streamArgument ) ;

  public:

    /** Query processor works on core varialbe */
    Processor();

    /** String representation of stream */
    string printRowValue( const string query_name );

    /** This function take in proper moment data to process */
    void processRows( set < string > inSet );

    /** Main purpose of this function is fullfill all stream fields values
     *  on given moment by  realization all 1v2v3 elements stream programs
     */
    void updateContext( set < string > inSet ) ;

    /** This function return lenght of data stream */
    int getStreamCount( const string query_name );

    /** */
    number getValueOfRollup( const query &q , int offset );

};
