#ifndef EM_PORT_API
#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#if defined(__cplusplus)
#define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#else
#define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#endif
#else
#if defined(__cplusplus)
#define EM_PORT_API(rettype) extern "C" rettype
#else
#define EM_PORT_API(rettype) rettype
#endif
#endif
#endif
#include <iostream>
#include <string.h>
#include <algorithm>
#include <stack>
#include <vector>
#include <map>
#include <fstream>
#include <queue>
#include <unordered_map>
using namespace std;
typedef vector<int> EdgePos;
typedef vector<int> CornerPos;
typedef vector<int> edgeOrientation;
typedef vector<int> cornerOrientation;
typedef pair<EdgePos, edgeOrientation> Edges;
typedef pair<CornerPos, cornerOrientation> Corners;
typedef pair<Edges, Corners> Cube;
map<char, EdgePos> nextEdgePos = {{'R', {0, 5, 2, 3, 4, 9, 1, 7, 8, 6, 10, 11}},
                                  {'U', {1, 2, 3, 0, 4, 5, 6, 7, 8, 9, 10, 11}},
                                  {'F', {0, 1, 6, 3, 4, 5, 10, 2, 8, 9, 7, 11}},
                                  {'B', {4, 1, 2, 3, 8, 0, 6, 7, 5, 9, 10, 11}},
                                  {'L', {0, 1, 2, 7, 3, 5, 6, 11, 8, 9, 10, 4}},
                                  {'D', {0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 9, 10}},
                                  {'E', {0, 1, 2, 3, 7, 4, 5, 6, 8, 9, 10, 11}}};
map<char, CornerPos> nextCornerPos = {{'R', {0, 5, 1, 3, 4, 6, 2, 7}},
                                      {'U', {1, 2, 3, 0, 4, 5, 6, 7}},
                                      {'F', {0, 1, 6, 2, 4, 5, 7, 3}},
                                      {'B', {4, 0, 2, 3, 5, 1, 6, 7}},
                                      {'L', {3, 1, 2, 7, 0, 5, 6, 4}},
                                      {'D', {0, 1, 2, 3, 7, 4, 5, 6}}};
map<char, vector<map<int, int>>> nextEdgeOrientation = {{'R', {{{3, 3}, {2, 2}}, {{3, 2}, {1, 1}}, {{3, 3}, {-2, -2}}, {{3, 3}, {-1, -1}}, {{2, 2}, {-1, -1}}, {{2, -3}, {1, 1}}, {{-2, 3}, {1, 1}}, {{-2, -2}, {-1, -1}}, {{2, 2}, {-3, -3}}, {{1, 1}, {-3, -2}}, {{-3, -3}, {-2, -2}}, {{-3, -3}, {-1, -1}}}},
                                                        {'U', {{{3, 3}, {2, 1}}, {{3, 3}, {1, -2}}, {{3, 3}, {-2, -1}}, {{3, 3}, {-1, 2}}, {{2, 2}, {-1, -1}}, {{2, 2}, {1, 1}}, {{-2, -2}, {1, 1}}, {{-2, -2}, {-1, -1}}, {{2, 2}, {-3, -3}}, {{1, 1}, {-3, -3}}, {{-3, -3}, {-2, -2}}, {{-3, -3}, {-1, -1}}}},
                                                        {'F', {{{3, 3}, {2, 2}}, {{3, 3}, {1, 1}}, {{3, 1}, {-2, -2}}, {{3, 3}, {-1, -1}}, {{2, 2}, {-1, -1}}, {{2, 2}, {1, 1}}, {{-2, -2}, {1, -3}}, {{-2, -2}, {-1, 3}}, {{2, 2}, {-3, -3}}, {{1, 1}, {-3, -3}}, {{-3, -1}, {-2, -2}}, {{-3, -3}, {-1, -1}}}},
                                                        {'B', {{{3, -1}, {2, 2}}, {{3, 3}, {1, 1}}, {{3, 3}, {-2, -2}}, {{3, 3}, {-1, -1}}, {{2, 2}, {-1, -3}}, {{2, 2}, {1, 3}}, {{-2, -2}, {1, 1}}, {{-2, -2}, {-1, -1}}, {{2, 2}, {-3, 1}}, {{1, 1}, {-3, -3}}, {{-3, -3}, {-2, -2}}, {{-3, -3}, {-1, -1}}}},
                                                        {'L', {{{3, 3}, {2, 2}}, {{3, 3}, {1, 1}}, {{3, 3}, {-2, -2}}, {{3, -2}, {-1, -1}}, {{2, 3}, {-1, -1}}, {{2, 2}, {1, 1}}, {{-2, -2}, {1, 1}}, {{-2, -3}, {-1, -1}}, {{2, 2}, {-3, -3}}, {{1, 1}, {-3, -3}}, {{-3, -3}, {-2, -2}}, {{-3, 2}, {-1, -1}}}},
                                                        {'D', {{{3, 3}, {2, 2}}, {{3, 3}, {1, 1}}, {{3, 3}, {-2, -2}}, {{3, 3}, {-1, -1}}, {{2, 2}, {-1, -1}}, {{2, 2}, {1, 1}}, {{-2, -2}, {1, 1}}, {{-2, -2}, {-1, -1}}, {{2, -1}, {-3, -3}}, {{1, 2}, {-3, -3}}, {{-3, -3}, {-2, 1}}, {{-3, -3}, {-1, -2}}}}};
map<char, vector<map<int, int>>> nextCornerOrientation = {{'R', {{{-1, -1}, {2, 2}, {3, 3}}, {{1, 1}, {2, -3}, {3, 2}}, {{1, 1}, {-2, 3}, {3, 2}}, {{-1, -1}, {-2, -2}, {3, 3}}, {{-1, -1}, {2, 2}, {-3, -3}}, {{1, 1}, {2, -3}, {-3, -2}}, {{1, 1}, {-2, 3}, {-3, -2}}, {{-1, -1}, {-2, -2}, {-3, -3}}}},
                                                          {'U', {{{-1, 2}, {2, 1}, {3, 3}}, {{1, -2}, {2, 1}, {3, 3}}, {{1, -2}, {-2, -1}, {3, 3}}, {{-1, 2}, {-2, -1}, {3, 3}}, {{-1, -1}, {2, 2}, {-3, -3}}, {{1, 1}, {2, 2}, {-3, -3}}, {{1, 1}, {-2, -2}, {-3, -3}}, {{-1, -1}, {-2, -2}, {-3, -3}}}},
                                                          {'F', {{{-1, -1}, {2, 2}, {3, 3}}, {{1, 1}, {2, 2}, {3, 3}}, {{1, -3}, {-2, -2}, {3, 1}}, {{-1, 3}, {-2, -2}, {3, 1}}, {{-1, -1}, {2, 2}, {-3, -3}}, {{1, 1}, {2, 2}, {-3, -3}}, {{1, -3}, {-2, -2}, {-3, -1}}, {{-1, 3}, {-2, -2}, {-3, -1}}}},
                                                          {'B', {{{-1, -3}, {2, 2}, {3, -1}}, {{1, 3}, {2, 2}, {3, -1}}, {{1, 1}, {-2, -2}, {3, 3}}, {{-1, -1}, {-2, -2}, {3, 3}}, {{-1, -3}, {2, 2}, {-3, 1}}, {{1, 3}, {2, 2}, {-3, 1}}, {{1, 1}, {-2, -2}, {-3, -3}}, {{-1, -1}, {-2, -2}, {-3, -3}}}},
                                                          {'L', {{{-1, -1}, {2, 3}, {3, -2}}, {{1, 1}, {2, 2}, {3, 3}}, {{1, 1}, {-2, -2}, {3, 3}}, {{-1, -1}, {-2, -3}, {3, -2}}, {{-1, -1}, {2, 3}, {-3, 2}}, {{1, 1}, {2, 2}, {-3, -3}}, {{1, 1}, {-2, -2}, {-3, -3}}, {{-1, -1}, {-2, -3}, {-3, 2}}}},
                                                          {'D', {{{-1, -1}, {2, 2}, {3, 3}}, {{1, 1}, {2, 2}, {3, 3}}, {{1, 1}, {-2, -2}, {3, 3}}, {{-1, -1}, {-2, -2}, {3, 3}}, {{-1, -2}, {2, -1}, {-3, -3}}, {{1, 2}, {2, -1}, {-3, -3}}, {{1, 2}, {-2, 1}, {-3, -3}}, {{-1, -2}, {-2, 1}, {-3, -3}}}}};
EdgePos SOLVED_EDGE_POS = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
CornerPos SOLVED_CORNER_POS = {0, 1, 2, 3, 4, 5, 6, 7};
edgeOrientation SOLVED_EDGE_ORIENTATION = {3, 3, 3, 3, 2, 2, -2, -2, -3, -3, -3, -3};
cornerOrientation SOLVED_CORNER_ORIENTATION = {3, 3, 3, 3, -3, -3, -3, -3};
unordered_map<int8_t, int8_t> edgePosForNumber = {{0, 0b100010}, {1, 0b100100}, {2, 0b101000}, {3, 0b110000}, {4, 0b010010}, {5, 0b000110}, {6, 0b001100}, {7, 0b011000}, {8, 0b000011}, {9, 0b000101}, {10, 0b001001}, {11, 0b010001}};
unordered_map<int8_t, vector<int8_t>> edgeIndicesForPos = {{0b100010, {1, 37}}, {0b100100, {5, 28}}, {0b101000, {7, 19}}, {0b110000, {3, 10}}, {0b010010, {12, 41}}, {0b000110, {32, 39}}, {0b001100, {23, 30}}, {0b011000, {14, 21}}, {0b000011, {43, 52}}, {0b000101, {34, 50}}, {0b001001, {25, 46}}, {0b010001, {16, 48}}};
unordered_map<int8_t, int8_t> cornerPosForNumber = {{0, 0b110010}, {1, 0b100110}, {2, 0b101100}, {3, 0b111000}, {4, 0b010011}, {5, 0b000111}, {6, 0b001101}, {7, 0b011001}};
unordered_map<int8_t, vector<int8_t>> cornerIndicesForPos = {{0b110010, {0, 9, 38}}, {0b100110, {2, 29, 36}}, {0b101100, {8, 20, 27}}, {0b111000, {6, 11, 18}}, {0b010011, {15, 44, 51}}, {0b000111, {35, 42, 53}}, {0b001101, {26, 33, 47}}, {0b011001, {17, 24, 45}}};
unordered_map<int8_t, int8_t> edgeNumberForPos = {{0b100010, 0}, {0b100100, 1}, {0b101000, 2}, {0b110000, 3}, {0b010010, 4}, {0b000110, 5}, {0b001100, 6}, {0b011000, 7}, {0b000011, 8}, {0b000101, 9}, {0b001001, 10}, {0b010001, 11}};
unordered_map<int8_t, int8_t> cornerNumberForPos = {{0b110010, 0}, {0b100110, 1}, {0b101100, 2}, {0b111000, 3}, {0b010011, 4}, {0b000111, 5}, {0b001101, 6}, {0b011001, 7}};
Cube Execute(Cube c, string s)
{
    Cube temp = c;
    for (auto i : s)
    {
        for (int j = 0; j < 12; j++)
            temp.first.second[j] = nextEdgeOrientation[i][temp.first.first[j]][temp.first.second[j]],
            temp.first.first[j] = nextEdgePos[i][temp.first.first[j]];
        for (int j = 0; j < 8; j++)
            temp.second.second[j] = nextCornerOrientation[i][temp.second.first[j]][temp.second.second[j]],
            temp.second.first[j] = nextCornerPos[i][temp.second.first[j]];
    }
    return temp;
}
void printStatus(Cube c)
{
    cout << "{" << c.first.first[0];
    for (int i = 1; i < 12; i++)
        cout << "," << c.first.first[i];
    cout << "}\t";
    cout << "{" << c.first.second[0];
    for (int i = 1; i < 12; i++)
        cout << "," << c.first.second[i];
    cout << "}\t";
    cout << "{" << c.second.first[0];
    for (int i = 1; i < 8; i++)
        cout << "," << c.second.first[i];
    cout << "}\t";
    cout << "{" << c.second.second[0];
    for (int i = 1; i < 8; i++)
        cout << "," << c.second.second[i];
    cout << "}\n";
}
string reverseAlgorithm(string s)
{
    string result = "";
    for (auto i : s)
        for (int j = 0; j < 3; j++)
            result += i;
    reverse(result.begin(), result.end());
    return result;
}
vector<int> maskG2condition(Cube c)
{
    vector<int> result;
    for (int i = 0; i < 12; i++)
    {
        result.push_back(c.first.first[i]);
        // result.push_back(c.first.second[i]);
    }
    for (int i = 0; i < 8; i++)
    {
        result.push_back(c.second.first[i]);
        // result.push_back(c.second.second[i]);
    }
    return result;
}
string solveG2(Cube c)
{
    Cube source = c, destination = {{SOLVED_EDGE_POS, SOLVED_EDGE_ORIENTATION}, {SOLVED_CORNER_POS, SOLVED_CORNER_ORIENTATION}};
    vector<string> operations = {"U", "UU", "UUU", "D", "DD", "DDD", "RR", "FF", "LL", "BB"};
    // vector<string> operations1 = {"U", "D", "RR", "FF", "LL", "BB"}, operations2 = {"UU", "DD", "RR", " FF ", " LL ", "BB"};
    queue<pair<Cube, string>> forward, backward;
    map<vector<int>, string> f_sol, b_sol;
    f_sol[maskG2condition(source)] = b_sol[maskG2condition(destination)] = "";
    forward.push({source, ""});
    backward.push({destination, ""});
    while (true)
    {
        auto f = forward.front();
        forward.pop();
        // for (auto i : randomize(operations1))
        for (auto i : (operations))
        // for (auto i : (operations1))
        {
            Cube temp = Execute(f.first, i);
            vector<int> x = maskG2condition(temp);
            if (b_sol.count(x))
                return f.second + i + reverseAlgorithm(b_sol[x]);
            if (!f_sol.count(x))
            {
                f_sol[x] = f.second + i;
                forward.push({temp, f.second + i});
            }
        }
        auto b = backward.front();
        backward.pop();
        // for (auto i : randomize(operations2))
        for (auto i : (operations))
        // for (auto i : (operations2))
        {
            Cube temp = Execute(b.first, i);
            vector<int> x = maskG2condition(temp);
            if (f_sol.count(x))
                return f_sol[x] + reverseAlgorithm(b.second + i);
            if (!b_sol.count(x))
            {
                b_sol[x] = b.second + i;
                backward.push({temp, b.second + i});
            }
        }
    }
}
vector<string> alg(string moves)
{
    stack<pair<char, int>> s;
    vector<string> v = {"", "", "2", "'"}, result;
    for (auto i : moves)
    {
        if (s.empty() || s.top().first != i)
            s.push({i, 1});
        else
        {
            auto x = s.top();
            s.pop();
            if (x.second != 3)
                s.push({x.first, x.second + 1});
        }
    }
    while (!s.empty())
    {
        auto x = s.top();
        if (x.second)
            result.insert(begin(result), x.first + v[x.second]);
        s.pop();
    }
    return result;
}
vector<int> maskG1Condition(Cube c)
{
    vector<pair<int, int>> edgeMask(12);
    vector<int> cornerMask(8), finalMask;
    for (int i = 0; i < 4; i++)
        edgeMask[c.first.first[i]] = {0, c.first.second[i]};
    for (int i = 4; i < 8; i++)
        edgeMask[c.first.first[i]] = {1, c.first.second[i]};
    for (int i = 8; i < 12; i++)
        edgeMask[c.first.first[i]] = {0, c.first.second[i]};
    for (int i = 0; i < 8; i++)
        cornerMask[c.second.first[i]] = c.second.second[i];
    for (auto i : edgeMask)
    {
        finalMask.push_back(i.first);
        finalMask.push_back(i.second);
    }
    for (auto i : cornerMask)
        finalMask.push_back(i);
    return finalMask;
}
string solveG1(Cube c)
{
    Cube source = c, destination = {{SOLVED_EDGE_POS, SOLVED_EDGE_ORIENTATION}, {SOLVED_CORNER_POS, SOLVED_CORNER_ORIENTATION}};
    vector<string> operations = {"R", "RR", "RRR", "U", "UU", "UUU", "F", "FF", "FFF", "L", "LL", "LLL", "B", "BB", "BBB", "D", "DD", "DDD"};
    queue<pair<Cube, string>> forward, backward;
    map<vector<int>, string> f_sol, b_sol;
    f_sol[maskG1Condition(source)] = b_sol[maskG1Condition(destination)] = "";
    forward.push({source, ""});
    backward.push({destination, ""});
    while (true)
    {
        auto f = forward.front();
        forward.pop();
        for (auto i : operations)
        {
            Cube temp = Execute(f.first, i);
            vector<int> x = maskG1Condition(temp);
            if (b_sol.count(x))
                return f.second + i + reverseAlgorithm(b_sol[x]);
            if (!f_sol.count(x))
            {
                f_sol[x] = f.second + i;
                forward.push({temp, f.second + i});
            }
        }
        auto b = backward.front();
        backward.pop();
        for (auto i : operations)
        {
            Cube temp = Execute(b.first, i);
            vector<int> x = maskG1Condition(temp);
            if (f_sol.count(x))
                return f_sol[x] + reverseAlgorithm(b.second + i);
            if (!b_sol.count(x))
            {
                b_sol[x] = b.second + i;
                backward.push({temp, b.second + i});
            }
        }
    }
}
vector<string> splitString(string s)
{
    vector<string> result;
    result.push_back("");
    for (auto i : s)
    {
        if (i == ' ')
            result.push_back("");
        else
            result.back().push_back(i);
    }
    return result;
}
string ip(vector<string> s)
{
    string result = "";
    for (auto i : s)
    {
        if (i.size() == 1)
            result += i;
        else
        {
            if (i[1] == '\'')
                for (int8_t j = 0; j < 3; j++)
                    result += i[0];
            else
                for (int8_t j = 0; j < 2; j++)
                    result += i[0];
        }
    }
    return result;
}
Cube colorToMatrix(string colorInput)
{
    Cube c = {{SOLVED_EDGE_POS, SOLVED_EDGE_ORIENTATION}, {SOLVED_CORNER_POS, SOLVED_CORNER_ORIENTATION}};
    string basicPositions = "ULFRBD", givenPositions = "";
    vector<int8_t> basicPositionsInfo = {4, 13, 22, 31, 40, 49}, basicOrientationInfo = {3, -1, -2, 1, 2, -3};
    for (auto i : basicPositionsInfo)
        givenPositions += colorInput[i];
    for (int8_t i = 0; i < 12; i++)
    {
        int8_t x = 0;
        for (auto j : givenPositions)
        {
            int8_t flag = 0;
            for (auto k : edgeIndicesForPos[edgePosForNumber[i]])
                if (colorInput[k] == j)
                    flag = 1;
            x = 2 * x + flag;
        }
        c.first.first[edgeNumberForPos[x]] = i;
    }
    for (int8_t i = 0; i < 8; i++)
    {
        int8_t x = 0;
        for (auto j : givenPositions)
        {
            int8_t flag = 0;
            for (auto k : cornerIndicesForPos[cornerPosForNumber[i]])
                if (colorInput[k] == j)
                    flag = 1;
            x = 2 * x + flag;
        }
        c.second.first[cornerNumberForPos[x]] = i;
    }
    for (int8_t i = 0; i < 4; i++)
        for (auto j : edgeIndicesForPos[edgePosForNumber[c.first.first[i]]])
            if (colorInput[j] == givenPositions[0])
                c.first.second[i] = basicOrientationInfo[j / 9];
    for (int8_t i = 4; i < 8; i++)
        for (auto j : edgeIndicesForPos[edgePosForNumber[c.first.first[i]]])
            if (colorInput[j] == givenPositions[2] || colorInput[j] == givenPositions[4])
                c.first.second[i] = basicOrientationInfo[j / 9];
    for (int8_t i = 8; i < 12; i++)
        for (auto j : edgeIndicesForPos[edgePosForNumber[c.first.first[i]]])
            if (colorInput[j] == givenPositions[5])
                c.first.second[i] = basicOrientationInfo[j / 9];
    for (int8_t i = 0; i < 8; i++)
        for (auto j : cornerIndicesForPos[cornerPosForNumber[c.second.first[i]]])
            if (colorInput[j] == givenPositions[0] || colorInput[j] == givenPositions[5])
                c.second.second[i] = basicOrientationInfo[j / 9];
    return c;
}
vector<string> solveCube(Cube c)
{
    string G0_to_G1 = solveG1(c);
    c = Execute(c, G0_to_G1);
    string G1_to_G2 = solveG2(c);
    return alg(G0_to_G1 + G1_to_G2);
}
const char *generateCharArray(const std::string &userInput)
{
    char *charArray = new char[userInput.length() + 1];
    strcpy(charArray, userInput.c_str());
    return charArray;
}
EM_PORT_API(const char *)
solver(int8_t ep0, int8_t ep1, int8_t ep2, int8_t ep3, int8_t ep4, int8_t ep5, int8_t ep6, int8_t ep7, int8_t ep8, int8_t ep9, int8_t ep10, int8_t ep11,
       int8_t eo0, int8_t eo1, int8_t eo2, int8_t eo3, int8_t eo4, int8_t eo5, int8_t eo6, int8_t eo7, int8_t eo8, int8_t eo9, int8_t eo10, int8_t eo11,
       int8_t cp0, int8_t cp1, int8_t cp2, int8_t cp3, int8_t cp4, int8_t cp5, int8_t cp6, int8_t cp7,
       int8_t co0, int8_t co1, int8_t co2, int8_t co3, int8_t co4, int8_t co5, int8_t co6, int8_t co7)
{
    vector<string> algorithm = solveCube({{{ep0, ep1, ep2, ep3, ep4, ep5, ep6, ep7, ep8, ep9, ep10, ep11},
                                           {eo0, eo1, eo2, eo3, eo4, eo5, eo6, eo7, eo8, eo9, eo10, eo11}},
                                          {{cp0, cp1, cp2, cp3, cp4, cp5, cp6, cp7},
                                           {co0, co1, co2, co3, co4, co5, co6, co7}}});
    string res = "";
    for (auto i : algorithm)
        res += i + " ";
    return generateCharArray(res);
}
int main()
{
    return 0;
}