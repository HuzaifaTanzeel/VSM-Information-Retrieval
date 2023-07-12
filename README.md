# VSM-Information Retrieval

## How to Run?

1. Install the C/C++ extension for Visual Studio Code.
2. Obtain the latest version of Mingw-w64.
3. Open the Assignment Folder in Visual Studio Code.
4. The folder must contain the following files: a .cpp file, Stopword-List.txt, and all 30 .txt files.

## Compiling and Running in Shell Terminal

```
g++ -o test 20k-0305.cpp
./test
```

## Exceptions

- If the program displays "Wrong Format" after selecting an option, add `cin.ignore()` after `cin >> ask` in the main function.

## Process Flow

1. Read stop words into a vector, eliminating spaces, etc.
2. Data Structure: `vector<pair<string, vector<pair<int, vector<int>>>>> dic;`
   - The outer vector pair represents the term (string), and the inner vector pair represents the Docid (int) and positional indexes of that Docid for that particular term.
3. Read the files.
4. Tokenize the contents of the files, returning tokenized terms along with their positions.
5. Insert pairs of tokens with positions into the dictionary.
   - Insert only the Docid if the term is already present in the dictionary.
   - Insert only the position if the Docid is also present for that term.
6. Sort the dictionary to enable searching with a time complexity of nlogn.
7. Perform stemming along with tokenization.
8. Calculate tf-idf for each document corresponding to the term in the document.
9. Perform dot product and normalization.
10. Calculate tf-idf of the query vector.
11. Calculate cosine similarities.
12. Rank the documents based on cosine similarities.

Note: According to the instructor, since almost the entire class implemented the vector space model on the entire corpus without reducing dimensions, the answers may vary with respect to the gold set.

### Example Queries and Result Sets

- Query: cricket politics
  Result-Set: 5, 26, 29, 14, 25, 3, 21, 24, 11, 17

- Query: Dharamsala to Indore
  Result-Set: 17
  dharamsala to indore

- Query: retirement
  Result-Set: 14

- Query: test captain
  Result-Set: 3, 6, 21, 17, 14

- Query: pcb psl
  Result-Set: 11, 29

- Query: hate
  Result-Set: (empty)

- Query: bowling coach
  Result-Set: 29, 6, 24

- Query: relative comfort
  Result-Set: 11, 19, 2, 1, 15, 22, 7

- Query: possible
  Result-Set: 23, 15

- Query: batter bowler
  Result-Set: 2, 25, 27, 30, 9, 3, 16, 7, 23, 22
