#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <stdio.h>
#include <stack>
#include <variant>
#include <bitset>
#include <cstdlib>
#include <cmath>
#include <iomanip>
using namespace std;



class Posting
{
public:
    int doc_id;
    int tf;
    double idf;
    double tf_idf;

    Posting()
    {
        tf = 0;
        idf = 0;
        tf_idf = 0;
    }
};
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main()
{
    string input;
    getline(cin, input);

    vector<string> strings;

    // Use stringstream to split the input into individual strings
    stringstream ss(input);
    string token;
    while (getline(ss, token, ' ')) // Assuming ',' is the delimiter
    {
        
        strings.push_back(token);
    }

    // Print the individual strings
    for (string str : strings)
    {
        cout << str << endl;
    }

    cout<<log10(30 / 7);
    
    return 0;

}

