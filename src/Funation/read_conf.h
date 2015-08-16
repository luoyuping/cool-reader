#include <vector>
#include <string> 
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;




class readMysqlConfig 
{ 
    public: 
        //readMysqlConfig ();
        readMysqlConfig(char* file):filename(file)
        {
            dealConfigFile();
        }
         ~readMysqlConfig () {}

        string getIP();
        string getUsername();
        string getPasswd(); 
        void debug();
    private: 
        string filename;
        string ip;
        string passwd;
        string username;
        void dealConfigFile();
              
}; 



