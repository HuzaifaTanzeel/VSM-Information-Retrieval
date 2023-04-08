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

bool is_stopword(const vector<string> &arr, const string &target)
{
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target)
        {

            return true;
        }
        else if (arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return false;
}

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

class Term
{
public:
    string name;
    vector<Posting> postings;

    void update_posting(string name, vector<Term> terms)
    {
    }
};

bool is_exist(const vector<pair<string, vector<Posting>>> &dic, const string &target)
{

    for (int i = 0; i < dic.size(); i++)
    {
        if (dic[i].first == target)
        {
            return true;
        }
    }

    return false;
}

int find_index(const vector<pair<string, vector<Posting>>> &dic, const string &target) // Find index of term
{
    int index = -1;
    for (int i = 0; i < dic.size(); i++)
    {
        if (dic[i].first == target)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        return index;
    }
    else
    {

        return -1;
    }
}

class Dictionary
{

public:
    vector<pair<string, vector<Posting>>> terms;

    void addTerm(vector<string> tokens, int doc_id)
    {

        for (int i = 0; i < tokens.size(); i++)
        {

            if (!is_exist(terms, tokens[i]))
            {

                if (tokens[i].length() > 1)
                {

                    Posting post;
                    post.doc_id = doc_id;
                    post.tf = 1;
                    post.idf = 0;
                    post.tf_idf = 0;

                    vector<Posting> v;

                    v.push_back(post);
                    terms.push_back(make_pair(tokens[i], v));

                    v.clear();
                }
            }
            else
            {
                bool check = 0;
                int ind = find_index(terms, tokens[i]);
                for (int l = 0; l < terms[ind].second.size(); l++)
                {
                    if (doc_id == terms[ind].second[l].doc_id)
                    {

                        terms[ind].second[l].tf += 1;
                        check = 1;
                        break;
                    }
                }

                if (check == 0)
                {
                    Posting post;
                    post.doc_id = doc_id;
                    post.tf = 1;
                    post.idf = 0;
                    post.tf_idf = 0;

                    terms[ind].second.push_back(post);
                }
            }

            for (int i = 0; i < terms.size(); i++)
            {

                for (int j = 0; j < terms[i].second.size(); j++)
                {
                    terms[i].second[j].idf = log10(30 / terms[i].second.size());
                    terms[i].second[j].tf_idf = terms[i].second[j].tf * terms[i].second[j].idf;
                }
            }
        }
    }

    void print_dictionary()
    {

        for (int i = 0; i < terms.size() - 3000; i++)
        {
            cout << terms[i].first << " : ";

            for (int j = 0; j < terms[i].second.size(); j++)
            {

                cout << terms[i].second[j].doc_id << "--";
                cout << terms[i].second[j].tf << "--";
                cout << terms[i].second[j].idf << "--";
                cout << terms[i].second[j].tf_idf;
                cout << " ,";
            }

            cout << endl;
        }


    }

    void sort_dictionary()
    {
        sort(terms.begin(),terms.end());
    }
};

/*vector<Posting> getPostings(string termName) {
    for (Term term : terms) {
        if (term.name == termName) {
            return term.postings;
        }
    }
    return vector<Posting>(); // return empty vector if term not found
}*/

/*

/*void print_terms()
{
   for (Term term : terms) {
        cout<<term.name;
    }
}*/

bool ends_with(string word, string suffix)
{
    if (word.length() < suffix.length())
    {
        return false;
    }
    return equal(suffix.rbegin(), suffix.rend(), word.rbegin());
}

void replace_suffix(string &word, string suffix, string replacement)
{
    if (ends_with(word, suffix))
    {
        word.replace(word.length() - suffix.length(), suffix.length(), replacement);
    }
}

bool isvowel(char c)
{
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
    {
        return true;
    }
    else
    {
        return false;
    }
}

string stem_porter(string word)
{
    // Rule 1
    replace_suffix(word, "ses", "s");
    replace_suffix(word, "xes", "x");
    replace_suffix(word, "zes", "z");
    replace_suffix(word, "ches", "ch");
    replace_suffix(word, "shes", "sh");
    replace_suffix(word, "s", "");

    // Rule 2
    replace_suffix(word, "ied", "i");
    replace_suffix(word, "ed", "");
    // replace_suffix(word, "d", "");

    // Rule 3
    replace_suffix(word, "ying", "ie");
    replace_suffix(word, "ing", "");
    replace_suffix(word, "ly", "");
    replace_suffix(word, "east", "est");

    // Rule 4
    if (ends_with(word, "y") && word.length() > 2 && !string("aeiou").find(word[word.length() - 2]) != string::npos)
    {
        word[word.length() - 1] = 'i';
    }

    // Rule 5
    if (ends_with(word, "bb") || ends_with(word, "dd") || ends_with(word, "ff") || ends_with(word, "gg") || ends_with(word, "mm") || ends_with(word, "nn") || ends_with(word, "pp") || ends_with(word, "rr") || ends_with(word, "tt"))
    {
        word.erase(word.length() - 1, 1);
    }

    if (ends_with(word, "e") && word.length() > 4)
    {
        if (word.length() > 1 && !ends_with(word, "le"))
        {
            word.erase(word.length() - 1, 1);
        }
        else if (word.length() > 2 && ends_with(word, "le") && string("aeiou").find(word[word.length() - 3]) == string::npos)
        {
            word.erase(word.length() - 2, 2);
        }
    }

    if (ends_with(word, "ter") && word[word.length() - 4] != 't')
    {
        word.erase(word.length() - 2, 2);
    }

    if (ends_with(word, "ion") || (ends_with(word, "ain")))
    {
        word.erase(word.length() - 3, 3);
    }

    if (ends_with(word, "iliti") || ends_with(word, "le") && word.length() > 8)
    {
        if (ends_with(word, "iliti"))
        {
            word.erase(word.length() - 5, 5);
        }
        else
        {
            word.erase(word.length() - 2, 2);
        }
    }
    if (ends_with(word, "ians") || ends_with(word, "ian") && word.length() > 8)
    {
        if (ends_with(word, "ians"))
        {
            word.erase(word.length() - 2, 2);
        }
        else
        {
            word.erase(word.length() - 1, 1);
        }
    }

    return word;
}

vector<string> read_stopwords()
{

    vector<string> stop_word;
    ifstream inFile;
    char chh;
    string stopdata = "";

    // open the file
    inFile.open("Stopword-List.txt");

    // check if the file was opened successfully
    if (!inFile)
    {
        cout << "Unable to open file";
    }
    bool flagg = 0;
    // read the stopword file character by character
    while (inFile.get(chh))
    {
        if ((int(chh) < 97 || int(chh) > 122 || int(chh) == 32) && flagg == 0)
        {
            stop_word.push_back(stopdata);
            stopdata = "";
            flagg = 1;
        }
        else if (int(chh) >= 97 && int(chh) <= 122)
        {
            stopdata = stopdata + chh;
            flagg = 0;
        }
    }
    inFile.close();

    sort(stop_word.begin(), stop_word.end());

    return stop_word;
}

string readfile(int file_no)
{

    char ch;
    string filedata = "";
    ifstream inFile;

    filedata = "";
    string fileName = to_string(file_no) + ".txt";

    inFile.open(fileName);

    // check if the file was opened successfully
    if (!inFile)
    {
        cout << "Unable to open file " << file_no << endl;
        return 0;
    }

    // read the file character by character
    while (inFile.get(ch))
    {

        filedata = filedata + ch;
    }
    cout << "Readed file " << file_no << endl;
    inFile.close();

    return filedata;
}

vector<string> tokenize(string &data, const vector<string> &stop_word)
{
    bool flag = 0;
    int x;
    vector<string> tokens;
    string curr_token = "";
    transform(data.begin(), data.end(), data.begin(), ::tolower);

    for (int i = 0; i < data.length(); i++)
    {
        int x = int(data[i]);

        if ((x >= 97 && x <= 122) && x != '\n')
        {

            curr_token += data[i];
        }
        else
        {

            if ((!is_stopword(stop_word, curr_token)) && (curr_token != "") && curr_token.length() > 1)
            {

                string stemmed_token = stem_porter(curr_token);
                tokens.push_back(stemmed_token); // Add current token to list of tokens
            }

            curr_token = "";

            if (data[i] == '\0' || (data[i] == '.' && data[i + 1] == '\0'))
            {
                break;
            }
        }
    }

    return tokens;
}

int main()
{

    vector<string> stop_word = read_stopwords();
    Dictionary dic;

    for (int i = 0; i < 30; i++)
    {
        string filedata = readfile(i + 1);
        vector<string> tokens = tokenize(filedata, stop_word);
        dic.addTerm(tokens, i + 1);
    }

    dic.print_dictionary();
    /*cout<<dic.getPostingslength("cricket");*/
}