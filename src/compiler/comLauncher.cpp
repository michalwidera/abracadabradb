#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>
#include <boost/system/error_code.hpp>
#include <boost/regex.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>

#include <fstream>
#include <iostream>

#include <memory>

#include "../share/QStruct.h"

#include "compiler.hpp"

using namespace std;
using namespace boost;

using boost::lexical_cast;

extern string parser(vector<string> vsInputFile);
extern string storeParseResult(string sOutputFile);

extern qTree coreInstance ;

int main(int argc, char* argv[]) {
    // Clarification: When gcc has been upgraded to 9.x version some tests fails.
    // Bug appear when data are passing to program via script .sh
    // additional 13 (\r) character was append - this code normalize argv list.
    // C99: The parameters argc and argv and the strings pointed to by the argv array
    // shall be modifiable by the program, and retain their last-stored values
    // between program startup and program termination.
    for ( int i = 0 ; i < argc ;  i ++ ) {
        auto len = strlen( argv[i] ) ;

        if ( len > 0 )
            if ( argv[i][len - 1] == 13 ) {
                argv[i][len - 1] = 0 ;
            }
    }

    try {
        std::unique_ptr<ostream> p_ofs;
        namespace po = boost::program_options;
        string sOutputFile ;
        string sInputFile ;
        po::options_description desc("Avaiable options");
        desc.add_options()
        ("help,h", "Show program options")
        ("queryfile,q", po::value<string>(&sInputFile), "query set file")
        ("outfile,o", po::value<string>(&sOutputFile)->default_value("query.qry"), "output file")
        ;
        po::positional_options_description p;       //Assume that infile is the first option
        p.add("queryfile", -1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc ;
            return system::errc::success;
        }

        if ( vm.count("queryfile") == 0 ) {
            cout << argv[0] << ": fatal error: no input file" << endl ;
            cout << "compilation terminated." << endl ;
            return EPERM ; //ERROR defined in errno-base.h
        }

        string sQueryFile = vm["queryfile"].as< string >() ;
        string sOutFile   = vm["outfile"].as< string >() ;
        string sLinkFile  = "query.lkn" ;
        string sSubsetFile = "query.sub" ;
        //override defaults if filename is matching regexp
        {
            boost::regex filenameRe("(\\w*).(\\w*)");  //filename.extension
            boost::cmatch what;

            if ( regex_match(sQueryFile.c_str(), what, filenameRe) ) {
                string filenameNoExt = string( what[1] ) ;
                sLinkFile = filenameNoExt + ".lkn" ;
                sSubsetFile = filenameNoExt + ".sub" ;
            }
        }
        cout << "Input file:" << sQueryFile << endl ;
        cout << "Compile result:" << parser(load_file(sQueryFile)) << endl ;
        cout << "Store result:" << storeParseResult(sLinkFile) << endl ;
        //
        // Main Algorithm body
        //
        std::ifstream ifs( sLinkFile.c_str(), std::ios::binary);
        boost::archive::text_iarchive ia(ifs);
        ia >> coreInstance ;

        if ( coreInstance.size() == 0 ) {
            throw std::out_of_range("No queries to process found");
        }

        string sOutFileLog1   = vm["outfile"].as< string >() + ".lg1" ;
        dumpInstance(sOutFileLog1);
        int dAfterParserSize((int)coreInstance.size());
        string response ;
        response = simplifyLProgram() ;
        string sOutFileLog2   = vm["outfile"].as< string >() + ".lg2" ;
        dumpInstance(sOutFileLog2);
        response = prepareFields();
        string sOutFileLog3   = vm["outfile"].as< string >() + ".lg3" ;
        dumpInstance( sOutFileLog3);
        response = intervalCounter() ;
        response = convertReferences();
        response = replicateIDX();
        int dAfterCompilingSize((int)coreInstance.size());
        dumpInstance( sOutFile);
        int dSize ( dAfterCompilingSize - dAfterParserSize ) ;
        string sSize( "" ) ;

        if ( dSize > 0 ) {
            stringstream s ;
            s << dSize ;
            sSize = string( " + " ) + string( s.str() ) ;
        }
    } catch(std::exception &e) {
        cerr << e.what() << "\n";
        return system::errc::interrupted;
    }

    return system::errc::success;
}
