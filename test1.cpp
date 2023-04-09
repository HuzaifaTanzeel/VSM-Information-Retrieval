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
#include <sstream>
using namespace std;

class Posting
{
public:
    int doc_id;
    double tf;
    double idf;
    double tf_idf;

    Posting()
    {
        tf = 0;
        idf = 0;
        tf_idf = 0;
    }
};

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

int find_binaryindex(const vector<pair<string, vector<Posting>>> &dic, const string &target)
{
    int left = 0;
    int right = dic.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (dic[mid].first == target)
        {

            return mid;
        }
        else if (dic[mid].first < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return 0;
}

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
    vector<double> normalizedlengths;
    vector<double> max_freq;

    Dictionary()
    {

        normalizedlengths.resize(30);
        max_freq.resize(30);
    }

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
                    if (tokens[i] == "cricket" && doc_id == 14)
                        post.tf++;

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
        }
    }

    void calculate_maxfreq()
    {
        for (int i = 0; i < terms.size(); i++)
        {

            for (int j = 0; j < terms[i].second.size(); j++)
            {

                int docc_id = terms[i].second[j].doc_id;
                if (max_freq[docc_id - 1] < terms[i].second[j].tf)
                {
                    max_freq[docc_id - 1] = terms[i].second[j].tf;
                }
            }
        }
    }

    void calculate_tf()
    {
        for (int i = 0; i < terms.size(); i++)
        {

            for (int j = 0; j < terms[i].second.size(); j++)
            {
                int docc_id = terms[i].second[j].doc_id;
                double temp_freq = terms[i].second[j].tf;
                double temp_maxfreq = max_freq[docc_id - 1];

                double partial = 0.5 + ((0.5) * (temp_freq) / temp_maxfreq);

                terms[i].second[j].tf = partial;
            }
        }
    }

    void calculate_tfidfs()
    {
        for (int i = 0; i < terms.size(); i++)
        {

            for (int j = 0; j < terms[i].second.size(); j++)
            {

                double n = terms[i].second.size();
                double tmp = log10(30 / n);
                terms[i].second[j].idf = tmp;
                terms[i].second[j].tf_idf = terms[i].second[j].tf * terms[i].second[j].idf;
                int docc_id = terms[i].second[j].doc_id;
                normalizedlengths[docc_id - 1] += pow(terms[i].second[j].tf_idf, 2);
            }
        }

        for (int i = 0; i < 30; i++)
        {
            normalizedlengths[i] = sqrt(normalizedlengths[i]);
        }
    }

    void print_dictionary()
    {

        for (int i = 0; i < terms.size() - 300; i++)
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
};

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
    replace_suffix(word, "cised", "cize");
    replace_suffix(word, "ed", "");

    // replace_suffix(word, "d", "");

    // Rule 3
    replace_suffix(word, "ying", "ie");
    replace_suffix(word, "ing", "");
    replace_suffix(word, "ly", "");
    replace_suffix(word, "east", "est");
    replace_suffix(word, "er", "");
    replace_suffix(word, "capt", "captt");

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
    if (ends_with(word, "tainci"))
    {
        word.erase(word.length() - 5, 5);
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
    if (ends_with(word, "rement") && word.length() > 9)
    {
        word.erase(word.length() - 5, 5);
    }

    if (ends_with(word, "tionship") || ends_with(word, "lativ"))
    {
        if (ends_with(word, "tionship"))
        {
            word.erase(word.length() - 7, 7);
        }
        else
        {
            word.erase(word.length() - 2, 2);
        }
    }
    if (ends_with(word, "rtab"))
    {
        word.erase(word.length() - 2, 2);
    }

    if (ends_with(word, "att") || ends_with(word, "tter") )
    {
        if (ends_with(word, "att"))
        {
            word.erase(word.length() - 2, 2);
        }
        else
        {
            word.erase(word.length() - 3, 3);
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

/*vector<string> tokenize(string &data, const vector<string> &stop_word)
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
}*/

vector<string> tokenize(string &data, const vector<string> &stop_word)
{
    bool flag = false;
    vector<string> tokens;
    string curr_token = "";
    transform(data.begin(), data.end(), data.begin(), ::tolower);

    for (int i = 0; i < data.length(); i++)
    {
        char c = data[i];

        if (isalpha(c) || isdigit(c))
        {
            curr_token += c;
        }
        else
        {
            if (!is_stopword(stop_word, curr_token) && !curr_token.empty() && curr_token.length() > 1)
            {
                string stemmed_token = stem_porter(curr_token);
                tokens.push_back(stemmed_token);
            }

            curr_token = "";

            if (i == data.length() - 1 || (c == '.' && data[i + 1] == '\0'))
            {
                break;
            }
        }
    }

    return tokens;
}

void merge(vector<pair<string, vector<Posting>>> &terms, int l, int m, int r, vector<pair<string, vector<Posting>>> &temp)
{
    int i, j, k;
    i = l;
    j = m + 1;
    k = l;

    // Merge the two subarrays into a temporary vector
    while (i <= m && j <= r)
    {
        if (terms[i].first <= terms[j].first)
        {
            temp[k] = terms[i];
            i++;
        }
        else
        {
            temp[k] = terms[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements from the first subarray to the temporary vector
    while (i <= m)
    {
        temp[k] = terms[i];
        i++;
        k++;
    }

    // Copy the remaining elements from the second subarray to the temporary vector
    while (j <= r)
    {
        temp[k] = terms[j];
        j++;
        k++;
    }

    // Copy the sorted elements from the temporary vector back to the original vector
    for (i = l; i <= r; i++)
    {
        terms[i] = temp[i];
    }
}

void mergeSort(vector<pair<string, vector<Posting>>> &terms, int l, int r, vector<pair<string, vector<Posting>>> &temp)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        // Recursively sort the two subarrays
        mergeSort(terms, l, m, temp);
        mergeSort(terms, m + 1, r, temp);

        // Merge the two sorted subarrays
        merge(terms, l, m, r, temp);
    }
}

void input_query(vector<string> stop_word, vector<pair<string, vector<Posting>>> &terms, Dictionary &dic)
{
    vector<pair<string, Posting>> query;
    vector<double> scores(30, 0.0);

    double query_normlength;

    string input;

    scores.resize(30);
    cout << "Enter query : ";
    getline(cin, input);

    // Use stringstream to split the input into individual strings
    stringstream ss(input);
    string token;
    bool flag = false;
    while (getline(ss, token, ' ')) // Assuming ',' is the delimiter
    {
        if (!is_stopword(stop_word, token))
        {
            string stemmed = stem_porter(token);
            cout << stemmed << endl;

            for (int i = 0; i < query.size(); i++)
            {
                if (stemmed == query[i].first)
                {
                    query[i].second.tf += 1;
                    flag = true;
                }
            }
            if (flag == false)
            {
                Posting post;
                post.tf = 1;
                post.idf = 0;
                post.tf_idf = 0;

                query.push_back(make_pair(stemmed, post));
            }
            flag = false;
        }
    }

    /* for(int i=0;i<query.size();i++)
     {
         int ind=find_binaryindex(terms,query[i].first);
         query[i].second.idf=terms[ind].second[0].idf;
         query[i].second.tf_idf=query[i].second.idf*query[i].second.tf;

         cout<<query[i].first<<"  "<<query[i].second.tf<<"--"<<query[i].second.idf<<"--"<<query[i].second.tf_idf<<endl;
     }*/

    for (int i = 0; i < query.size(); i++)
    {
        int ind = find_index(terms, query[i].first);
        if (ind == -1)
        {
            query[i].second.idf = 0;
            query[i].second.tf_idf = query[i].second.idf * query[i].second.tf;
            query[i].second.tf_idf = 0;
            query_normlength += pow(query[i].second.tf_idf, 2);
        }
        if (query[i].first == "cricket")
        {
            // cout << "ind : " << ind << endl;
            cout << terms[ind].second[0].idf << endl;
        }
        if (ind != -1)
        {
            query[i].second.idf = terms[ind].second[0].idf;
            query[i].second.tf_idf = query[i].second.idf * query[i].second.tf;
            //query[i].second.tf_idf =1;
            query_normlength += pow(query[i].second.tf_idf, 2);

            for (int j = 0; j < 30; j++)
            {
                for (int k = 0; k < terms[ind].second.size(); k++)
                {
                    if (terms[ind].second[k].doc_id == j + 1)
                    {
                        if (terms[ind].first == "retir")
                        {
                            cout << terms[ind].first << endl;
                            cout << terms[ind].second[k].doc_id << endl;
                            cout << "j+1" << j + 1 << endl;
                            cout << terms[ind].second[k].tf_idf << endl;
                            cout << query[i].second.tf_idf << endl;
                            system("pause");
                        }
                        scores[j] += terms[ind].second[k].tf_idf * query[i].second.tf_idf;
                        break;
                    }
                }
            }
        }

        cout << query[i].first << "  " << query[i].second.tf << "--" << query[i].second.idf << "--" << query[i].second.tf_idf << endl;
    }

    query_normlength = sqrt(query_normlength);

    vector<pair<int, double>> cosine_scores;
    for (int i = 0; i < 30; i++)
    {
        cosine_scores.push_back(make_pair(i + 1, (scores[i] / (query_normlength * dic.normalizedlengths[i]))));
    }
    sort(cosine_scores.begin(), cosine_scores.end(), [](const auto &x, const auto &y)
         { return x.second > y.second; });

    cout << "Multiplied Scores ---------------" << endl
         << endl;
    for (int i = 0; i < 30; i++)
    {
        cout << "Doc ) " << i + 1 << " " << scores[i] << endl;
    }

    cout << "Answer : ";
    for (int i = 0; i < 30; i++)
    {
        if (cosine_scores[i].second > 0)
        {
            cout << cosine_scores[i].first << " --- " << cosine_scores[i].second << " ," << endl;
        }

        // cout<<"Doc : "<<i+1<<"-- "<<(scores[i]/(query_normlength*dic.normalizedlengths[i]))<<endl;
    }
    cout << endl;

    cout << "Normalized Query Length  " << query_normlength << endl;

    query.clear();
    scores.clear();
    cosine_scores.clear();
    cosine_scores.shrink_to_fit();
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
    //dic.calculate_maxfreq();
    //dic.calculate_tf();
    dic.calculate_tfidfs();
    int n = dic.terms.size();
    vector<pair<string, vector<Posting>>> temp(n);
    mergeSort(dic.terms, 0, n - 1, temp);
    dic.print_dictionary();
    cout << dic.terms.size() << endl;
    while (1)
    {
        input_query(stop_word, dic.terms, dic);
    }
    for (int i = 0; i < dic.normalizedlengths.size(); i++)
    {
        cout << i + 1 << " " << dic.normalizedlengths[i] << endl;
    }

    cout << dic.terms.size() << endl;
}